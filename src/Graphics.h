#ifndef LEVIATHAN_GRAPHICS_H
#define LEVIATHAN_GRAPHICS_H

#include "Leviathan.h"

namespace Leviathan
{
    struct Graphics
    {
        static void UpdateAndDraw();
        static void Init();
        static void Term();
    };

    struct QuadF
    {
        float PosX = 0.0f;
        float PosY = 0.0f;
        float SizeX = 0.0f;
        float SizeY = 0.0f;
    };

    struct RGBA
    {
        float R = 0.0f;
        float G = 0.0f;
        float B = 0.0f;
        float A = 0.0f;
    };

    RGBA Norm8Bit(unsigned char R, unsigned char G, unsigned char B);

    struct InstQuadColorData
    {
        QuadF Rect;
        RGBA Color;
    };

#define DX_CHECK(DXResult) if (FAILED(DXResult)) { DebugBreak(); }
#define DX_SAFE_RELEASE(Ptr) if (Ptr) { Ptr->Release(); Ptr = nullptr; }
#define DX_UUID_HELPER(Type, Ptr) __uuidof(Type), (void**)&Ptr
}

#endif // LEVIATHAN_GRAPHICS_H