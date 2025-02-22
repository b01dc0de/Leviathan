#include "Fonts.h"
#include "Leviathan.h"

namespace Leviathan
{
    namespace OpenTypeFont
    {
        const char* RequiredTableTags[] =
        {
            "cmap", // character to glyph mapping
            "glyf", // glyph data
            "head", // font header
            "hhea", // horizontal header
            "hmtx", // horizontal metrics
            "loca", // index to location
            "maxp", // maximum profile
            "name", // naming
            "post" // PostScript
        };
        const char* OptionalTableTags[] =
        {
            "cvt ", // control value
            "fpgm",	// font program
            "hdmx",	// horizontal device metrics
            "kern",	// kerning
            "OS/2",	// OS/2
            "prep",	// control value program
        };

        const char* OpenTypeRequired[] =
        {
            "cmap", // Character to glyph mapping
            "head", // Font header
            "hhea", // Horizontal header
            "hmtx", // Horizontal metrics
            "maxp", // Maximum profile
            "name", // Naming table
            "OS/2", // OS/2 and Windows specific metrics
            "post"  // PostScript information
        };
        const char* TrueTypeOutlinesTables[] =
        {
            "cvt ", // Control Value Table(optional table)
            "fpgm", // Font program(optional table)
            "glyf", // Glyph data
            "loca", // Index to location
            "prep", // Control Value Program(optional table)
            "gasp", // Grid - fitting / Scan - conversion(optional table)
        };
        const char* CFFOutlinesTables[] = {
            "CFF ", // Compact Font Format 1.0
            "CFF2", // Compact Font Format 2.0
            "VORG" // Vertical Origin(optional table)
        };

        const char* OpenTypeCore[] =
        {
            "GSUB", // Glyph Substitution
            "GPOS", // Glyph Positioning
            "BASE", // Baseline
            "JSTF", // Justification
            "GDEF", // Glyph Definition
        };
        const char* ReadTags[] =
        {
            "GDEF", // GDEF    Glyph Definition Table    https://learn.microsoft.com/en-us/typography/opentype/spec/gdef
            "GPOS", // GPOS    Glyph Positioning Table    https://learn.microsoft.com/en-us/typography/opentype/spec/gpos
            "GSUB", // GSUB    Glyph Substitution Table    https://learn.microsoft.com/en-us/typography/opentype/spec/gsub
            "OS/2", // OS/2    OS/2 and Windows Metrics Table    https://learn.microsoft.com/en-us/typography/opentype/spec/os2
            "cmap", // cmap    Character to Glyph Index Mapping Table    https://learn.microsoft.com/en-us/typography/opentype/spec/cmap
            "cvt ", // cvt     Control Value Table    https://learn.microsoft.com/en-us/typography/opentype/spec/cvt
            "fpgm", // fpgm    Font Program    https://learn.microsoft.com/en-us/typography/opentype/spec/fpgm
            "gasp", // gasp    Grid-fitting and Scan-conversion Procedure Table    https://learn.microsoft.com/en-us/typography/opentype/spec/gasp 
            "glyf", // glyf    Glyph Data    https://learn.microsoft.com/en-us/typography/opentype/spec/glyf
            "head", // head    Font Header Table    https://learn.microsoft.com/en-us/typography/opentype/spec/head
            "hhea", // hhea    Horizontal Header Table    https://learn.microsoft.com/en-us/typography/opentype/spec/hhea
            "hmtx" // hmtx    Horizontal Metrics Table    https://learn.microsoft.com/en-us/typography/opentype/spec/hmtx
            "loca", // loca    Index to Location    https://learn.microsoft.com/en-us/typography/opentype/spec/loca
            "maxp", // maxp    Maximum Profile    https://learn.microsoft.com/en-us/typography/opentype/spec/maxp
            "name", // name    Naming Table    https://learn.microsoft.com/en-us/typography/opentype/spec/name
            "post", // post    PostScript Table    https://learn.microsoft.com/en-us/typography/opentype/spec/post
            "prep", // prep    Control Value Program    https://learn.microsoft.com/en-us/typography/opentype/spec/prep
        };
        u32 TagToU32(const char* Tag)
        {
            return *(u32*)Tag;
        }

        // NOTE: From https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6.html
        u32 CalcTableChecksum(u32 *Table, u32 numberOfBytesInTable)
        {
            u32 CheckSum = 0;
            u32 NumLongs = (numberOfBytesInTable + 3) / 4;
            while (NumLongs-- > 0) { CheckSum += *Table++; }
            return CheckSum;
        }

    #pragma pack(push, 1)
        union TagT
        {
            struct CharsT
            {
                u8 _0;
                u8 _1;
                u8 _2;
                u8 _3;
            } Chars;
            u32 Int;
        };
        struct TableDirectoryT
        {
            u32 SfntVersion; //	0x00010000 or 0x4F54544F ('OTTO') — see below.
            u16 NumTables; // Number of tables.
            u16 SearchRange; // Maximum power of 2 less than or equal to numTables, times 16 ((2**floor(log2(numTables))) * 16, where “**” is an exponentiation operator).
            u16 EntrySelector; // Log2 of the maximum power of 2 less than or equal to numTables(log2(searchRange / 16), which is equal to floor(log2(numTables))).
            u16 RangeShift; //numTables times 16, minus searchRange((numTables * 16) - searchRange).
            // TableRecordT TableRecords[NumTables];
        };
        struct TableRecordT
        {
            TagT TableTag; // Table identifier.
            u32 Checksum; // Checksum for this table.
            u32 Offset; // Offset from beginning of font file.
            u32 Length; // Length of this table.
        };
    #pragma pack(pop)
    }

    LvFont LoadOpenTypeFont_WIP()
    {
        using namespace OpenTypeFont;
        const char* JetBrainsMonoFontFilename = "Assets/Fonts/JetBrainsMono-Regular.ttf";

        LvFont Result = {};

        FileContentsT OpenTypeFile = LoadFileContents(JetBrainsMonoFontFilename);
        if (!OpenTypeFile.Contents) { return Result; }

        TableDirectoryT TableDirectory;
        TableRecordT* TableRecords = nullptr;

        u8* TrueTypeReadPtr = OpenTypeFile.Contents;
        (void)OpenTypeFile.Read(TrueTypeReadPtr, sizeof(TableDirectoryT), &TableDirectory);
        ASSERT(TableDirectory.NumTables > 0);
        TableRecords = new TableRecordT[TableDirectory.NumTables];
        int ForceNumTables = 17; // TableDirectory.NumTables
        (void)OpenTypeFile.Read(TrueTypeReadPtr, sizeof(TableRecordT) * TableDirectory.NumTables, TableRecords);

        for (int TableIdx = 0; TableIdx < ForceNumTables; TableIdx++)
        {
            TagT& CurrTag = TableRecords[TableIdx].TableTag;
            char TagString[] =
            {
                CurrTag.Chars._0, CurrTag.Chars._1,
                CurrTag.Chars._2, CurrTag.Chars._3,
                '\n', '\0'
            };
            OutputDebugStringA(TagString);
        }

        return Result;
    }
}