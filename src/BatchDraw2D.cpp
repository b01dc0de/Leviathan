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
}