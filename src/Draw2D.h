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

    struct InstLineData
    {
        LineF Line;
        fColor Color;
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
        Array<InstLineData> LineBatchCmds;
        Array<InstRectColorRotationData> RotationColorBatchCmds;
        Array<InstRectTextureRotationData> RotationTextureBatchCmds;

        void Clear();
        void AddRect(const RectF& InRect, const fColor& InColor);
        void AddBox(const RectF& InRect, const fColor& InColor, float LineWeight = 1.0f);
        void AddRect(const RectF& InRect, const RectF& InTexRect);
        void AddTextRect(const RectF& InPosRect, const RectF& InTexRect);
        void AddLine(const LineF& InLine, const fColor& InColor);

        void AddRect(const RectF& InRect, const fColor& InColor, float AngleZ);
        void AddRect(const RectF& InPosRect, const RectF& InTexRect, float AngleZ);
    };
}

using Leviathan::BatchDraw2D;

#endif // LEVIATHAN_BATCHDRAW2D_H
