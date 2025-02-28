#include "Graphics.h" // BatchDraw2D.h included in Graphics.h

namespace Leviathan
{
    void BatchDraw2D::Clear()
    {
        ColorBatchCmds.Empty();
        TextureBatchCmds.Empty();
        TextBatchCmds.Empty();
        LineBatchCmds.Empty();
        RotationColorBatchCmds.Empty();
        RotationTextureBatchCmds.Empty();
    }

    void BatchDraw2D::AddRect(const RectF& InRect, const fColor& InColor)
    {
        ColorBatchCmds.Add(InstRectColorData{ InRect, InColor });
    }

    void BatchDraw2D::AddBox(const RectF& InRect, const fColor& InColor, float LineWeight)
    {
        float HalfLineWeight = LineWeight / 2.0f;
        InstRectColorData BoxRects[] = {
            { { InRect.PosX - HalfLineWeight, InRect.PosY - HalfLineWeight, LineWeight, InRect.SizeY + HalfLineWeight }, InColor }, // Left
            { { InRect.PosX + InRect.SizeX - HalfLineWeight, InRect.PosY - HalfLineWeight, LineWeight, InRect.SizeY + HalfLineWeight }, InColor }, // Right
            { { InRect.PosX - HalfLineWeight, InRect.PosY - HalfLineWeight + InRect.SizeY, InRect.SizeX + HalfLineWeight, LineWeight }, InColor }, // Top
            { { InRect.PosX - HalfLineWeight, InRect.PosY - HalfLineWeight, InRect.SizeX + HalfLineWeight, LineWeight }, InColor } // Bottom
        };

        ColorBatchCmds.Add(BoxRects, ARRAY_SIZE(BoxRects));
    }

    void BatchDraw2D::AddRect(const RectF& InRect, const RectF& InTexRect)
    {
        TextureBatchCmds.Add(InstRectTextureData{ InRect, InTexRect });
    }

    void BatchDraw2D::AddTextRect(const RectF& InPosRect, const RectF& InTexRect)
    {
        TextBatchCmds.Add(InstRectTextureData{ InPosRect, InTexRect });
    }

    void BatchDraw2D::AddLine(const LineF& InLine, const fColor& InColor)
    {
        LineBatchCmds.Add(InstLineData{ InLine, InColor });
    }

    void BatchDraw2D::AddRect(const RectF& InRect, const fColor& InColor, float AngleZ)
    {
        RotationColorBatchCmds.Add(InstRectColorRotationData{ InRect, InColor, AngleZ });
    }

    void BatchDraw2D::AddRect(const RectF& InPosRect, const RectF& InTexRect, float AngleZ)
    {
        RotationTextureBatchCmds.Add(InstRectTextureRotationData{ InPosRect, InTexRect, AngleZ });
    }
}