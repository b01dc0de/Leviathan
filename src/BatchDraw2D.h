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

    struct InstRectColorData
    {
        RectF Rect;
        RGBA Color;
    };

    struct InstRectTextureData
    {
        RectF PosRect;
        RectF TexRect;
    };

    struct BatchDraw2D
    {
        Array<InstRectColorData> ColorBatchCmds;
        Array<InstRectTextureData> TextureBatchCmds;
        Array<InstRectTextureData> TextBatchCmds;

        void Clear();
        void AddRect(const InstRectColorData& InInstRectColorData);
        void AddRect(const RectF& InRect, const RGBA& InColor);
        void AddBox(const RectF& InRect, const RGBA& InColor, float LineWeight = 1.0f);
        void AddRect(const InstRectTextureData& InInstRectTextureData);
        void AddRect(const RectF& InRect, const RectF& InTexRect);
        void AddTextRect(const RectF& InPosRect, const RectF& InTexRect);
    };
}

using Leviathan::BatchDraw2D;

#endif // LEVIATHAN_BATCHDRAW2D_H
