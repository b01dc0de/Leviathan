#include "Leviathan.h" // Includes ImagePNG.h 

namespace Leviathan
{

namespace PNG
{

enum struct ChunkType
{
    Other,
    IHDR,
    PLTE,
    IDAT,
    IEND,
};

struct Chunk
{
    u32 Length = 0;
    u32 Type = 0;
    bool bAncillary = false;
    bool bPrivate = false;
    bool bReserved = false;
    bool bSafeToCopy = false;
    byte* Data = nullptr;
    u32 CRC = 0;
};

enum struct ColorFormat
{
    Invalid,
    TruecolorAlpha,
    GrayscaleAlpha,
    Truecolor,
    Grayscale,
    PaletteIndexed
};

struct HeaderData
{
    u32 Width;
    u32 Height;
    byte BitDepth;
    byte ColorType;
    byte CompressionMethod;
    byte FilterMethod;
    byte InterlaceMethod;
};

struct PaletteData
{
    struct Entry
    {
        u8 R;
        u8 G;
        u8 B;
    };

    Entry* Entries = nullptr;
    u8 NumEntries = 0;
};

struct ParseContext
{
    byte* Contents;
    size_t Size;
    size_t ReadIdx;
    bool bError;
    bool bReadHeader;
    bool bReadEnd;
    bool bReadingImageData;

    HeaderData Header;
    PaletteData Palette;
    Array<byte> ImageData;

    static constexpr byte FileSignature[] = { 137, 80, 78, 71, 13, 10, 26, 10 };

