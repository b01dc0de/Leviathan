#include "Graphics.h" // Fonts.h included in Graphics.h
#include "Leviathan.h"

namespace Leviathan
{

void LvFont::Init(ID3D11Device* InDevice, const char* Filename)
{
    LvTex2D = LoadTextureBMP(Filename, InDevice);
    if (LvTex2D.Texture)
    {
        GlyphAspectRatio = (float)LvTex2D.Width / LvTex2D.Height;
        NumGlyphsX = 16;
        NumGlyphsY = 16;
        GlyphSize = { 1.0f / NumGlyphsX, 1.0f / NumGlyphsY };
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

void LvFont::Draw(BatchDrawCmds& Draw2D, v2f Origin, float Scale, const char* Msg, int MsgLength)
{
    v2f TextPos = Origin;
    constexpr float ScaleFactor = 100.0f;
    Scale *= ScaleFactor;
    const v2f TextSize{ GlyphSize.X * Scale, GlyphSize.Y * Scale * (1.0f / GlyphAspectRatio)};
    const float Spacing = 0.01f * Scale;
    for (int MsgCharIdx = 0; MsgCharIdx < MsgLength; MsgCharIdx++)
    {
        RectF TextRect{ TextPos.X, TextPos.Y, TextSize.X, TextSize.Y };
        Draw2D.AddTextRect(TextRect, GlyphRects[Msg[MsgCharIdx]]);
        TextPos.X += TextSize.X + Spacing;
    }
}

void LvFont::Term()
{
    SafeRelease(LvTex2D);
    delete[] GlyphRects;
}

void SafeRelease(LvFont& InFont)
{
    InFont.Term();
}

}

