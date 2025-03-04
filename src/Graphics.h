#ifndef LEVIATHAN_GRAPHICS_H
#define LEVIATHAN_GRAPHICS_H

#include "Leviathan.h"

namespace Leviathan
{
    struct Graphics
    {
        static void Draw();
        static void Init();
        static void Term();

        static ID3D11Device* Device();
        static ID3D11DeviceContext* Context();
    };

    inline void SafeRelease(IUnknown* Ptr) { if (Ptr) { Ptr->Release(); Ptr = nullptr; } }
}

#define DX_CHECK(DXResult) if (FAILED(DXResult)) { DebugBreak(); }

#include "DrawState.h"
#include "Draw2D.h"
#include "MeshState.h"

#endif // LEVIATHAN_GRAPHICS_H