    ParseContext(FileContentsT& InFileContents, ImageT& OutImage)
    {
        Contents = InFileContents.Contents;
        Size = InFileContents.Size;
        ReadIdx = 0;
        bError = false;
        bReadHeader = false;
        bReadEnd = false;
        bReadingImageData = false;
        Header = {};
        Palette = {};

        Parse();
        ASSERT(!bError);
        if (!bError)
        {
            ConstructImage(OutImage);
        }
        CleanUp();
    }
    bool CheckSignature()
    {
        ASSERT(Contents && Size > 0 && ReadIdx == 0);
        bool bValidSig = true;
        for (int Idx = 0; Idx < ARRAY_SIZE(FileSignature); Idx++)
        {
            if (FileSignature[Idx] != Contents[Idx])
            {
                bValidSig = false;
                break;
            }
        }
        return bValidSig;
    }
    u32 SwapEndiannessU32(u32 Value)
    {
        u32 Result = ((Value & 0x000000FF) << 24) | ((Value & 0x0000FF00) << 8) |
            ((Value & 0x00FF0000) >> 8) | ((Value & 0xFF000000) >> 24);
        return Result;
    }
    void ReadChunk(Chunk& NewChunk)
    {
        ASSERT(!bError);
        if (!bError)
        {
            NewChunk = {};
            NewChunk.Length = SwapEndiannessU32(*(u32*)(Contents + ReadIdx));
            ReadIdx += sizeof(u32);
            NewChunk.Type = *(u32*)(Contents + ReadIdx);
            NewChunk.bAncillary = NewChunk.Type & 0x20;
            NewChunk.bPrivate = NewChunk.Type & 0x2000;
            NewChunk.bReserved = NewChunk.Type & 0x200000;
            NewChunk.bSafeToCopy = NewChunk.Type & 0x20000000;
            ReadIdx += sizeof(u32);
            if (NewChunk.Length > 0)
            {
                NewChunk.Data = new byte[NewChunk.Length];
                memcpy(NewChunk.Data, Contents + ReadIdx, NewChunk.Length);
                ReadIdx += NewChunk.Length;
            }
            NewChunk.CRC = SwapEndiannessU32(*(u32*)(Contents + ReadIdx));
            ReadIdx += sizeof(u32);
            // TODO(CKA): Verify CRC

        }
    }
    ChunkType GetChunkType(Chunk& InChunk)
    {
        const ChunkType ChunkTypeValues[] = { ChunkType::IHDR, ChunkType::PLTE, ChunkType::IEND, ChunkType::IDAT, };
        const char* ChunkTypeLiterals[] = { "IHDR", "PLTE", "IEND", "IDAT", };
        ASSERT(ARRAY_SIZE(ChunkTypeValues) == ARRAY_SIZE(ChunkTypeLiterals));

        ChunkType Result = ChunkType::Other;
        for (int ChunkIdx = 0; ChunkIdx < ARRAY_SIZE(ChunkTypeLiterals); ChunkIdx++)
        {
            if (InChunk.Type == *(u32*)ChunkTypeLiterals[ChunkIdx]) { Result = ChunkTypeValues[ChunkIdx]; break; }
        }
        return Result;
    }
    ColorFormat GetColorType(byte Type)
    {
        ColorFormat Result = ColorFormat::Invalid;
        switch (Type)
        {
            case 0: { Result = ColorFormat::Grayscale; } break;
            case 2: { Result = ColorFormat::Truecolor; } break;
            case 3: { Result = ColorFormat::PaletteIndexed; } break;
            case 4: { Result = ColorFormat::GrayscaleAlpha; } break;
            case 6: { Result = ColorFormat::TruecolorAlpha; } break;
        }
        return Result;
    }
    void HandleChunk(Chunk& InChunk)
    {
        ChunkType ChunkType = GetChunkType(InChunk);
        switch (ChunkType)
        {
            case ChunkType::Other:
            {
                if (!bReadHeader) { bError = true; }
                // Check that we're not skipping a critical chunk
                ASSERT(InChunk.bAncillary);
                delete[] InChunk.Data;
            } break;
            case ChunkType::IHDR:
            {
                ASSERT(!bReadHeader);
                if (!bReadHeader)
                {
                    Header = *(HeaderData*)InChunk.Data;
                    Header.Width = SwapEndiannessU32(Header.Width);
                    Header.Height = SwapEndiannessU32(Header.Height);
                    bReadHeader = true;
                }
                else { bError = true; }
                delete[] InChunk.Data;
            } break; 
            case ChunkType::PLTE:
            {
                ASSERT(Palette.Entries == nullptr);
                // Palette chunk must come before any IDAT chunks
                ASSERT(ImageData.Num == 0);
                bool bValidPaletteSize = InChunk.Length % 3 == 0;
                ASSERT(bValidPaletteSize);
                if (bReadHeader && ImageData.Num == 0 && Palette.Entries == nullptr && bValidPaletteSize)
                {
                    Palette.Entries = (PaletteData::Entry*)InChunk.Data;
                    Palette.NumEntries = InChunk.Length / 3;
                }
                else
                {
                    bError = true; 
                    delete[] InChunk.Data;
                }
            } break;
            case ChunkType::IEND:
            {
                if (bReadHeader) { bReadEnd = true; }
                else { bError = true; }
                ASSERT(InChunk.Length == 0 && InChunk.Data == nullptr);
            } break;
            case ChunkType::IDAT:
            {
                ASSERT(ImageData.Num == 0 || bReadingImageData);
                if (ImageData.Num == 0) { bReadingImageData = true; }
                if (bReadHeader && bReadingImageData)
                {
                    ImageData.Add(InChunk.Data, InChunk.Length);
                }
                else
                {
                    bError = true;
                }
                delete[] InChunk.Data;
            } break;
            default:
            {
                ASSERT(false);
            } break;
        }
        if (bReadingImageData && ChunkType != ChunkType::IDAT) { bReadingImageData = false; }
    }
    void Parse()
    {
        ASSERT(Contents && Size > 0 && ReadIdx == 0 && ImageData.Num == 0);
        bool bValidSignature = CheckSignature();
        bError = !bValidSignature;
        if (bValidSignature)
        {
            ReadIdx = ARRAY_SIZE(FileSignature);
            while (!bError && !bReadEnd && ReadIdx < Size)
            {
                Chunk NewChunk{};
                ReadChunk(NewChunk);
                HandleChunk(NewChunk);
            }
        }
        bError |= ReadIdx != Size || !bReadHeader || !bReadEnd || ImageData.Num == 0;
        ASSERT(!bError);
    }
    void DecompressImageData(Array<byte>& ImageData, Array<byte>& OutStream)
    {
        ASSERT(ImageData.Num > 0);
        ASSERT(OutStream.Num == 0);
        // RFC 1950
        byte zlib_CompressionMethod = ImageData[0] & 0x0F;
        byte zlib_CompressionInfo = (ImageData[0] & 0xF0) >> 4;
        ASSERT(zlib_CompressionMethod == 8);
        ASSERT(zlib_CompressionInfo < 8);
        byte zlib_FlagsCheckBits = ImageData.Data[1] & 0x1F;
        bool zlib_bPresetDictionary = ImageData.Data[1] & 0x20;
        byte zlib_CompressionLevel = (ImageData.Data[1] & 0xC0) >> 6;
        ASSERT((ImageData.Data[0] * 256 + ImageData.Data[1]) % 31 == 0);
        ASSERT(!zlib_bPresetDictionary);
        // NOTE: zlib_CompressionLevel ==
        //      0 -> Compressor used fastest algorithm
        //      1 -> Compressor used fast algorithm
        //      2 -> Compressor used default algorithm
        //      3 -> Compressor used maximum compression, slowest algorithm

        // Decompress zlib data stream
        // RFC 1951
        int ByteReadIdx = 2;
        bool bFinal = false;
        while (!bFinal && ByteReadIdx < ImageData.Num)
        {
            // Read compressed block
            byte BlockHeaderByte = ImageData.Data[ByteReadIdx];
            bFinal = ImageData.Data[ByteReadIdx] & 0x01;
            byte BlockType = (ImageData.Data[ByteReadIdx] & 0x06) >> 1;
            ASSERT(BlockType != 3); // Reserved / invalid BTYPE
            if (BlockType == 0) // No compression
            {
                u16 LEN = *(u16*)(ImageData.Data + ByteReadIdx + 1);
                u16 NLEN = *(u16*)(ImageData.Data + ByteReadIdx + 3);
                ASSERT((ByteReadIdx + LEN + 5) <= ImageData.Num);
                OutStream.Add(ImageData.Data + ByteReadIdx + 5, LEN);
                ByteReadIdx += 5 + LEN;
            }
            else if (BlockType == 1) // Compressed (w/ fixed Huffman codes)
            {
                // TODO(CKA): Implement
                break;
            }
            else if (BlockType == 2) // Compressed (w/ dynamic Huffman codes)
            {
                byte HLIT = (ImageData[ByteReadIdx] & 0xF8) >> 3;
                byte HDIST = (ImageData[ByteReadIdx + 1] & 0x1F);
                byte HCLEN = ((ImageData[ByteReadIdx + 1] & 0xE0) >> 1) |
                    ((ImageData[ByteReadIdx + 2] & 0x01) << 3);

                ByteReadIdx += 2;
                int NextBit = 1;

                int NumCodeLengthCodes = (HCLEN + 4);
                int NumLiteralLengthCodes = HLIT + 257;
                int NumDistLengthCodes = HDIST + 1;

                // TODO(CKA): Draw the owl
                break;
            }
            else { ASSERT(false); }

            u32 CRC = SwapEndiannessU32(*(u32*)(ImageData.Data + ByteReadIdx));
            ByteReadIdx += 4;
            if (bFinal)
            {
                ASSERT(ByteReadIdx == ImageData.Num);
            }
        }
    }
    byte PaethPredictor(byte A, byte B, byte C)
    {
        byte P = A + B - C;
        byte PA = Abs(P - A);
        byte PB = Abs(P - B);
        byte PC = Abs(P - C);
        if (PA <= PB && PA <= PC) { return A; }
        else if (PB <= PC) { return B; }
        else { return C; }
    }
    void ReconstructImageData(Array<byte>& DecompressedStream, Array<byte>& UnfilteredStream)
    {
        size_t SamplesPerPixel = 0;
        ColorFormat ColorType = GetColorType(Header.ColorType);
        if (ColorType == ColorFormat::Grayscale) { SamplesPerPixel = 1; }
        else if (ColorType == ColorFormat::Truecolor) { SamplesPerPixel = 3; }
        else if (ColorType == ColorFormat::PaletteIndexed) { SamplesPerPixel = 1; }
        else if (ColorType == ColorFormat::GrayscaleAlpha) { SamplesPerPixel = 2; }
        else if (ColorType == ColorFormat::TruecolorAlpha) { SamplesPerPixel = 4; }
        ASSERT(SamplesPerPixel > 0);
        size_t BitsPerPixel = Header.BitDepth * SamplesPerPixel;
        u32 Width = Header.Width;
        ASSERT(Width * BitsPerPixel % 8 == 0); // TODO: Need to handle this case (bit padding)
        size_t BytesPerScanline = 1 + (Width * BitsPerPixel / 8);
        size_t BytesPerReconScanline = (Width * BitsPerPixel / 8);
        int ActiveFilterMethod = 0;
        for (size_t StreamIdx = 0; StreamIdx < DecompressedStream.Num; StreamIdx++)
        {
            bool bScanlineBegin = StreamIdx % BytesPerScanline == 0;
            if (bScanlineBegin)
            {
                ActiveFilterMethod = DecompressedStream[StreamIdx];
            }
            else
            {
                byte X = DecompressedStream[StreamIdx];
                if (ActiveFilterMethod != 0)
                {
                    // TODO(CKA): These calculations need to be adjusted for different bit depths / samples per pixel / etc
                    bool bLeftColumnPx = (StreamIdx % BytesPerScanline) < (SamplesPerPixel + 1);
                    bool bTopRowPx = (StreamIdx / BytesPerScanline) == 0;
                    int CurrRow = (StreamIdx / BytesPerScanline);
                    byte A = bLeftColumnPx ? 0 : UnfilteredStream[StreamIdx - SamplesPerPixel - CurrRow - 1];
                    byte B = bTopRowPx ? 0 : UnfilteredStream[StreamIdx - BytesPerReconScanline - CurrRow - 1];
                    byte C = (bLeftColumnPx || bTopRowPx) ? 0 : UnfilteredStream[StreamIdx - BytesPerReconScanline - SamplesPerPixel - CurrRow - 1];

                    switch (ActiveFilterMethod)
                    {
                        case 1: { X = X + A; } break; // Sub
                        case 2: { X = X + B; } break; // Up
                        case 3: { X = X + ((A + B) / 2); } break; // Average
                        case 4: { X = X + PaethPredictor(A, B, C); } break; // Paeth
                        default: { ASSERT(false); } break;
                    }
                }
                UnfilteredStream.Add(X);
            }
        }
    }
    void ConstructImage(ImageT& OutImage)
    {
        ASSERT(bReadHeader && bReadEnd && ImageData.Num > 0);
        ASSERT(OutImage.PxBuffer == nullptr);

        if (ImageData.Num > 0)
        {
            ColorFormat ColorType = GetColorType(Header.ColorType);
            ASSERT(ColorType != ColorFormat::Invalid);
            ASSERT(ColorType != ColorFormat::Truecolor || (Header.BitDepth == 8 || Header.BitDepth == 16));
            ASSERT(ColorType != ColorFormat::PaletteIndexed || (Header.BitDepth == 1 || Header.BitDepth == 2 || Header.BitDepth == 4 || Header.BitDepth == 8));
            ASSERT(!(ColorType == ColorFormat::GrayscaleAlpha || ColorType == ColorFormat::TruecolorAlpha) || (Header.BitDepth == 8 || Header.BitDepth == 16));
            ASSERT(Header.CompressionMethod == 0);
            ASSERT(Header.FilterMethod == 0);
            ASSERT(Header.InterlaceMethod == 0 || Header.InterlaceMethod == 1);
            bool bAdam7Interlace = Header.InterlaceMethod == 1;
            ASSERT(!bAdam7Interlace);

            Array<byte> DecompressedStream;
            DecompressImageData(ImageData, DecompressedStream);
            ASSERT(DecompressedStream.Num > 0);

            OutImage.Width = Header.Width;
            OutImage.Height = Header.Height;
            OutImage.PxCount = OutImage.Width * OutImage.Height;
            OutImage.PxBufferSize = sizeof(RGBA32) * OutImage.PxCount;
            OutImage.PxBuffer = new RGBA32[OutImage.PxCount];

            Array<byte> UnfilteredStream;
            ReconstructImageData(DecompressedStream, UnfilteredStream);

            ASSERT(UnfilteredStream.Num > 0);
            switch (ColorType)
            {
                case ColorFormat::TruecolorAlpha:
                {
                    ASSERT(false);
                } break;
                case ColorFormat::GrayscaleAlpha:
                {
                    ASSERT(false);
                } break;
                case ColorFormat::Truecolor:
                {
                    size_t StreamIdx = 0;
                    for (size_t PxIdx = 0; PxIdx < OutImage.PxCount; PxIdx++)
                    {
                        OutImage.PxBuffer[PxIdx] =
                        {
                            UnfilteredStream[StreamIdx + 0],
                            UnfilteredStream[StreamIdx + 1],
                            UnfilteredStream[StreamIdx + 2],
                            255
                        };
                        StreamIdx += 3;
                    }
                    ASSERT(StreamIdx == UnfilteredStream.Num);
                } break;
                case ColorFormat::Grayscale:
                {
                    ASSERT(false);
                } break;
                case ColorFormat::PaletteIndexed:
                {
                    ASSERT(false);
                } break;
            }
        }
    }
    void CleanUp()
    {
        if (Palette.Entries)
        {
            byte* PaletteChunkData = (byte*)Palette.Entries;
            delete[] PaletteChunkData;
        }
    }
};

}

void LoadPNGFile(const char* Filename, ImageT& OutImage)
{
    FileContentsT LoadedPNG = LoadFileContents(Filename);
    ASSERT(LoadedPNG.Contents && LoadedPNG.Size > 0);
    if (LoadedPNG.Contents && LoadedPNG.Size > 0)
    {
        PNG::ParseContext Context{LoadedPNG, OutImage};

        delete[] LoadedPNG.Contents;
    }
}

}
