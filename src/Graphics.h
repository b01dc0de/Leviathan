#ifndef LEVIATHAN_GRAPHICS_H
#define LEVIATHAN_GRAPHICS_H

#include "Leviathan.h"

namespace Leviathan
{
    struct Graphics
    {
        static void Init();
        static void UpdateAndDraw();
        static void Term();
    };

#define DX_CHECK(DXResult) if (FAILED(DXResult)) { DebugBreak(); }
#define DX_SAFE_RELEASE(Ptr) if (Ptr) { Ptr->Release(); Ptr = nullptr; }
#define DX_UUID_HELPER(Type, Ptr) __uuidof(Type), (void**)&Ptr
}

#endif // LEVIATHAN_GRAPHICS_H