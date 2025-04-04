#include "Graphics.h" // Draw3D.h included in Graphics.h

namespace Leviathan
{
    void BatchDrawVoxel::Init()
    {
        ColorCmds.Reserve(DefaultSize);
    }

    void BatchDrawVoxel::Clear()
    {
        ColorCmds.Empty();
    }

    void BatchDrawVoxel::AddVoxel(v4f Pos_, v4f Color_, float Scale_)
    {
        ColorCmds.Add(InstVoxelColorData{ Pos_, Color_, Scale_ });
    }
}

