#ifndef LEVIATHAN_FONTS_H
#define LEVIATHAN_FONTS_H

#include "Graphics.h"

namespace Leviathan
{
    struct HandmadeTextSheet
    {
        LvTexture2D LvTex2D;
        int NumGlyphsX;
        int NumGlyphsY;
        v2f GlyphSize;
        RectF* GlyphRects;

        void Init(ID3D11Device* InDevice, const char* Filename, int InNumGlyphsX, int InNumGlyphsY);
        void Draw(BatchDraw2D& Draw2D, v2f Origin, float Scale, const char* Msg, int MsgLength);
        RectF GetGlyph(char GlyphChar);
        void Term();
    };
}

#endif // LEVIATHAN_FONTS_H

