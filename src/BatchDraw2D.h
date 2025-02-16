#ifndef LEVIATHAN_BATCHDRAW2D_H
#define LEVIATHAN_BATCHDRAW2D_H

// Included in Graphics.h

namespace Leviathan
{
    struct RectF
    {
        float PosX = 0.0f;
        float PosY = 0.0f;
        float SizeX = 0.0f;
        float SizeY = 0.0f;
    };

    struct InstQuadColorData
    {
        RectF Rect;
        RGBA Color;
    };

    struct InstQuadTextureData
    {
        RectF PosRect;
        RectF TexRect;
    };

    struct BatchDraw2D
    {
        Array<InstQuadColorData> ColorBatchCmds;
        Array<InstQuadTextureData> TextureBatchCmds;

        void Clear();
        void AddQuad(const InstQuadColorData& InInstQuadColorData);
        void AddQuad(const RectF& InQuad, const RGBA& InColor);
        void AddBox(const RectF& InQuad, const RGBA& InColor, float LineWeight = 1.0f);
        void AddQuad(const InstQuadTextureData& InInstQuadTextureData);
        void AddQuad(const RectF& InQuad, const RectF& InTexRect);
    };
}

using Leviathan::BatchDraw2D;

#endif // LEVIATHAN_BATCHDRAW2D_H
