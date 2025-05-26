#include "Leviathan.h"
#include "Image.h"

namespace Leviathan
{

void GetDebugImage(ImageT& OutImage)
{
    unsigned int DebugImageLength = 16;
    OutImage.Width = DebugImageLength;
    OutImage.Height = DebugImageLength;
    OutImage.PxCount = OutImage.Width * OutImage.Height;
    OutImage.PxBufferSize = sizeof(RGBA32) * OutImage.PxCount;
    OutImage.PxBuffer = new RGBA32[OutImage.PxCount];

    const RGBA32 Pink{ 255u, 73u, 173u, 255u };
    const RGBA32 Black{ 0u, 0u, 0u, 255u };
    const RGBA32 Red{ 255u, 0u, 0u, 255u };
    const RGBA32 Green{ 0u, 255u, 0u, 255u };
    const RGBA32 Blue{ 0u, 0u, 255u, 255u };
    const RGBA32 White{ 255u, 255u, 255u, 255u };
    for (int PxIdx = 0; PxIdx < OutImage.PxCount; PxIdx++)
    {
        int PxRow = PxIdx / OutImage.Width;
        int PxCol = PxIdx % OutImage.Width;
        if (PxRow == 0 && PxCol == 0)
        {
            OutImage.PxBuffer[PxIdx] = Red;
        }
        else if (PxRow == 0 && PxCol == (OutImage.Width - 1))
        {
            OutImage.PxBuffer[PxIdx] = Green;
        }
        else if (PxRow == (OutImage.Height - 1) && PxCol == 0)
        {
            OutImage.PxBuffer[PxIdx] = Blue;
        }
        else if (PxRow == (OutImage.Height - 1) && PxCol == (OutImage.Width - 1))
        {
            OutImage.PxBuffer[PxIdx] = White;
        }
        else
        {
            bool bEvenCell = (PxRow + PxCol) % 2 == 0;
            OutImage.PxBuffer[PxIdx] = bEvenCell ? Black : Pink;
        }
    }
}

#pragma pack(push, 1)
struct BMPFileHeader
{
    u16 Type; // Always Ascii BM
    u32 SizeInBytes; // Size (bytes) of file
    u16 Res1; // 0
    u16 Res2; // 0
    u32 OffsetBytes; // Offset (bytes) to actual pixel data
};

struct BMPInfoHeader
{
    u32 StructSize; // Size (bytes) of InfoHeader
    s32 Width;
    s32 Height; // NOTE(chris): If positive, pixel data is bottom to top
    u16 Planes; // Must be 1
    u16 BitsPerPixel; // Bits-per-pixel (0, 1, 4, 8, 16, 24, 32)
    u32 Compression; // *Should* be 0
    u32 Unused_ImgSize; // Only used if Compression is weird (not 0)
    s32 HRes; // Horizontal resolution
    s32 VRes; // Vertical resolution
    u32 ColorsUsed; // 0 for our purposes
    u32 ColorsImportant; // 0 for our purposes
};
struct BMPHeader
{
    BMPFileHeader FileHeader;
    BMPInfoHeader InfoHeader;
};
#pragma pack(pop)

void LoadBMPFile(const char* Filename, ImageT& OutImage)
{
    FileContentsT LoadedFile = LoadFileContents(Filename);
    if (LoadedFile.Size && LoadedFile.Contents)
    {
        u8* FileReadPtr = LoadedFile.Contents;
        BMPHeader BmpHdr = {};

        memcpy(&BmpHdr.FileHeader, FileReadPtr, sizeof(BMPFileHeader));
        FileReadPtr += sizeof(BMPFileHeader);
        memcpy(&BmpHdr.InfoHeader, FileReadPtr, sizeof(BMPInfoHeader));
        FileReadPtr += sizeof(BMPInfoHeader);

        if (BmpHdr.InfoHeader.BitsPerPixel == 32)
        {
            RGBA32* PxReadPtr = (RGBA32*)FileReadPtr;
            size_t BitmapFileSize = BmpHdr.FileHeader.SizeInBytes;
            size_t RemainingFileSize = BitmapFileSize - sizeof(BMPHeader);

            OutImage.Width = BmpHdr.InfoHeader.Width;
            OutImage.Height = BmpHdr.InfoHeader.Height;
            OutImage.PxCount = OutImage.Width * OutImage.Height;
            OutImage.PxBufferSize = sizeof(RGBA32) * OutImage.PxCount;
            OutImage.PxBuffer = new RGBA32[OutImage.PxCount];

            size_t ColorTableSize = RemainingFileSize - OutImage.PxBufferSize;
            constexpr bool bReadColorTable = false;
            if (bReadColorTable)
            {
                u8* ColorTable = new u8[ColorTableSize];
                memcpy(ColorTable, FileReadPtr, ColorTableSize);
                delete[] ColorTable;
            }
            FileReadPtr += ColorTableSize;
            if (BmpHdr.InfoHeader.Height < 0)
            {
                memcpy(OutImage.PxBuffer, FileReadPtr, OutImage.PxBufferSize);
            }
            else
            {
                size_t Stride = OutImage.Width * sizeof(RGBA32);
                RGBA32* PxBufferWrite = OutImage.PxBuffer;
                for (int RowIdx = OutImage.Height - 1; RowIdx >= 0; RowIdx--)
                {
                    u8* RowReadPtr = FileReadPtr + (Stride * RowIdx);
                    memcpy(PxBufferWrite, RowReadPtr, Stride);
                    PxBufferWrite += OutImage.Width;
                }
            }
        }

        for (size_t PxIdx = 0; PxIdx < OutImage.PxCount; PxIdx++)
        {
            RGBA32& PxColor = OutImage.PxBuffer[PxIdx];
            unsigned char Tmp = PxColor.R;
            PxColor.R = PxColor.B;
            PxColor.B = Tmp;
        }

        delete[] LoadedFile.Contents;
    }
}

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

