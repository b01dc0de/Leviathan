#include "Graphics.h" // BatchDraw2D.h included in Graphics.h

namespace Leviathan
{
    void BatchDraw2D::Clear()
    {
        ColorBatchCmds.Empty();
        TextureBatchCmds.Empty();
    }
    void BatchDraw2D::AddQuad(const InstQuadColorData& InInstQuadColorData)
    {
        ColorBatchCmds.Add(InInstQuadColorData);
    }
    void BatchDraw2D::AddQuad(const RectF& InQuad, const RGBA& InColor)
    {
        AddQuad(InstQuadColorData{ InQuad, InColor });
    }
    void BatchDraw2D::AddBox(const RectF& InQuad, const RGBA& InColor, float LineWeight)
    {
        float HalfLineWeight = LineWeight / 2.0f;
        RectF LeftSide{InQuad.PosX - HalfLineWeight, InQuad.PosY - HalfLineWeight, LineWeight, InQuad.SizeY + HalfLineWeight};
        RectF RightSide{ InQuad.PosX + InQuad.SizeX - HalfLineWeight, InQuad.PosY - HalfLineWeight, LineWeight, InQuad.SizeY + HalfLineWeight};

        RectF TopSide{ InQuad.PosX - HalfLineWeight, InQuad.PosY - HalfLineWeight + InQuad.SizeY, InQuad.SizeX + HalfLineWeight, LineWeight};
        RectF BottomSide{ InQuad.PosX - HalfLineWeight, InQuad.PosY - HalfLineWeight, InQuad.SizeX + HalfLineWeight, LineWeight };

        // TODO: Add this as array (will require new Array<>::Add method)
        AddQuad(LeftSide, InColor);
        AddQuad(TopSide, InColor);
        AddQuad(RightSide, InColor);
        AddQuad(BottomSide, InColor);
    }
    void BatchDraw2D::AddQuad(const InstQuadTextureData& InInstQuadTextureData)
    {
        TextureBatchCmds.Add(InInstQuadTextureData);
    }
    void BatchDraw2D::AddQuad(const RectF& InQuad, const RectF& InTexRect)
    {
        AddQuad(InstQuadTextureData{ InQuad, InTexRect });
    }
}