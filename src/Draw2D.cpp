#include "Graphics.h" // BatchDraw2D.h included in Graphics.h

namespace Leviathan
{
    void BatchDraw2D::Clear()
    {
        ColorBatchCmds.Empty();
        TextureBatchCmds.Empty();
        TextBatchCmds.Empty();
        LineBatchCmds.Empty();
    }

    void BatchDraw2D::AddRect(const InstRectColorData& InInstRectColorData)
    {
        ColorBatchCmds.Add(InInstRectColorData);
    }

    void BatchDraw2D::AddRect(const RectF& InRect, const RGBA& InColor)
    {
        AddRect(InstRectColorData{ InRect, InColor });
    }

    void BatchDraw2D::AddBox(const RectF& InRect, const RGBA& InColor, float LineWeight)
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

    void BatchDraw2D::AddRect(const InstRectTextureData& InInstRectTextureData)
    {
        TextureBatchCmds.Add(InInstRectTextureData);
    }

    void BatchDraw2D::AddRect(const RectF& InRect, const RectF& InTexRect)
    {
        AddRect(InstRectTextureData{ InRect, InTexRect });
    }

    void BatchDraw2D::AddTextRect(const RectF& InPosRect, const RectF& InTexRect)
    {
        TextBatchCmds.Add(InstRectTextureData{ InPosRect, InTexRect });
    }

    void BatchDraw2D::AddLine(const InstLineData& InLineData)
    {
        LineBatchCmds.Add(InLineData);
    }

    void BatchDraw2D::AddLine(const LineF& InLine, const RGBA& InColor)
    {
        AddLine(InstLineData{ InLine, InColor });
    }
}