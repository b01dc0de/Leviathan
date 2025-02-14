#include "Graphics.h" // BatchDraw2D.h included in Graphics.h

namespace Leviathan
{
    void BatchDraw2D::Clear()
    {
        BatchCmds.Empty();
    }
    void BatchDraw2D::AddQuad(const InstQuadColorData& InInstQuadColorData)
    {
        BatchCmds.Add(InInstQuadColorData);
    }
    void BatchDraw2D::AddQuad(const QuadF& InQuad, const RGBA& InColor)
    {
        AddQuad(InstQuadColorData{ InQuad, InColor });
    }
    void BatchDraw2D::AddBox(const QuadF& InQuad, const RGBA& InColor, float LineWeight)
    {
        float HalfLineWeight = LineWeight / 2.0f;
        QuadF LeftSide{InQuad.PosX - HalfLineWeight, InQuad.PosY - HalfLineWeight, LineWeight, InQuad.SizeY + HalfLineWeight};
        QuadF RightSide{ InQuad.PosX + InQuad.SizeX - HalfLineWeight, InQuad.PosY - HalfLineWeight, LineWeight, InQuad.SizeY + HalfLineWeight};

        QuadF TopSide{ InQuad.PosX - HalfLineWeight, InQuad.PosY - HalfLineWeight + InQuad.SizeY, InQuad.SizeX + HalfLineWeight, LineWeight};
        QuadF BottomSide{ InQuad.PosX - HalfLineWeight, InQuad.PosY - HalfLineWeight, InQuad.SizeX + HalfLineWeight, LineWeight };

        // TODO: Add this as array (will require new Array<>::Add method)
        AddQuad(LeftSide, InColor);
        AddQuad(TopSide, InColor);
        AddQuad(RightSide, InColor);
        AddQuad(BottomSide, InColor);
    }
}