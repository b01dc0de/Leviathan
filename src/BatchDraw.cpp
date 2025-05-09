#include "Graphics.h" // Draw2D.h included in Graphics.h

namespace Leviathan
{

void BatchDrawCmds::Init()
{
    ColorBatchCmds.Reserve(DefaultSize);
    TextureBatchCmds.Reserve(DefaultSize);
    TextBatchCmds.Reserve(DefaultSize);
    RotationColorBatchCmds.Reserve(DefaultSize);
    RotationTextureBatchCmds.Reserve(DefaultSize);
    ColorVoxelCmds.Reserve(DefaultSize);
}

void BatchDrawCmds::ClearCmdsAll()
{
    ClearCmds2D();
    ClearCmds3D();
}

void BatchDrawCmds::ClearCmds2D()
{
    ColorBatchCmds.Empty();
    TextureBatchCmds.Empty();
    TextBatchCmds.Empty();
    RotationColorBatchCmds.Empty();
    RotationTextureBatchCmds.Empty();
}

void BatchDrawCmds::ClearCmds3D()
{
    ColorVoxelCmds.Empty();
}

void BatchDrawCmds::AddRect(const RectF& InRect, const fColor& InColor)
{
    ColorBatchCmds.Add(InstRectColorData{ InRect, InColor });
}

void BatchDrawCmds::AddBox(const RectF& InRect, const fColor& InColor, float LineWeight)
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

void BatchDrawCmds::AddRect(const RectF& InRect, const RectF& InTexRect)
{
    TextureBatchCmds.Add(InstRectTextureData{ InRect, InTexRect });
}

void BatchDrawCmds::AddTextRect(const RectF& InPosRect, const RectF& InTexRect)
{
    TextBatchCmds.Add(InstRectTextureData{ InPosRect, InTexRect });
}

void BatchDrawCmds::AddRect(const RectF& InRect, const fColor& InColor, float AngleZ)
{
    RotationColorBatchCmds.Add(InstRectColorRotationData{ InRect, InColor, AngleZ });
}

void BatchDrawCmds::AddRect(const RectF& InPosRect, const RectF& InTexRect, float AngleZ)
{
    RotationTextureBatchCmds.Add(InstRectTextureRotationData{ InPosRect, InTexRect, AngleZ });
}

void BatchDrawCmds::AddLine(const LineF& InLine, const fColor& InColor)
{
    constexpr float DefaultLineThickness = 2.0f;
    v2f MinPoint{ Min(InLine.BeginX, InLine.EndX), Min(InLine.BeginY, InLine.EndY) };
    v2f MaxPoint{ Max(InLine.BeginX, InLine.EndX), Max(InLine.BeginY, InLine.EndY) };
    v2f Delta = MaxPoint - MinPoint;
    float LineLength = Length(Delta);
    float DiffX = InLine.EndX - InLine.BeginX;
    float DiffY = InLine.EndY - InLine.BeginY;

    RectF LineRect
    {
        MinPoint.X + Delta.X / 2.0f,
        MinPoint.Y + Delta.Y / 2.0f,
        LineLength,
        DefaultLineThickness
    };
    float LineAngle = atan2f(DiffY, DiffX);
    AddRect(LineRect, InColor, LineAngle);
}

void BatchDrawCmds::AddVoxel(v4f Pos_, v4f Color_, float Scale_)
{
    ColorVoxelCmds.Add(InstVoxelColorData{ Pos_, Color_, Scale_ });
}

}

