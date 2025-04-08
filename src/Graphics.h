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
    struct DrawStateT;
    struct LvGFXContext
    {
        ID3D11DeviceContext* ImmContext;
        ID3D11Buffer* World_ViewProjBuffers[2];
        Camera* GameCamera;

        BatchDrawCmds* DrawBatch;

        DrawStateT* DrawStateColor;

        void SetShaderConstantBuffers_WVP();
        void UpdateShaderWorld(m4f* WorldData);
        void UpdateShaderViewProj(Camera* CameraData);
    };

    inline void SafeRelease(IUnknown* Ptr) { if (Ptr) { Ptr->Release(); Ptr = nullptr; } }
}

using Leviathan::LvGFXContext;

#define DX_CHECK(DXResult) if (FAILED(DXResult)) { DebugBreak(); }

#endif // LEVIATHAN_GRAPHICS_H

