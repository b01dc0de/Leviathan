#ifndef LEVIATHAN_FONTS_H
#define LEVIATHAN_FONTS_H

#include "Leviathan.h"

namespace Leviathan
{

struct LvFont
{
    LvTexture2D LvTex2D;
    float GlyphAspectRatio;
    int NumGlyphsX;
    int NumGlyphsY;
    v2f GlyphSize;
    RectF* GlyphRects;

    void Init(ID3D11Device* InDevice, const char* Filename);
    void Draw(BatchDrawCmds& Draw2D, v2f Origin, float Scale, const char* Msg, int MsgLength);
    void Term();
};

void SafeRelease(LvFont& InFont);

} // namespace Leviathan

#endif // LEVIATHAN_FONTS_H

