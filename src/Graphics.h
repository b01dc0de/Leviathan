#ifndef LEVIATHAN_GRAPHICS_H
#define LEVIATHAN_GRAPHICS_H

#include "Leviathan.h"
#include "DrawState.h"
#include "Draw3D.h"
#include "Draw2D.h"
#include "MeshState.h"

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

    struct GameGraphicsContext
    {
        BatchDraw2D* Draw2D;
    };
}

using Leviathan::GameGraphicsContext;

#define DX_CHECK(DXResult) if (FAILED(DXResult)) { DebugBreak(); }

#endif // LEVIATHAN_GRAPHICS_H

