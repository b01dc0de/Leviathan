#ifndef LEVIATHAN_GRAPHICS_H
#define LEVIATHAN_GRAPHICS_H

#include "Leviathan.h"
#include "BatchDraw.h"
#include "DrawState.h"
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

    struct Camera;
    void UpdateShaderWorld(ID3D11DeviceContext* Context, m4f* WorldData);
    void UpdateShaderViewProj(ID3D11DeviceContext* Context, Camera* CameraData);

    inline void SafeRelease(IUnknown* Ptr) { if (Ptr) { Ptr->Release(); Ptr = nullptr; } }

    struct DrawStateT;
    struct GameGraphicsContext
    {
        ID3D11DeviceContext* ImmContext;
        ID3D11Buffer* WorldBuffer;
        ID3D11Buffer* ViewProjBuffer;
        Camera* GameCamera;

        BatchDrawCmds* DrawBatch;

        DrawStateT* DrawStateColor;
    };
}

using Leviathan::GameGraphicsContext;

#define DX_CHECK(DXResult) if (FAILED(DXResult)) { DebugBreak(); }

#endif // LEVIATHAN_GRAPHICS_H

