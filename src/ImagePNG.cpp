#include "Leviathan.h" // Includes ImagePNG.h 

namespace Leviathan
{

u32 SwapEndiannessU32(u32 Value)
{
    u32 Result = ((Value & 0x000000FF) << 24) | ((Value & 0x0000FF00) << 8) |
        ((Value & 0x00FF0000) >> 8) | ((Value & 0xFF000000) >> 24);
    return Result;
}

namespace Zlib
{

struct BitReader
{
    size_t ByteIdx;
    size_t NextBit;

    Array<byte>& Stream;

    BitReader(Array<byte>& InStream)
        : Stream(InStream)
    {
        ByteIdx = 0;
        NextBit = 0;
    }
    byte Read(int Count)
    {
        byte Result = 0;
        ASSERT(Count <= 8);
        ASSERT(NextBit != 8);
        for (int BitIdx = 0; BitIdx < Count; BitIdx++)
        {
            byte StreamByte = Stream[ByteIdx];
            Result |= ((StreamByte & (1 << NextBit)) >> (NextBit)) << BitIdx;
            if (++NextBit == 8)
            {
                ByteIdx++;
                NextBit = 0;
                ASSERT(ByteIdx < Stream.Num);
            }
        }
        return Result;
    }
    void AdvanceBytes(int Count)
    {
        ByteIdx += Count;
        ASSERT(ByteIdx <= Stream.Num);
    }
};

struct HAEntry
{
    int Idx;
    int DebugSymbol;
    int Length;
    int Code;
};

struct HuffmanLiteral
{
    int Length;
    int Code;
};

struct HuffmanDist
{
    int Length;
    int Code;
    int Distance;
};

void ConstructAlphabet(int* List, int Num, HAEntry** OutEntries);
void DebugPrintAlphabet(HAEntry* Alphabet, int Num);

struct CodeLengthAlphabet
{
    static constexpr int CodeLengthLUT[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

    Array<HuffmanLiteral> LitCodes;
    Array<HuffmanDist> DistCodes;
    void Construct(int* CodeLengths, int Num)
    {
        ASSERT(Num == ARRAY_SIZE(CodeLengthLUT));
        ASSERT(LitCodes.Num == 0 && DistCodes.Num == 0);

        static constexpr int NumCodeLengths = 16;
        Outf("LitAlphabet:\n");
        HAEntry* LitAlphabet = nullptr;
        ConstructAlphabet(CodeLengths, NumCodeLengths, &LitAlphabet);
        DebugPrintAlphabet(LitAlphabet, NumCodeLengths);

        int NumUsedDistCodes = 0;
        for (int Idx = NumCodeLengths; Idx < ARRAY_SIZE(CodeLengthLUT); Idx++)
        {
            if (CodeLengths[Idx] != 0) { NumUsedDistCodes++; }
        }

        if (NumUsedDistCodes == 0)
        {
        }
        else if (NumUsedDistCodes == 1)
        {
        }
    }
};

struct HuffmanAlphabet
{
    static constexpr int Num = 288;
    HAEntry Entries[Num];
};
void ConstructFixedAlphabet(HuffmanAlphabet* Out)
{
    for (int LitValue = 0; LitValue <= 287; LitValue++)
    {
        if (LitValue < 144) { Out->Entries[LitValue] = HAEntry{ LitValue, 0, 8, 0b00110000 + LitValue }; }
        else if (LitValue < 256) { Out->Entries[LitValue] = HAEntry{ LitValue, 0, 9, 0b110010000 + (LitValue - 144) }; }
        else if (LitValue < 280) { Out->Entries[LitValue] = HAEntry{ LitValue, 0, 7, 0b0000000 + (LitValue - 256) }; }
        else { Out->Entries[LitValue] = HAEntry{ LitValue, 0, 8, 0b11000000 + (LitValue - 280) }; }
    }
}

int FindMaxLength(int* List, int Num)
{
    int Result = 0;
    for (int Idx = 0; Idx < Num; Idx++)
    {
        if (Result < List[Idx])
        {
            Result = List[Idx];
        }
    }
    return Result;
}

void CountLengths(int* List, int Num, int* OutCountList)
{
    for (int Idx = 0; Idx < Num; Idx++) { OutCountList[List[Idx]]++; }
}

bool IsSorted(Array<HAEntry>& InAlphabet)
{
    bool bResult = true;
    for (int Idx = 1; Idx < InAlphabet.Num; Idx++)
    {
        if (InAlphabet[Idx - 1].Length > InAlphabet[Idx].Length) { bResult = false; break; }
    }
    return bResult;
}

void SortAlphabet(Array<HAEntry>& InAlphabet)
{
    while (!IsSorted(InAlphabet))
    {
        for (int Idx = 1; Idx < InAlphabet.Num; Idx++)
        {
            if (InAlphabet[Idx - 1].Length > InAlphabet[Idx].Length)
            {
                HAEntry Tmp = InAlphabet[Idx - 1];
                InAlphabet[Idx - 1] = InAlphabet[Idx];
                InAlphabet[Idx] = Tmp;
            }
        }
    }
};

void ConstructAlphabet(int* List, int Num, HAEntry** OutEntries)
{
    // RFC 1951 - 3.2.2
    int MAX_BITS = FindMaxLength(List, Num);

    // Step 1 - Count number of codes for each code length
    int* bl_count = new int[MAX_BITS + 1];
    ZeroMemory(bl_count, sizeof(int) * (MAX_BITS + 1));
    CountLengths(List, Num, bl_count);

    // Step 2 - Find numerical value of smallest code for each code length
    int* next_code = new int[MAX_BITS + 1];
    int code = 0;
    bl_count[0] = 0;
    next_code[0] = 0;
    for (int bits = 1; bits <= MAX_BITS; bits++)
    {
        code = (code + bl_count[bits - 1]) << 1;
        next_code[bits] = code;
    }

    // Step 3 - Assign numerical vlaues to all codes
    int max_code = Num;
    HAEntry* Alphabet = new HAEntry[max_code];
    char NextSymbol = 'A';
    for (int n = 0; n < max_code; n++)
    {
        int Len = List[n];
        if (Len != 0)
        {
            Alphabet[n] = HAEntry{ n, NextSymbol++, Len, next_code[Len] };
            next_code[Len]++;
        }
        else
        {
            Alphabet[n] = HAEntry{ n, };
        }
    }

    ASSERT(OutEntries);
    *OutEntries = Alphabet;

    delete[] bl_count;
    delete[] next_code;
}

void DebugPrintAlphabet(HAEntry* Alphabet, int Num)
{
    Outf("Symbol Length   Code\n");
    Outf("------ ------   ----\n");
    for (int Idx = 0; Idx < Num; Idx++)
    {
        Outf("%c       %d       %4d    [%d]\n", Alphabet[Idx].DebugSymbol, Alphabet[Idx].Length, Alphabet[Idx].Code, Alphabet[Idx].Idx);
    }
    Outf("\n");
}

void Decompress(Array<byte>& InStream, Array<byte>& OutStream)
{
    ASSERT(InStream.Num > 0);
    ASSERT(OutStream.Num == 0);

    BitReader BR{ InStream };

    // RFC 1950
    byte CompressionMethod = BR.Read(4);
    byte CompressionInfo = BR.Read(4);
    ASSERT(CompressionMethod == 8);
    ASSERT(CompressionInfo < 8);
    byte FlagsCheckBits = BR.Read(5);
    bool bPresetDictionary = BR.Read(1);
    byte CompressionLevel = BR.Read(2);
    ASSERT((InStream.Data[0] * 256 + InStream.Data[1]) % 31 == 0);
    ASSERT(!bPresetDictionary);
    // NOTE: CompressionLevel == 0 -> fastest, 1 -> fast, 2 -> default, 3 -> maximum / slowest

    // Decompress zlib data stream
    // RFC 1951
    bool bFinal = false;
    while (!bFinal && BR.ByteIdx < InStream.Num)
    {
        // Read compressed block
        bFinal = BR.Read(1);
        byte BlockType = BR.Read(2);
        ASSERT(BlockType != 3); // Reserved / invalid BTYPE
        if (BlockType == 0) // No compression
        {
            u16 LEN = *(u16*)(InStream.Data + BR.ByteIdx + 1);
            u16 NLEN = *(u16*)(InStream.Data + BR.ByteIdx + 3);
            OutStream.Add(InStream.Data + BR.ByteIdx + 5, LEN);
            BR.AdvanceBytes(5 + LEN);
        }
        else
        {
            HuffmanAlphabet A;
            if (BlockType == 1) // Compressed (w/ fixed Huffman codes)
            {
                ConstructFixedAlphabet(&A);
            }
            else if (BlockType == 2) // Compressed (w/ dynamic Huffman codes)
            {
                int HLIT = BR.Read(5) + 257;
                int HDIST = BR.Read(5) + 1;
                int HCLEN = BR.Read(4) + 4;

                //static constexpr byte CodeLengthLUT[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
                //byte OrderedCodeLengths[ARRAY_SIZE(CodeLengthLUT)];
                static constexpr int CodeLengthLUT[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
                int OrderedCodeLengths[ARRAY_SIZE(CodeLengthLUT)];
                for (int Idx = 0; Idx < ARRAY_SIZE(CodeLengthLUT); Idx++)
                {
                    byte CodeLength = 0;
                    if (Idx < HCLEN) { CodeLength = BR.Read(3); }

                    int AlphabetIdx = CodeLengthLUT[Idx];
                    ASSERT(AlphabetIdx < ARRAY_SIZE(CodeLengthLUT));
                    OrderedCodeLengths[AlphabetIdx] = CodeLength;
                }

                HAEntry* Alphabet = nullptr;
                //ConstructAlphabet(OrderedCodeLengths, 16, &Alphabet);
                ConstructAlphabet(OrderedCodeLengths, ARRAY_SIZE(CodeLengthLUT), &Alphabet);
                Array<HAEntry> CodeLength_LitAlphabet;
                int MaxBits = 0;
                int MinBits = 0;
                //for (int Idx = 0; Idx < 16; Idx++)
                for (int Idx = 0; Idx < ARRAY_SIZE(CodeLengthLUT); Idx++)
                {
                    int Length = Alphabet[Idx].Length;
                    if (Length != 0)
                    {
                        if (MinBits == 0 || Length < MinBits) { MinBits = Length; }
                        if (MaxBits < Length) { MaxBits = Length; }
                        CodeLength_LitAlphabet.Add(Alphabet[Idx]);
                    }
                }
                SortAlphabet(CodeLength_LitAlphabet);
                DebugPrintAlphabet(CodeLength_LitAlphabet.Data, CodeLength_LitAlphabet.Num);

                struct OutputSymbol
                {
                    int Symbol;
                    int CodeLength;
                };
                auto DebugPrintParsedCodeLengths = [](OutputSymbol* Symbols, int Num)
                {
                    int Idx = 0;
                    while (Idx < Num)
                    {
                        if (Symbols[Idx].CodeLength != 0)
                        {
                            Outf("\t%d\tDebug: %c\n", Symbols[Idx].CodeLength, (char)Symbols[Idx].Symbol);
                            Idx++;
                        }
                        else
                        {
                            int NumZeroes = 1;
                            Idx++;
                            while (Idx < Num && Symbols[Idx].CodeLength == 0) { NumZeroes++; Idx++; }
                            Outf(" ... # of zeroes: %d ...\n", NumZeroes);
                        }
                    }
                };
                int TotalNumCodes = HLIT + HDIST;
                OutputSymbol* Symbols = new OutputSymbol[TotalNumCodes];
                int Idx = 0;
                while (Idx < TotalNumCodes)
                {
                    int CurrBits = MinBits;
                    byte NextValue = BR.Read(CurrBits);
                    bool bFoundMatch = false;
                    for (int EntryIdx = 0; EntryIdx < CodeLength_LitAlphabet.Num; EntryIdx++)
                    {
                        HAEntry& CurrEntry = CodeLength_LitAlphabet[EntryIdx];
                        if (CurrEntry.Length != CurrBits)
                        {
                            ASSERT(CurrEntry.Length > CurrBits);
                            int NumBitsToRead = CurrEntry.Length - CurrBits;
                            NextValue = (NextValue << NumBitsToRead) | BR.Read(NumBitsToRead);
                            CurrBits += NumBitsToRead;
                        }

                        ASSERT(CurrEntry.Length == CurrBits);
                        if (CurrEntry.Code == NextValue)
                        {
                            bFoundMatch = true;
                            switch (CurrEntry.Idx)
                            {
                                case 16:
                                {
                                    ASSERT(Idx > 0);
                                    int Length = BR.Read(2);
                                    int RepeatCount = 3 + Length;
                                    int SymbolToRepeat = Symbols[Idx - 1].Symbol;
                                    int ValueToRepeat = Symbols[Idx - 1].CodeLength;
                                    ASSERT(ValueToRepeat != 0);
                                    for (int RepeatIdx = 0; RepeatIdx < RepeatCount; RepeatIdx++)
                                    {
                                        Symbols[Idx].Symbol = SymbolToRepeat;
                                        Symbols[Idx].CodeLength = ValueToRepeat;
                                        Idx++;
                                    }
                                } break;
                                case 17:
                                {
                                    int Length = BR.Read(3);
                                    int RepeatCount = 3 + Length;
                                    ASSERT(3 <= RepeatCount && RepeatCount <= 10);
                                    for (int RepeatIdx = 0; RepeatIdx < RepeatCount; RepeatIdx++)
                                    {
                                        Symbols[Idx].Symbol = '0';
                                        Symbols[Idx].CodeLength = 0;
                                        Idx++;
                                    }
                                } break;
                                case 18:
                                {
                                    int Length = BR.Read(7);
                                    int RepeatCount = 11 + Length;
                                    ASSERT(11 <= RepeatCount && RepeatCount <= 138);
                                    for (int RepeatIdx = 0; RepeatIdx < RepeatCount; RepeatIdx++)
                                    {
                                        Symbols[Idx].Symbol = '0';
                                        Symbols[Idx].CodeLength = 0;
                                        Idx++;
                                    }
                                } break;
                                default:
                                {
                                    Symbols[Idx].Symbol = CurrEntry.DebugSymbol;
                                    Symbols[Idx].CodeLength = EntryIdx;
                                    Idx++;
                                } break;
                            }
                        }
                    }
                    ASSERT(bFoundMatch);
                }

                DebugPrintParsedCodeLengths(Symbols, TotalNumCodes);
                DebugBreak();
            }


            bool bRunTest = true;
            if (bRunTest)
            {
                int BitLengths[] = { 3, 3, 3, 3, 3, 2, 4, 4 };

                HAEntry* Alphabet = nullptr;
                ConstructAlphabet(BitLengths, ARRAY_SIZE(BitLengths), &Alphabet);
                DebugPrintAlphabet(Alphabet, ARRAY_SIZE(BitLengths));

                DebugBreak();
                delete[] Alphabet;
            }
        }

        ASSERT(BR.ByteIdx <= (InStream.Num - 4));
        u32 CRC = SwapEndiannessU32(*(u32*)(InStream.Data + BR.ByteIdx));
        BR.AdvanceBytes(4);
        if (bFinal) { ASSERT(BR.ByteIdx == InStream.Num); }
    }
}

}

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
    void ReconstructImageData(Array<byte>& DecompressedStream, Array<byte>& ReconStream)
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
                    bool bLeftColumnPx = (StreamIdx % BytesPerScanline) < (SamplesPerPixel + 1);
                    bool bTopRowPx = (StreamIdx / BytesPerScanline) == 0;
                    int CurrRow = (StreamIdx / BytesPerScanline);
                    byte A = bLeftColumnPx ? 0 : ReconStream[StreamIdx - SamplesPerPixel - CurrRow - 1];
                    byte B = bTopRowPx ? 0 : ReconStream[StreamIdx - BytesPerReconScanline - CurrRow - 1];
                    byte C = (bLeftColumnPx || bTopRowPx) ? 0 : ReconStream[StreamIdx - BytesPerReconScanline - SamplesPerPixel - CurrRow - 1];

