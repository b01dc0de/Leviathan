#include "Leviathan.h" // Includes ImagePNG.h 

namespace Leviathan
{

enum struct PNGChunkType
{
    Other,
    IHDR,
    PLTE,
    IDAT,
    IEND,
};

struct PNGChunk
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

enum struct PNGColorType
{
    Invalid,
    TruecolorAlpha,
    GrayscaleAlpha,
    Truecolor,
    Grayscale,
    PaletteIndexed
};

struct PNGChunkDataIHDR
{
    u32 Width;
    u32 Height;
    byte BitDepth;
    byte ColorType;
    byte CompressionMethod;
    byte FilterMethod;
    byte InterlaceMethod;
};

struct PaletteEntry
{
    u8 R;
    u8 G;
    u8 B;
};

struct PNGChunkDataPLTE
{
    PaletteEntry* Entries = nullptr;
    u8 NumEntries = 0;
};

struct PNGParseContext
{
    byte* Contents;
    size_t Size;
    size_t ReadIdx;
    bool bError;
    bool bReadHeader;
    bool bReadEnd;
    bool bReadingImageData;
    PNGChunkDataIHDR HeaderChunk;
    PNGChunkDataPLTE PaletteChunk;

    Array<byte> ImageData;
    static constexpr byte FileSignature[] = { 137, 80, 78, 71, 13, 10, 26, 10 };

