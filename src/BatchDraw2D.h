#ifndef LEVIATHAN_BATCHDRAW2D_H
#define LEVIATHAN_BATCHDRAW2D_H

// Included in Graphics.h

namespace Leviathan
{
    struct QuadF
    {
        float PosX = 0.0f;
        float PosY = 0.0f;
        float SizeX = 0.0f;
        float SizeY = 0.0f;
    };

    struct InstQuadColorData
    {
        QuadF Rect;
        RGBA Color;
    };

    struct BatchDraw2D
    {
        Array<InstQuadColorData> BatchCmds;

        void Clear();
        void AddQuad(const InstQuadColorData& InInstQuadColorData);
        void AddQuad(const QuadF& InQuad, const RGBA& InColor);
        void AddBox(const QuadF& InQuad, const RGBA& InColor, float LineWeight = 1.0f);
    };
}

#endif // LEVIATHAN_BATCHDRAW2D_H
