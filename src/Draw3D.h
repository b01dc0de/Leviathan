#ifndef LEVIATHAN_DRAW3D_H
#define LEVIATHAN_DRAW3D_H

// Included in Graphics.h
#include "Math.h"

namespace Leviathan
{
    struct InstVoxelColorData
    {
        v4f Pos;
        v4f Color;
        float Scale = 1.0f;
    };

    struct BatchDrawVoxel
    {
        Array<InstVoxelColorData> ColorCmds;

        static constexpr int DefaultSize = 1024;

        void Init();
        void Clear();
        void AddVoxel(v4f Pos_, v4f Color_, float Scale_);
    };
}

#endif // LEVIATHAN_DRAW3D_H