    Array<PNGChunk> ImageDataChunks;
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
            NewChunk.CRC = *(u32*)(Contents + ReadIdx);
            ReadIdx += sizeof(u32);

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
                ASSERT(ImageDataChunks.Num == 0);
                bool bValidPaletteSize = InChunk.Length % 3 == 0;
                ASSERT(bValidPaletteSize);
                if (bReadHeader && ImageDataChunks.Num == 0 && PaletteChunk.Entries == nullptr && bValidPaletteSize)
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
                ASSERT(ImageDataChunks.Num == 0 || bReadingImageData);
                if (ImageDataChunks.Num == 0) { bReadingImageData = true; }
                if (bReadHeader && bReadingImageData)
                {
                    ImageDataChunks.Add(InChunk);
                }
                else
                {
                    bError = true;
                    delete[] InChunk.Data;
                }
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
        ASSERT(Contents && Size > 0 && ReadIdx == 0 && ImageDataChunks.Num == 0);
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
        bError |= ReadIdx != Size || !bReadHeader || !bReadEnd || ImageDataChunks.Num == 0;
        ASSERT(!bError);
    }
    void ConstructImage(ImageT& OutImage)
    {
        ASSERT(bReadHeader && bReadEnd && ImageDataChunks.Num > 0);
        ASSERT(OutImage.PxBuffer == nullptr);
        
        if (ImageDataChunks.Num > 0)
        {
            bool bGrayscale = HeaderChunk.ColorType == 0;
            bool bPalette = HeaderChunk.ColorType & 1;
            bool bColorUsed = HeaderChunk.ColorType & 2;
            bool bAlphaChannel = HeaderChunk.ColorType & 4;
            ASSERT(HeaderChunk.ColorType != 2 || (HeaderChunk.BitDepth == 8 || HeaderChunk.BitDepth == 16));
            ASSERT(HeaderChunk.ColorType != 3 || (HeaderChunk.BitDepth == 1 || HeaderChunk.BitDepth == 2 || HeaderChunk.BitDepth == 4 || HeaderChunk.BitDepth == 8));
            ASSERT(!bAlphaChannel || (HeaderChunk.BitDepth == 8 || HeaderChunk.BitDepth == 16));
            ASSERT(HeaderChunk.CompressionMethod == 0);
            ASSERT(HeaderChunk.FilterMethod == 0);
            ASSERT(HeaderChunk.InterlaceMethod == 0 || HeaderChunk.InterlaceMethod == 1);
            bool bAdam7Interlace = HeaderChunk.InterlaceMethod == 1;

            // TODO(CKA): Assume here that the first two bytes aren't split across two ImageDataChunks
            //      If that isn't true, then the PNG we're reading was encoded in a very dumb way
            // RFC 1950
            byte zlib_CompressionMethod = ImageDataChunks[0].Data[0] & 0x0F;
            byte zlib_CompressionInfo = (ImageDataChunks[0].Data[0] & 0xF0) >> 4;
            ASSERT(zlib_CompressionMethod == 8);
            ASSERT(zlib_CompressionInfo < 8);
            byte zlib_FlagsCheckBits = ImageDataChunks[0].Data[1] & 0x1F;
            bool zlib_bPresetDictionary = ImageDataChunks[0].Data[1] & 0x20;
            byte zlib_CompressionLevel = (ImageDataChunks[0].Data[1] & 0xC0) >> 6;
            ASSERT((ImageDataChunks[0].Data[0] * 256 + ImageDataChunks[0].Data[1]) % 31 == 0);
            ASSERT(!zlib_bPresetDictionary);
            // NOTE: zlib_CompressionLevel ==
            //      0 -> Compressor used fastest algorithm
            //      1 -> Compressor used fast algorithm
            //      2 -> Compressor used default algorithm
            //      3 -> Compressor used maximum compression, slowest algorithm

            return;
            // TODO:

            size_t ImageDataReadIdx = 2;
            Array<byte> DecompressedStream;
            // Decompress zlib data stream
            // RFC 1951
            for (int ChunkIdx = 0; ChunkIdx < ImageDataChunks.Num; ChunkIdx++)
            {
                size_t CurrChunkReadIdx = ChunkIdx == 0 ? ImageDataReadIdx : 0;
                PNGChunk& CurrChunk = ImageDataChunks[ChunkIdx];
                while (CurrChunkReadIdx < CurrChunk.Length)
                {
                    // Read compressed block
                    bool bFinal = CurrChunk.Data[CurrChunkReadIdx] & 0x01;
                    byte BlockType = (CurrChunk.Data[CurrChunkReadIdx] & 0x06) >> 1;
                    ASSERT(BlockType != 3); // Reserved / invalid BTYPE
                    switch (BlockType)
                    {
                        case 0: // No compression
                        {
                            // TODO(CKA): Actually test this code path with uncompressed PNG image data
                            byte BlockLen = CurrChunk.Data[CurrChunkReadIdx + 1];
                            byte BlockNlen = CurrChunk.Data[CurrChunkReadIdx + 2];
                            ASSERT((CurrChunkReadIdx + BlockLen + 3) <= CurrChunk.Length);
                            for (int BlockByteIdx = 0; BlockByteIdx < BlockLen; BlockByteIdx++)
                            {
                                DecompressedStream.Add(CurrChunk.Data[CurrChunkReadIdx + 3 + BlockByteIdx]);
                            }
                        } break;
                        case 1: // Compressed (w/ fixed Huffman codes)
                        {
                        } break;
                        case 2: // Compressed (w/ dynamic Huffman codes)
                        {
                        } break;
                    }
                }
            }

            // 

            OutImage.Width = HeaderChunk.Width;
            OutImage.Height = HeaderChunk.Height;
            OutImage.PxCount = OutImage.Width * OutImage.Height;
            OutImage.PxBufferSize = sizeof(RGBA32) * OutImage.PxCount;
            OutImage.PxBuffer = new RGBA32[OutImage.PxCount];

        }
    }
    void CleanUp()
    {
        if (PaletteChunk.Entries)
        {
            byte* PaletteChunkData = (byte*)PaletteChunk.Entries;
            delete[] PaletteChunkData;
        }
        for (int ChunkIdx = 0; ChunkIdx < ImageDataChunks.Num; ChunkIdx++)
        {
            PNGChunk& IDATChunk = ImageDataChunks[ChunkIdx];
            if (IDATChunk.Length > 0 && IDATChunk.Data) { delete[] IDATChunk.Data; }
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

void SafeRelease(ImageT& Image)
{
    if (Image.PxBuffer)
    {
        delete[] Image.PxBuffer;
        Image.PxBuffer = nullptr;
    }
}

}