                    switch (ActiveFilterMethod)
                    {
                        case 1: { X = X + A; } break; // Sub
                        case 2: { X = X + B; } break; // Up
                        case 3: { X = X + ((A + B) / 2); } break; // Average
                        case 4: { X = X + PaethPredictor(A, B, C); } break; // Paeth
                        default: { ASSERT(false); } break;
                    }
                }
                ReconStream.Add(X);
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
            Zlib::Decompress(ImageData, DecompressedStream);
            ASSERT(DecompressedStream.Num > 0);

            OutImage.Width = Header.Width;
            OutImage.Height = Header.Height;
            OutImage.PxCount = OutImage.Width * OutImage.Height;
            OutImage.PxBufferSize = sizeof(RGBA32) * OutImage.PxCount;
            OutImage.PxBuffer = new RGBA32[OutImage.PxCount];

            Array<byte> ReconStream;
            ReconstructImageData(DecompressedStream, ReconStream);

            ASSERT(ReconStream.Num > 0);
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
                        if (Header.BitDepth == 8)
                        {
                            OutImage.PxBuffer[PxIdx] =
                            {
                                ReconStream[StreamIdx + 0],
                                ReconStream[StreamIdx + 1],
                                ReconStream[StreamIdx + 2],
                                255
                            };
                            StreamIdx += 3;
                        }
                        else if (Header.BitDepth == 16)
                        {
                            // TODO(CKA): Test this
                            // NOTE(CKA): We're losing some color resolution here
                            byte R = ReconStream[StreamIdx];
                            byte G = ReconStream[StreamIdx + 2];
                            byte B = ReconStream[StreamIdx + 4];
                            OutImage.PxBuffer[PxIdx] = { R, G, B, 255 };
                            StreamIdx += 6;
                        }
                        else { ASSERT(false); }
                    }
                    ASSERT(StreamIdx == ReconStream.Num);
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
