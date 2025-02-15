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

#define DX_CHECK(DXResult) if (FAILED(DXResult)) { DebugBreak(); }

    inline void SafeRelease(IUnknown* Ptr) { if (Ptr) { Ptr->Release(); Ptr = nullptr; } }
}

#include "BatchDraw2D.h"

#endif // LEVIATHAN_GRAPHICS_H