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
        RectF LeftSide{InRect.PosX - HalfLineWeight, InRect.PosY - HalfLineWeight, LineWeight, InRect.SizeY + HalfLineWeight};
        RectF RightSide{ InRect.PosX + InRect.SizeX - HalfLineWeight, InRect.PosY - HalfLineWeight, LineWeight, InRect.SizeY + HalfLineWeight};

        RectF TopSide{ InRect.PosX - HalfLineWeight, InRect.PosY - HalfLineWeight + InRect.SizeY, InRect.SizeX + HalfLineWeight, LineWeight};
        RectF BottomSide{ InRect.PosX - HalfLineWeight, InRect.PosY - HalfLineWeight, InRect.SizeX + HalfLineWeight, LineWeight };

        // TODO: Add this as array (will require new Array<>::Add method)
        AddRect(LeftSide, InColor);
        AddRect(TopSide, InColor);
        AddRect(RightSide, InColor);
        AddRect(BottomSide, InColor);
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