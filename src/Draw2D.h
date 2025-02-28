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

    struct LineF
    {
        float BeginX = 0.0f;
        float BeginY = 0.0f;
        float EndX = 0.0f;
        float EndY = 0.0f;
    };

    struct InstRectColorData
    {
        RectF Rect;
        fColor Color;
    };

    struct InstRectTextureData
    {
        RectF PosRect;
        RectF TexRect;
    };

    struct InstRectColorRotationData
    {
        RectF Rect;
        fColor Color;
        float AngleZ;
    };

    struct InstRectTextureRotationData
    {
        RectF PosRect;
        RectF TexRect;
        float AngleZ;
    };

    struct BatchDraw2D
    {
        Array<InstRectColorData> ColorBatchCmds;
        Array<InstRectTextureData> TextureBatchCmds;
        Array<InstRectTextureData> TextBatchCmds;
        Array<InstRectColorRotationData> RotationColorBatchCmds;
        Array<InstRectTextureRotationData> RotationTextureBatchCmds;

        void Clear();
        void AddRect(const RectF& InRect, const fColor& InColor);
        void AddBox(const RectF& InRect, const fColor& InColor, float LineWeight = 1.0f);
        void AddRect(const RectF& InRect, const RectF& InTexRect);
        void AddTextRect(const RectF& InPosRect, const RectF& InTexRect);

        void AddRect(const RectF& InRect, const fColor& InColor, float AngleZ);
        void AddRect(const RectF& InPosRect, const RectF& InTexRect, float AngleZ);
        void AddLine(const LineF& InLine, const fColor& InColor);
    };
}

using Leviathan::BatchDraw2D;

#endif // LEVIATHAN_BATCHDRAW2D_H
