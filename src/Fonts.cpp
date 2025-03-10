#include "Fonts.h"
#include "Leviathan.h"

namespace Leviathan
{
    void HandmadeTextSheet::Init(ID3D11Device* InDevice, const char* Filename, int InNumGlyphsX, int InNumGlyphsY)
    {
        *this = {};
        NumGlyphsX = InNumGlyphsX;
        NumGlyphsY = InNumGlyphsY;

        LvTex2D = LoadTextureBMP(Filename, InDevice);
        if (LvTex2D.Texture)
        {
            GlyphSize = {1.0f / NumGlyphsX, 1.0f / NumGlyphsY};
            GlyphRects = new RectF[NumGlyphsX * NumGlyphsY];
            int GlyphIdx = 0;
            for (int RowIdx = 0; RowIdx < NumGlyphsY; RowIdx++)
            {
                for (int ColIdx = 0; ColIdx < NumGlyphsX; ColIdx++, GlyphIdx++)
                {
                    GlyphRects[GlyphIdx] = RectF
                    {
                        GlyphSize.X * ColIdx, GlyphSize.Y * RowIdx,
                        GlyphSize.X, GlyphSize.Y
                    };
                }
            }
        }
    }

    void HandmadeTextSheet::Draw(BatchDraw2D& Draw2D, v2f Origin, float Scale, const char* Msg, int MsgLength)
    {
        v2f TextPos = Origin;
        const v2f TextSize{ GlyphSize.X * Scale, GlyphSize.Y * Scale };
        const float Spacing = 0.01f * Scale;
        for (int MsgCharIdx = 0; MsgCharIdx < MsgLength; MsgCharIdx++)
        {
            RectF TextRect{ TextPos.X, TextPos.Y, TextSize.X, TextSize.Y };
            Draw2D.AddTextRect(TextRect, GetGlyph(Msg[MsgCharIdx]));
            TextPos.X += TextSize.X + Spacing;
        }
    }

    RectF HandmadeTextSheet::GetGlyph(char GlyphChar)
    {
        constexpr RectF InvalidRect{ 1.0f, 1.0f, 0.0f, 0.0f };
        RectF Result = InvalidRect;
        if (!GlyphRects) { return Result; }
        switch (GlyphChar)
        {
            case 'A':
            case 'a': { Result = GlyphRects[0]; } break;
            case 'B':
            case 'b': { Result = GlyphRects[1]; } break;
            case 'C':
            case 'c': { Result = GlyphRects[2]; } break;
            case 'D':
            case 'd': { Result = GlyphRects[3]; } break;
            case 'E':
            case 'e': { Result = GlyphRects[4]; } break;
            case 'F':
            case 'f': { Result = GlyphRects[5]; } break;
            case 'G':
            case 'g': { Result = GlyphRects[6]; } break;
            case 'H':
            case 'h': { Result = GlyphRects[7]; } break;
            case 'I':
            case 'i': { Result = GlyphRects[8]; } break;
            case 'J':
            case 'j': { Result = GlyphRects[9]; } break;
            case 'K':
            case 'k': { Result = GlyphRects[10]; } break;
            case 'L':
            case 'l': { Result = GlyphRects[11]; } break;
            case 'M':
            case 'm': { Result = GlyphRects[12]; } break;
            case 'N':
            case 'n': { Result = GlyphRects[13]; } break;
            case 'O':
            case 'o': { Result = GlyphRects[14]; } break;
            case 'P':
            case 'p': { Result = GlyphRects[15]; } break;
            case 'Q':
            case 'q': { Result = GlyphRects[16]; } break;
            case 'R':
            case 'r': { Result = GlyphRects[17]; } break;
            case 'S':
            case 's': { Result = GlyphRects[18]; } break;
            case 'T':
            case 't': { Result = GlyphRects[19]; } break;
            case 'U':
            case 'u': { Result = GlyphRects[20]; } break;
            case 'V':
            case 'v': { Result = GlyphRects[21]; } break;
            case 'W':
            case 'w': { Result = GlyphRects[22]; } break;
            case 'X':
            case 'x': { Result = GlyphRects[23]; } break;
            case 'Y':
            case 'y': { Result = GlyphRects[24]; } break;
            case 'Z':
            case 'z': { Result = GlyphRects[25]; } break;
            case '0': { Result = GlyphRects[26]; } break;
            case '1': { Result = GlyphRects[27]; } break;
            case '2': { Result = GlyphRects[28]; } break;
            case '3': { Result = GlyphRects[29]; } break;
            case '4': { Result = GlyphRects[30]; } break;
            case '5': { Result = GlyphRects[31]; } break;
            case '6': { Result = GlyphRects[32]; } break;
            case '7': { Result = GlyphRects[33]; } break;
            case '8': { Result = GlyphRects[34]; } break;
            case '9': { Result = GlyphRects[35]; } break;
            case ' ': { Result = GlyphRects[36]; } break;
            default: { } break;
        }
        return Result;
    }

    void HandmadeTextSheet::Term()
    {
        SafeRelease(LvTex2D);
        delete[] GlyphRects;
        *this = {};
    }
}

