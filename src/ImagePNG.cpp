#include "Leviathan.h" // Includes ImagePNG.h 

namespace Leviathan
{

// Helper functions:
u32 SwapEndiannessU32(u32 Value)
{
    u32 Result = ((Value & 0x000000FF) << 24) | ((Value & 0x0000FF00) << 8) |
        ((Value & 0x00FF0000) >> 8) | ((Value & 0xFF000000) >> 24);
    return Result;
}

int FindMaxLength(int* List, int Num)
{
    ASSERT(List && Num);
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
    ASSERT(List && Num && OutCountList)
    for (int Idx = 0; Idx < Num; Idx++) { OutCountList[List[Idx]]++; }
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
    // NOTE: This interpets the first bit as _MOST_ significant bit
    byte Read_Most(int Count)
    {
        byte Result = 0;
        ASSERT(Count <= 8);
        ASSERT(NextBit != 8);
        byte StreamByte = Stream[ByteIdx] >> NextBit;
        for (int BitIdx = 0; BitIdx < Count; BitIdx++)
        {
            Result |= (StreamByte & 1) << (Count - 1 - BitIdx);
            StreamByte = StreamByte >> 1;
            if (++NextBit == 8)
            {
                ByteIdx++;
                NextBit = 0;
                ASSERT(ByteIdx < Stream.Num);
                StreamByte = Stream[ByteIdx];
            }
        }
        return Result;
    }
    // NOTE: This interpets the first bit as _LEAST_ significant bit
    byte Read_Least(int Count)
    {
        byte Result = 0;
        ASSERT(Count <= 8);
        ASSERT(NextBit != 8);
        byte StreamByte = Stream[ByteIdx] >> NextBit;
        for (int BitIdx = 0; BitIdx < Count; BitIdx++)
        {
            Result |= (StreamByte & 1) << BitIdx;
            StreamByte = StreamByte >> 1;
            if (++NextBit == 8)
            {
                ByteIdx++;
                NextBit = 0;
                ASSERT(ByteIdx < Stream.Num);
                StreamByte = Stream[ByteIdx];
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

struct AlphabetEntry
{
    int SymbolValue;
    int Length;
    int Code;
};

static void ConstructAlphabet(int* List, int Num, AlphabetEntry* AlphabetEntries, int NumEntries)
{
    ASSERT(AlphabetEntries);
    ASSERT(Num == NumEntries);

    // RFC 1951 - 3.2.2
    int MAX_BITS = FindMaxLength(List, Num);
    int* bl_count = new int[MAX_BITS + 1];
    int* next_code = new int[MAX_BITS + 1];

    // Step 1 - Count number of codes for each code length
    ZeroMemory(bl_count, sizeof(int) * (MAX_BITS + 1));
    CountLengths(List, Num, bl_count);

    // Step 2 - Find numerical value of smallest code for each code length
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
    for (int n = 0; n < max_code; n++)
    {
        int Len = List[n];
        AlphabetEntry NewEntry{ n, Len, 0 };
        if (Len != 0)
        {
            NewEntry.Code = next_code[Len];
            next_code[Len]++;
        }
        AlphabetEntries[n] = NewEntry;
    }

    delete[] bl_count;
    delete[] next_code;
}

bool IsSortedByLength(Array<AlphabetEntry>& Entries)
{
    bool bResult = true;
    for (int Idx = 1; Idx < Entries.Num; Idx++)
    {
        if (Entries[Idx - 1].Length > Entries[Idx].Length) { bResult = false; break; }
    }
    return bResult;
}

void SortByLength(Array<AlphabetEntry>& Entries)
{
    // NOTE: Dumb bubble-sort
    while (!IsSortedByLength(Entries))
    {
        for (int Idx = 1; Idx < Entries.Num; Idx++)
        {
            if (Entries[Idx - 1].Length > Entries[Idx].Length)
            {
                AlphabetEntry Tmp = Entries[Idx - 1];
                Entries[Idx - 1] = Entries[Idx];
                Entries[Idx] = Tmp;
            }
        }
    }
}

void DebugPrint(Array<AlphabetEntry>& Entries)
{
    Outf("Symbol Length   Code\n");
    Outf("------ ------   ----\n");
    for (int Idx = 0; Idx < Entries.Num; Idx++)
    {
        Outf("%3d       %d       %4d    \n", Entries[Idx].SymbolValue, Entries[Idx].Length, Entries[Idx].Code);
    }
    Outf("\n");
}

struct CodeLengthAlphabet
{
    int MinBitLength = 0;
    int MaxBitLength = 0;
    Array<AlphabetEntry> Entries;

    void Construct(BitReader& BR, int HCLEN)
    {
        static constexpr int CLA_LUT[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
        static constexpr int CLA_Size = ARRAY_SIZE(CLA_LUT);
        ASSERT(Entries.Num == 0);
        int OrderedCodeLengths[CLA_Size] = {};
        for (int Idx = 0; Idx < CLA_Size; Idx++)
        {
            byte CodeLength = 0;
            if (Idx < HCLEN) { CodeLength = BR.Read_Least(3); }
            int AlphabetIdx = CLA_LUT[Idx];
            ASSERT(AlphabetIdx < CLA_Size);
            OrderedCodeLengths[AlphabetIdx] = CodeLength;
        }

        AlphabetEntry Alphabet[CLA_Size] = {};
        ConstructAlphabet(OrderedCodeLengths, CLA_Size, Alphabet, CLA_Size);

        for (int Idx = 0; Idx < CLA_Size; Idx++)
        {
            int Length = Alphabet[Idx].Length;
            if (Length != 0)
            {
                if (MinBitLength == 0 || Length < MinBitLength) { MinBitLength = Length; }
                if (MaxBitLength < Length) { MaxBitLength = Length; }
                Entries.Add(Alphabet[Idx]);
            }
        }
        ASSERT(MinBitLength > 0 && MaxBitLength > 0);

        SortByLength(Entries);

        bool bDebugPrint = true;
        if (bDebugPrint)
        {
            Outf("CodeLengths Alphabet:\n");
            DebugPrint(Entries);
        }
    }

    void DecodeCodeLengthSequence(BitReader& BR, int NumCodeLengths, Array<int>& CodeLengthSequence)
    {
        ASSERT(CodeLengthSequence.Num == 0);
        // NOTE(CKA): My current implementation requires Entries to be sorted by length:
        ASSERT(IsSortedByLength(Entries)); 

        int Idx = 0;
        while (Idx < NumCodeLengths)
        {
            int CurrBits = MinBitLength;
            byte NextValue = BR.Read_Most(CurrBits);
            bool bFoundMatch = false;
            for (int EntryIdx = 0; EntryIdx < Entries.Num; EntryIdx++)
            {
                AlphabetEntry& CurrEntry = Entries[EntryIdx];
                if (CurrEntry.Length != CurrBits)
                {
                    ASSERT(CurrEntry.Length > CurrBits); // BitLengths must be in ascending order
                    int NumBitsToRead = CurrEntry.Length - CurrBits;
                    // NOTE(CKA): From the spec (RFC 1951 - 3.1.1)
                    // "Huffman codes are packed starting with the most-significant bit of the code."
                    NextValue = (NextValue << NumBitsToRead) | BR.Read_Most(NumBitsToRead);
                    CurrBits += NumBitsToRead;
                }

                ASSERT(CurrEntry.Length == CurrBits);
                if (CurrEntry.Code == NextValue)
                {
                    if (0 <= CurrEntry.SymbolValue && CurrEntry.SymbolValue <= 15)
                    {
                        // Values 0 - 15 represent code lengths of 0 - 15
                        CodeLengthSequence.Add(CurrEntry.SymbolValue);
                        Idx++;
                    }
                    else if (CurrEntry.SymbolValue == 16)
                    {
                        // TODO(CKA): Test this code path
                        /*
                            NOTE: From the spec (RFC 1951 3.2.7)
                                16: Copy the previous code length 3 - 6 times.
                                The next 2 bits indicate repeat length (0 = 3, ... , 3 = 6)
                                Example:  Codes 8, 16 (+2 bits 11), 16 (+2 bits 10)
                                    will expand to 12 code lengths of 8 (1 + 6 + 5)
                        */
                        ASSERT(Idx > 0 && CodeLengthSequence.Num > 0);
                        constexpr int BaseRepeatCount = 3;
                        constexpr int NumBitsToRead = 2;
                        int RepeatCount = BaseRepeatCount + BR.Read_Least(NumBitsToRead);
                        int ValueToRepeat = CodeLengthSequence[Idx - 1];
                        ASSERT(ValueToRepeat != 0);
                        CodeLengthSequence.AddCount(ValueToRepeat, RepeatCount);
                        Idx += RepeatCount;
                    }
                    else if (CurrEntry.SymbolValue == 17)
                    {
                        // TODO(CKA): Test this code path
                        /*
                            NOTE: From the spec (RFC 1951 3.2.7)
                                17: Repeat a code length of 0 for 3 - 10 times.
                                    (3 bits of length)
                        */
                        constexpr int BaseRepeatCount = 3;
                        constexpr int NumBitsToRead = 3;
                        int RepeatCount = BaseRepeatCount + BR.Read_Least(NumBitsToRead);
                        ASSERT(3 <= RepeatCount && RepeatCount <= 10);
                        CodeLengthSequence.AddCount(0, RepeatCount);
                        Idx += RepeatCount;
                    }
                    else if (CurrEntry.SymbolValue == 18)
                    {
                        /*
                            NOTE: From the spec (RFC 1951 3.2.7)
                                18: Repeat a code length of 0 for 11 - 138 times
                                    (7 bits of length)
                        */
                        constexpr int BaseRepeatCount = 11;
                        constexpr int NumBitsToRead = 7;
                        int ReadRepeatCount = BR.Read_Least(NumBitsToRead);
                        int RepeatCount = BaseRepeatCount + ReadRepeatCount;//BR.Read_Least(NumBitsToRead);
                        ASSERT(11 <= RepeatCount && RepeatCount <= 138);
                        CodeLengthSequence.AddCount(0, RepeatCount);
                        Idx += RepeatCount;
                    }
                    else { ASSERT(false); }
                    bFoundMatch = true;
                    break;
                }
            }
            ASSERT(bFoundMatch);
        }
        ASSERT(Idx == NumCodeLengths && CodeLengthSequence.Num == NumCodeLengths);

        static bool bDebugPrint = true;
        if (bDebugPrint)
        {
            int SequenceIdx = 0;
            Outf("(Decoded) Code Length Sequence:\n");
            Outf("LENGTH\t[VALUE]\n");
            while (SequenceIdx < NumCodeLengths)
            {
                if (CodeLengthSequence[SequenceIdx] != 0)
                {
                    Outf("  %d\t  [%d]\n", CodeLengthSequence[SequenceIdx], SequenceIdx);
                    SequenceIdx++;
                }
                else
                {
                    int ZeroesCount = 1;
                    int BeginIdx = SequenceIdx;
                    SequenceIdx++;
                    while (SequenceIdx < NumCodeLengths && CodeLengthSequence[SequenceIdx] == 0) { ZeroesCount++; SequenceIdx++; }
                    if (ZeroesCount > 1)
                    {
                        Outf(" ... # of zeroes: %d ...\t[%d] - [%d]\n", ZeroesCount, BeginIdx, SequenceIdx - 1);
                    }
                    else
                    {
                        Outf("  %d\t  [%d]\n", CodeLengthSequence[SequenceIdx], SequenceIdx);
                    }
                }
            }
            Outf("\n");
        }
    }
};

struct DeflateAlphabet
{
    static constexpr int EndOfBlock = 256;
    Array<AlphabetEntry> LitEntries;
    Array<AlphabetEntry> DistEntries;

    void ConstructDefaultFixedAlphabet()
    {
        ASSERT(LitEntries.Num == 0);
        constexpr int NumSymbolValues = 288; // Symbol values [0] - [287]
        LitEntries.Reserve(EndOfBlock);
        DistEntries.Reserve(NumSymbolValues - EndOfBlock);
        for (int Idx = 0; Idx < NumSymbolValues; Idx++)
        {
            AlphabetEntry NewEntry{ Idx, 0, 0 };
            if (Idx <= 143) { NewEntry.Length = 8; NewEntry.Code = 0b110000 + Idx; }
            else if (Idx <= 255) { NewEntry.Length = 9; NewEntry.Code = 0b110010000 + (Idx - 144); }
            else if (Idx <= 279) { NewEntry.Length = 7; NewEntry.Code = 0b0000000 + (Idx - 256); }
            else if (Idx <= 287) { NewEntry.Length = 8; NewEntry.Code = 0b11000000 + (Idx - 280); }
            else { ASSERT(false); }
            LitEntries.Add(NewEntry);
        }
        SortByLength(LitEntries);
        SortByLength(DistEntries);

        bool bDebugPrint = true;
        if (bDebugPrint)
        {
            Outf("Fixed Decoder Alphabet / Literal Entries:\n");
            DebugPrint(LitEntries);

            Outf("Fixed Decoder Alphabet / Distance Entries:\n");
            DebugPrint(DistEntries);
        }
    }

    void ConstructLitDistAlphabets(int* CodeLengthSequence, int NumCodeLengths, int NumLitLengths, int NumDistLengths)
    {
        ASSERT(CodeLengthSequence);
        ASSERT(NumCodeLengths > 1);
        ASSERT(NumCodeLengths == NumLitLengths + NumDistLengths);
        ASSERT(LitEntries.Num == 0 && DistEntries.Num == 0);

        AlphabetEntry* TmpLitAlphabet = new AlphabetEntry[NumLitLengths];
        ConstructAlphabet(CodeLengthSequence, NumLitLengths, TmpLitAlphabet, NumLitLengths);

        AlphabetEntry* TmpDistAlphabet = new AlphabetEntry[NumDistLengths];
        ConstructAlphabet(CodeLengthSequence + NumLitLengths, NumDistLengths, TmpDistAlphabet, NumDistLengths);

        for (int Idx = 0; Idx < NumLitLengths; Idx++)
        {
            if (TmpLitAlphabet[Idx].Length != 0) { LitEntries.Add(TmpLitAlphabet[Idx]); }
        }

        for (int Idx = 0; Idx < NumDistLengths; Idx++)
        {
            if (TmpDistAlphabet[Idx].Length != 0)
            {
                AlphabetEntry AdjEntry{ TmpDistAlphabet[Idx] };
                AdjEntry.SymbolValue = Idx + EndOfBlock + 1;
                DistEntries.Add(AdjEntry);
            }
        }

        SortByLength(LitEntries);
        SortByLength(DistEntries);

        bool bDebugPrint = true;
        if (bDebugPrint)
        {
            Outf("Decoder Alphabet / Literal Entries:\n");
            DebugPrint(LitEntries);

            Outf("Decoder Alphabet / Distance Entries:\n");
            DebugPrint(DistEntries);
        }

        delete[] TmpLitAlphabet;
        delete[] TmpDistAlphabet;
    }

    enum struct SymbolType { Invalid, Literal, Distance };
    struct ParsedSymbol
    {
        SymbolType Type = SymbolType::Invalid;
        bool bValid = false;
        int EntryIdx = -1;
        int Value = -1;
    };
    ParsedSymbol DecodeSymbol(BitReader& BR)
    {
        size_t CachedByteIdx = BR.ByteIdx;
        size_t CachedNextBit = BR.NextBit;

        ParsedSymbol Result;
        int CurrBits = 0; int CurrValue = 0;
        bool bFoundMatch = false;
        for (int EntryIdx = 0; !bFoundMatch && EntryIdx < LitEntries.Num; EntryIdx++)
        {
            AlphabetEntry& CurrEntry = LitEntries[EntryIdx];

            if (CurrEntry.Length != CurrBits)
            {
                ASSERT(CurrEntry.Length > CurrBits); // BitLengths must be in ascending order
                int NumBitsToRead = CurrEntry.Length - CurrBits;
                CurrValue = (CurrValue << NumBitsToRead) | BR.Read_Most(NumBitsToRead);
                CurrBits += NumBitsToRead;
            }

            ASSERT(CurrEntry.Length == CurrBits);
            if (CurrEntry.Code == CurrValue)
            {
                bFoundMatch = true;
                Result = { SymbolType::Literal, true, EntryIdx, CurrEntry.SymbolValue };
            }
        }
        if (!bFoundMatch)
        {
            BR.ByteIdx = CachedByteIdx;
            BR.NextBit = CachedNextBit;
            CurrBits = 0;
            CurrValue = 0;
        }
        for (int EntryIdx = 0; !bFoundMatch && EntryIdx < DistEntries.Num; EntryIdx++)
        {
            AlphabetEntry& CurrEntry = DistEntries[EntryIdx];

            if (CurrEntry.Length != CurrBits)
            {
                ASSERT(CurrEntry.Length > CurrBits);
                int NumBitsToRead = CurrEntry.Length - CurrBits;
                CurrValue = (CurrValue << NumBitsToRead) | BR.Read_Most(NumBitsToRead);
                CurrBits += NumBitsToRead;
            }

            ASSERT(CurrEntry.Length == CurrBits);
            if (CurrEntry.Code == CurrValue)
            {
                bFoundMatch = true;
                Result = { SymbolType::Distance, true, EntryIdx, CurrEntry.SymbolValue };
            }
        }
        ASSERT(bFoundMatch);
        return Result;
    }
    int ParseLength(BitReader& BR, ParsedSymbol Symbol)
    {
        ASSERT(Symbol.Type == SymbolType::Distance);
        ASSERT(Symbol.bValid);
        ASSERT(Symbol.EntryIdx < DistEntries.Num);
        ASSERT(Symbol.Value > EndOfBlock);

        struct LengthBits { int ExtraBits; int BaseLength; };
        static constexpr LengthBits LengthSymbolLUT[] =
        {
            { 0, 3 }, /* 257 */ { 0, 4 }, /* 258 */
            { 0, 5 }, /* 259 */ { 0, 6 }, /* 260 */
            { 0, 7 }, /* 261 */ { 0, 8 }, /* 262 */
            { 0, 9 }, /* 263 */ { 0, 10 }, /* 264 */
            { 1, 11 }, /* 265 */ { 1, 13 }, /* 266 */
            { 1, 15 }, /* 267 */ { 1, 17 }, /* 268 */
            { 2, 19 }, /* 269 */ { 2, 23 }, /* 270 */
            { 2, 27 }, /* 271 */ { 2, 31 }, /* 272 */
            { 3, 35 }, /* 273 */ { 3, 43 }, /* 274 */
            { 3, 51 }, /* 275 */ { 3, 59 }, /* 276 */
            { 4, 67 }, /* 277 */ { 4, 83 }, /* 278 */
            { 4, 99 }, /* 279 */ { 4, 115 }, /* 280 */
            { 5, 131 }, /* 281 */ { 5, 163 }, /* 282 */
            { 5, 195 }, /* 283 */ { 5, 227 }, /* 284 */
            { 0, 258 } /* 285 */
        };

        int LUTIdx = Symbol.Value - 257;
        ASSERT(0 <= LUTIdx && LUTIdx < ARRAY_SIZE(LengthSymbolLUT));
        LengthBits LengthEntry = LengthSymbolLUT[LUTIdx];

        int Result = LengthEntry.BaseLength;
        ASSERT(Result > 0);
        int ExtraLength = 0;
        if (LengthEntry.ExtraBits)
        {
            ExtraLength = BR.Read_Most(LengthEntry.ExtraBits);
            //ExtraLength = BR.Read_Least(LengthEntry.ExtraBits);
            Result = Result + ExtraLength;
        }
        static bool bDebugPrint = true;
        if (bDebugPrint)
        {
            Outf("\tCalculated length for symbol %d: %d (%d + %d) - %d extra bits\n",
                Symbol.Value, Result, LengthEntry.BaseLength,
                ExtraLength, LengthEntry.ExtraBits );
        }
        return Result;
    }
    int ParseDistance(BitReader& BR, ParsedSymbol Symbol)
    {
        ASSERT(Symbol.Type == SymbolType::Distance);
        ASSERT(Symbol.bValid);
        ASSERT(Symbol.EntryIdx < DistEntries.Num);
        ASSERT(Symbol.Value > EndOfBlock);

        struct DistanceBits { int ExtraBits; int BaseDistance; };
        static constexpr DistanceBits DistanceLUT[] =
        {
            { 0, 1 }, /* 0 */ { 0, 2 }, /* 1 */
            { 0, 3 }, /* 2 */ { 0, 4 }, /* 3 */
            { 1, 5 }, /* 4 */ { 1, 7 }, /* 5 */
            { 2, 9 }, /* 6 */ { 2, 13 }, /* 7 */
            { 3, 17 }, /* 8 */ { 3, 25 }, /* 9 */
            { 4, 33}, /* 10 */ { 4, 49 }, /* 11 */
            { 5, 64 }, /* 12 */ { 5, 97 }, /* 13 */
            { 6, 129 }, /* 14 */ { 6, 193 }, /* 15 */
            { 7, 257 }, /* 16 */ { 7, 385 }, /* 17 */
            { 8, 513 }, /* 18 */ { 8, 769 }, /* 19 */
            { 9, 1025 }, /* 20 */ { 9, 1537 }, /* 21 */
            { 10, 2049 }, /* 22 */ { 10, 3073 }, /* 23 */
            { 11, 4097 }, /* 24 */ { 11, 6145 }, /* 25 */
            { 12, 8193 }, /* 26 */ { 12, 12289 }, /* 27 */
            { 13, 16385 }, /* 28 */ { 13, 24577 }, /* 29 */
        };

        int DistanceCode = BR.Read_Most(5);
        ASSERT(0 <= DistanceCode && DistanceCode < ARRAY_SIZE(DistanceLUT));
        DistanceBits DistEntry = DistanceLUT[DistanceCode];
        int Result = DistEntry.BaseDistance;
        int ExtraDistance = 0;
        if (DistEntry.ExtraBits)
        {
            ExtraDistance = BR.Read_Most(DistEntry.ExtraBits);;
            //ExtraDistance = BR.Read_Least(DistEntry.ExtraBits);;
            Result = Result + ExtraDistance;
        }
        static bool bDebugPrint = true;
        if (bDebugPrint)
        {
            Outf("\tCalculated distance for symbol %d: %d (%d + %d) - %d extra bits\n",
                DistanceCode, Result,
                DistEntry.BaseDistance, ExtraDistance,
                DistEntry.ExtraBits);
        }
        return Result;
    }
    void ParseAndCopyLengthDistance(BitReader& BR, Array<byte>& OutStream, ParsedSymbol Symbol)
    {
        ASSERT(Symbol.Type == SymbolType::Distance);
        ASSERT(Symbol.bValid);
        ASSERT(Symbol.EntryIdx < DistEntries.Num);
        ASSERT(Symbol.Value > EndOfBlock);

        int Length = ParseLength(BR, Symbol);
        int Distance = ParseDistance(BR, Symbol);

        int StartIdx = OutStream.Num - Distance;
        int EndIdx = StartIdx + Length - 1;
        ASSERT(0 <= StartIdx && StartIdx < OutStream.Num);
        int NumBytesCopied = 0;
        for (int Idx = StartIdx; Idx <= EndIdx; Idx++)
        {
            byte ValueToCopy = OutStream[Idx];
            OutStream.Add(ValueToCopy);
            NumBytesCopied++;
        }
        static bool bDebugPrint = true;
        if (bDebugPrint)
        {
            Outf("\tCopying %d bytes from [%d] to [%d]\n\t",
                NumBytesCopied, StartIdx, EndIdx);
            for (int Idx = StartIdx; Idx <= EndIdx; Idx++)
            {
                Outf("0x%02X ", OutStream[Idx]);
            }
            Outf("\n");
        }
        ASSERT(NumBytesCopied == Length);
    }
    void Decode(BitReader& BR, Array<byte>& OutStream)
    {
        bool bError = false;
        bool bEnd = false;
        Outf("Decode: BEGIN - ByteIdx: %d\tNextBit: %d\n", BR.ByteIdx, BR.NextBit);
        size_t NumLitCodes = 0;
        size_t NumDistCodes = 0;
        while (!bError && !bEnd && BR.ByteIdx < BR.Stream.Num)
        {
            int CachedByteIdx = BR.ByteIdx;
            int CachedBitIdx = BR.NextBit;
            ParsedSymbol Symbol = DecodeSymbol(BR);
            int NumBitsRead = (BR.NextBit - CachedBitIdx) + ((BR.ByteIdx - CachedByteIdx) * 8);
            ASSERT(Symbol.bValid);
            if (!Symbol.bValid) { bError = true; break; }

            Outf("Decode[%d]: Read %d bits matched code %d with %s symbol %d\n",
                OutStream.Num,
                NumBitsRead,
                Symbol.Type == SymbolType::Literal ? LitEntries[Symbol.EntryIdx].Code : DistEntries[Symbol.EntryIdx].Code,
                Symbol.Type == SymbolType::Literal ? "literal" : "distance",
                Symbol.Value);

            if (Symbol.Value < EndOfBlock)
            {
                OutStream.Add(Symbol.Value);
                NumLitCodes++;
            }
            else if (Symbol.Value == EndOfBlock)
            {
                bEnd = true;
            }
            else if (Symbol.Value > EndOfBlock)
            {
                ParseAndCopyLengthDistance(BR, OutStream, Symbol);
                NumDistCodes++;
            }
            else { ASSERT(false); bError = true; break; }
        }
        static bool bDebugPrint = true;
        if (bDebugPrint)
        {
            Outf("Decoded %d literal codes, %d distance codes (+ 1 EoB) => %d bytes\n",
                NumLitCodes, NumDistCodes, OutStream.Num);
        }
    }
};

void Decompress(Array<byte>& InStream, Array<byte>& OutStream)
{
    ASSERT(InStream.Num > 0);
    ASSERT(OutStream.Num == 0);

    BitReader BR{ InStream };

    // RFC 1950
    byte CompressionMethod = BR.Read_Least(4);
    byte CompressionInfo = BR.Read_Least(4);
    ASSERT(CompressionMethod == 8);
    ASSERT(CompressionInfo < 8);
    byte FlagsCheckBits = BR.Read_Least(5);
    bool bPresetDictionary = BR.Read_Least(1);
    byte CompressionLevel = BR.Read_Least(2);
    ASSERT((InStream.Data[0] * 256 + InStream.Data[1]) % 31 == 0);
    ASSERT(!bPresetDictionary);
    // NOTE: CompressionLevel == 0 -> fastest, 1 -> fast, 2 -> default, 3 -> maximum / slowest

    // Decompress zlib data stream
    // RFC 1951
    bool bFinal = false;
    while (!bFinal && BR.ByteIdx < InStream.Num)
    {
        // Read compressed block
        bFinal = BR.Read_Least(1);
        byte BlockType = BR.Read_Least(2);
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
            DeflateAlphabet DecodeAlphabet;
            if (BlockType == 1) // Compressed (w/ fixed Huffman codes)
            {
                DecodeAlphabet.ConstructDefaultFixedAlphabet();
            }
            else if (BlockType == 2) // Compressed (w/ dynamic Huffman codes)
            {
                constexpr int MaxCode = 285;
                int HLIT = BR.Read_Least(5) + 257;
                int HDIST = BR.Read_Least(5) + 1;
                int HCLEN = BR.Read_Least(4) + 4;

                CodeLengthAlphabet CLA;
                CLA.Construct(BR, HCLEN);

                int NumCodeLengths = HLIT + HDIST;
                Array<int> CodeLengthSequence{ NumCodeLengths };
                CLA.DecodeCodeLengthSequence(BR, NumCodeLengths, CodeLengthSequence);

                DecodeAlphabet.ConstructLitDistAlphabets(CodeLengthSequence.Data, CodeLengthSequence.Num, HLIT, HDIST);
            }

            DecodeAlphabet.Decode(BR, OutStream);
        }

        // Advance to next whole byte for CRC
        if (BR.NextBit != 0) { BR.NextBit = 0; BR.ByteIdx++; }

        ASSERT(BR.ByteIdx == (InStream.Num - 4));
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
