#ifndef LEVIATHAN_BATCHDRAW_H
#define LEVIATHAN_BATCHDRAW_H

// Included in Graphics.h
#include "Math.h"

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

struct InstVoxelColorData
{
    v4f Pos;
    v4f Color;
    float Scale = 1.0f;
};

struct BatchDrawCmds
{
    // 2D:
    Array<InstRectColorData> ColorBatchCmds;
    Array<InstRectTextureData> TextureBatchCmds;
    Array<InstRectTextureData> TextBatchCmds;
    Array<InstRectColorRotationData> RotationColorBatchCmds;
    Array<InstRectTextureRotationData> RotationTextureBatchCmds;

    // 3D:
    Array<InstVoxelColorData> ColorVoxelCmds;

    static constexpr int DefaultSize = 1024;

    void Init();
    void ClearCmdsAll();
    void ClearCmds2D();
    void ClearCmds3D();

    void AddRect(const RectF& InRect, const fColor& InColor);
    void AddBox(const RectF& InRect, const fColor& InColor, float LineWeight = 1.0f);
    void AddRect(const RectF& InRect, const RectF& InTexRect);
    void AddTextRect(const RectF& InPosRect, const RectF& InTexRect);

    void AddRect(const RectF& InRect, const fColor& InColor, float AngleZ);
    void AddRect(const RectF& InPosRect, const RectF& InTexRect, float AngleZ);
    void AddLine(const LineF& InLine, const fColor& InColor);

    void AddVoxel(v4f Pos_, v4f Color_, float Scale_);
};

}

using Leviathan::BatchDrawCmds;

#endif // LEVIATHAN_BATCHDRAW_H