    PNGParseContext(FileContentsT& InFileContents, ImageT& OutImage)
    {
        Contents = InFileContents.Contents;
        Size = InFileContents.Size;
        ReadIdx = 0;
        bError = false;
        bReadHeader = false;
        bReadEnd = false;
        bReadingImageData = false;
        HeaderChunk = {};
        PaletteChunk = {};

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
        u8 Value0 = (Value & 0x000000FF) >> 0;
        u8 Value1 = (Value & 0x0000FF00) >> 8;
        u8 Value2 = (Value & 0x00FF0000) >> 16;
        u8 Value3 = (Value & 0xFF000000) >> 24;
        u32 Result = (Value0 << 24) | (Value1 << 16) | (Value2 << 8) | (Value3 << 0);
        return Result;
    }
    void ReadChunk(PNGChunk& NewChunk)
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
    PNGChunkType GetChunkType(PNGChunk& InChunk)
    {
        const PNGChunkType ChunkTypeValues[] = { PNGChunkType::IHDR, PNGChunkType::PLTE, PNGChunkType::IEND, PNGChunkType::IDAT, };
        const char* ChunkTypeLiterals[] = { "IHDR", "PLTE", "IEND", "IDAT", };
        ASSERT(ARRAY_SIZE(ChunkTypeValues) == ARRAY_SIZE(ChunkTypeLiterals));

        PNGChunkType Result = PNGChunkType::Other;
        for (int ChunkIdx = 0; ChunkIdx < ARRAY_SIZE(ChunkTypeLiterals); ChunkIdx++)
        {
            if (InChunk.Type == *(u32*)ChunkTypeLiterals[ChunkIdx]) { Result = ChunkTypeValues[ChunkIdx]; break; }
        }
        return Result;
    }
    PNGColorType GetColorType(byte ColorType)
    {
        PNGColorType Result = PNGColorType::Invalid;
        switch (ColorType)
        {
            case 0: { Result = PNGColorType::Grayscale; } break;
            case 2: { Result = PNGColorType::Truecolor; } break;
            case 3: { Result = PNGColorType::PaletteIndexed; } break;
            case 4: { Result = PNGColorType::GrayscaleAlpha; } break;
            case 6: { Result = PNGColorType::TruecolorAlpha; } break;
        }
        return Result;
    }
    void HandleChunk(PNGChunk& InChunk)
    {
        PNGChunkType ChunkType = GetChunkType(InChunk);
        switch (ChunkType)
        {
            case PNGChunkType::Other:
            {
                if (!bReadHeader) { bError = true; }
                // Check that we're not skipping a critical chunk
                ASSERT(InChunk.bAncillary);
                delete[] InChunk.Data;
            } break;
            case PNGChunkType::IHDR:
            {
                ASSERT(!bReadHeader);
                if (!bReadHeader)
                {
                    HeaderChunk = *(PNGChunkDataIHDR*)InChunk.Data;
                    HeaderChunk.Width = SwapEndiannessU32(HeaderChunk.Width);
                    HeaderChunk.Height = SwapEndiannessU32(HeaderChunk.Height);
                    bReadHeader = true;
                }
                else { bError = true; }
                delete[] InChunk.Data;
            } break; 
            case PNGChunkType::PLTE:
            {
                ASSERT(PaletteChunk.Entries == nullptr);
                // Palette chunk must come before any IDAT chunks
                ASSERT(ImageData.Num == 0);
                bool bValidPaletteSize = InChunk.Length % 3 == 0;
                ASSERT(bValidPaletteSize);
                if (bReadHeader && ImageData.Num == 0 && PaletteChunk.Entries == nullptr && bValidPaletteSize)
                {
                    PaletteChunk.Entries = (PaletteEntry*)InChunk.Data;
                    PaletteChunk.NumEntries = InChunk.Length / 3;
                }
                else
                {
                    bError = true; 
                    delete[] InChunk.Data;
                }
            } break;
            case PNGChunkType::IEND:
            {
                if (bReadHeader) { bReadEnd = true; }
                else { bError = true; }
                ASSERT(InChunk.Length == 0 && InChunk.Data == nullptr);
            } break;
            case PNGChunkType::IDAT:
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
        if (bReadingImageData && ChunkType != PNGChunkType::IDAT) { bReadingImageData = false; }
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
                PNGChunk NewChunk{};
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
                // TODO(CKA): Actually test this code path with uncompressed PNG image data
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
    void ConstructImage(ImageT& OutImage)
    {
        ASSERT(bReadHeader && bReadEnd && ImageData.Num > 0);
        ASSERT(OutImage.PxBuffer == nullptr);

        if (ImageData.Num > 0)
        {
            PNGColorType ColorType = GetColorType(HeaderChunk.ColorType);
            ASSERT(ColorType != PNGColorType::Invalid);
            ASSERT(ColorType != PNGColorType::Truecolor || (HeaderChunk.BitDepth == 8 || HeaderChunk.BitDepth == 16));
            ASSERT(ColorType != PNGColorType::PaletteIndexed || (HeaderChunk.BitDepth == 1 || HeaderChunk.BitDepth == 2 || HeaderChunk.BitDepth == 4 || HeaderChunk.BitDepth == 8));
            ASSERT(!(ColorType == PNGColorType::GrayscaleAlpha || ColorType == PNGColorType::TruecolorAlpha) || (HeaderChunk.BitDepth == 8 || HeaderChunk.BitDepth == 16));
            ASSERT(HeaderChunk.CompressionMethod == 0);
            ASSERT(HeaderChunk.FilterMethod == 0);
            ASSERT(HeaderChunk.InterlaceMethod == 0 || HeaderChunk.InterlaceMethod == 1);
            bool bAdam7Interlace = HeaderChunk.InterlaceMethod == 1;
            ASSERT(!bAdam7Interlace);

            Array<byte> DecompressedStream;
            DecompressImageData(ImageData, DecompressedStream);
            ASSERT(DecompressedStream.Num > 0);

            OutImage.Width = HeaderChunk.Width;
            OutImage.Height = HeaderChunk.Height;
            OutImage.PxCount = OutImage.Width * OutImage.Height;
            OutImage.PxBufferSize = sizeof(RGBA32) * OutImage.PxCount;
            OutImage.PxBuffer = new RGBA32[OutImage.PxCount];

            Array<byte> UnfilteredStream;
            size_t SamplesPerPixel = 0;
            if (ColorType == PNGColorType::Grayscale) { SamplesPerPixel = 1; }
            else if (ColorType == PNGColorType::Truecolor) { SamplesPerPixel = 3; }
            else if (ColorType == PNGColorType::PaletteIndexed) { SamplesPerPixel = 1; }
            else if (ColorType == PNGColorType::GrayscaleAlpha) { SamplesPerPixel = 2; }
            else if (ColorType == PNGColorType::TruecolorAlpha) { SamplesPerPixel = 4; }
            size_t BitsPerPixel = HeaderChunk.BitDepth * SamplesPerPixel;
            size_t BytesPerScanline = 1 + (OutImage.Width * BitsPerPixel / 8);
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
                    // TODO(CKA): These calculations are off
                    //      They need to be adjusted for different bit depths / samples per pixel / etc
                    bool bLeftColumnPx = StreamIdx % BytesPerScanline == 1;
                    bool bTopRowPx = StreamIdx / BytesPerScanline == 0;
                    byte X = DecompressedStream[StreamIdx];
                    byte A = bLeftColumnPx ? 0 : DecompressedStream[StreamIdx - 1];
                    byte B = bTopRowPx ? 0 : DecompressedStream[StreamIdx - BytesPerScanline];
                    byte C = (bLeftColumnPx || bTopRowPx) ? 0 : DecompressedStream[StreamIdx - BytesPerScanline - 1];

                    switch (ActiveFilterMethod)
                    {
                        case 0: { } break; // None
                        case 1: { X = X + A; } break; // Sub
                        case 2: { X = X + B; } break; // Up
                        case 3: { X = X + ((A + B) / 2); } break; // Average
                        case 4: // Paeth
                        {
                            auto PaethPredictor = [](byte A, byte B, byte C) -> byte
                            {
                                byte P = A + B - C;
                                byte PA = Abs(P - A);
                                byte PB = Abs(P - B);
                                byte PC = Abs(P - C);
                                if (PA <= PB && PA <= PC) { return A; }
                                else if (PB <= PC) { return B; }
                                else { return C; }
                            };
                            X = X + PaethPredictor(A, B, C);
                        } break;
                        default: { ASSERT(false); } break;
                    }
                    UnfilteredStream.Add(X);
                }
            }
            ASSERT(UnfilteredStream.Num > 0);
            switch (ColorType)
            {
                case PNGColorType::TruecolorAlpha:
                {
                    ASSERT(false);
                } break;
                case PNGColorType::GrayscaleAlpha:
                {
                    ASSERT(false);
                } break;
                case PNGColorType::Truecolor:
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
                case PNGColorType::Grayscale:
                {
                    ASSERT(false);
                } break;
                case PNGColorType::PaletteIndexed:
                {
                    ASSERT(false);
                } break;
            }
        }
    }
    void CleanUp()
    {
        if (PaletteChunk.Entries)
        {
            byte* PaletteChunkData = (byte*)PaletteChunk.Entries;
            delete[] PaletteChunkData;
        }
    }
};

void LoadPNGFile(const char* Filename, ImageT& OutImage)
{
    FileContentsT LoadedPNG = LoadFileContents(Filename);
    ASSERT(LoadedPNG.Contents && LoadedPNG.Size > 0);
    if (LoadedPNG.Contents && LoadedPNG.Size > 0)
    {
        PNGParseContext Context{LoadedPNG, OutImage};

        delete[] LoadedPNG.Contents;
    }
}

}
