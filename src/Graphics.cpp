#include "Graphics.h"
#include "AssetProcessor.h"
#include "Camera.h"
#include "DrawState.h"
#include "Image.h"
#include "InputVisualizer.h"
#include "UserInterface.h"
#include "Fonts.h"

#include "Game/GameManager.h"

namespace Leviathan
{

inline namespace GraphicsState
{
IDXGIFactory2* DXGI_Factory2 = nullptr;
ID3D11Device* DX_Device = nullptr;
ID3D11DeviceContext* DX_ImmContext = nullptr;
IDXGISwapChain1* DXGI_SwapChain1 = nullptr;
ID3D11Texture2D* DX_Backbuffer = nullptr;
ID3D11RenderTargetView* DX_RenderTargetView = nullptr;
ID3D11RasterizerState* DX_RasterizerState = nullptr;
ID3D11RasterizerState* DX_WireframeRasterizerState = nullptr;
ID3D11Texture2D* DX_DepthStencil = nullptr;
ID3D11DepthStencilView* DX_DepthStencilView = nullptr;
ID3D11DepthStencilState* DX_DefaultDepthStencilState = nullptr;
ID3D11DepthStencilState* DX_Draw2DDepthStencilState = nullptr;
ID3D11BlendState* DX_DefaultBlendState = nullptr;
ID3D11BlendState* DX_AlphaBlendState = nullptr;

DrawStateT DrawStateColor;
DrawStateT DrawStateUnicolor;
DrawStateT DrawStateTexture;
DrawStateT DrawStateInstRectColor;
DrawStateT DrawStateInstRectTexture;
DrawStateT DrawStateInstRectColorRotation;
DrawStateT DrawStateInstRectTextureRotation;
DrawStateT DrawStateInstVoxelColor;

ID3D11Buffer* DX_WorldBuffer = nullptr;
ID3D11Buffer* DX_ViewProjBuffer = nullptr;
const int WBufferSlot = 0;
const int VPBufferSlot = 1;
ID3D11Buffer* DX_UnicolorBuffer = nullptr;
const int UnicolorBufferSlot = 2;
ID3D11SamplerState* DX_DefaultSamplerState = nullptr;

MeshStateT MeshStateTriangle;
MeshStateT MeshStateCube;
MeshStateT MeshStateCubeMin;
MeshStateT MeshStateCubeFacesColor;
MeshStateT MeshStateCubeFacesTex;
MeshStateT MeshStateRect;
MeshStateT MeshStateCircle;
MeshStateT MeshStateSphere;
MeshStateT MeshStateOBJPyramid;
MeshStateT MeshStateOBJCylinder;
MeshInstStateT MeshInstStateRect;
MeshInstStateT MeshInstStateRectRotation;
MeshInstStateT MeshInstStateVoxelColor;

LvSpriteSheet BulletLimboSpriteSheet;

LvTexture2D LvDebugTexture;
LvTexture2D LvTestTexture;

D3D_FEATURE_LEVEL UsedFeatureLevel;

LvGFXContext GlobalGFXContext;



Camera OrthoCamera;
Camera GameCamera;
BatchDrawCmds DrawBatch;
LvFont ProggyCleanFont;

}

#define DX_UUID_HELPER(Type, Ptr) __uuidof(Type), (void**)&Ptr

void LvGFXContext::SetShaderConstantBuffers_WVP()
{
    SetShaderConstantBuffers(ImmContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);
}

void LvGFXContext::UpdateShaderWorld(m4f* WorldData)
{
    ID3D11Buffer* WorldBuffer = DX_WorldBuffer;
    UpdateShaderResource(ImmContext, WorldBuffer, WorldData, sizeof(m4f));
}
void LvGFXContext::UpdateShaderViewProj(Camera* CameraData)
{
    ID3D11Buffer* ViewProjBuffer = DX_ViewProjBuffer;
    UpdateShaderResource(ImmContext, ViewProjBuffer, CameraData, sizeof(Camera));
}

void DrawDebugAxis();
void DrawDebugDemo();
void DrawBatch2D(BatchDrawCmds& Draw2D, ID3D11ShaderResourceView* TextureSRV, bool bClear = false);
void DrawBatch3D(BatchDrawCmds& Draw3D, bool bClear = false);

static float HalfWidth = (float)AppWidth / 2.0f;
static float HalfHeight = (float)AppHeight / 2.0f;
static m4f DefaultSpriteWorld = m4f::Trans(-HalfWidth, -HalfHeight, 0.0f);
constexpr UINT DefaultSampleMask = 0xFFFFFFFF;
void Graphics::Draw()
{
    static bool bDrawDebugAxis = true;
    static bool bDrawGame = true;
    static bool bForceDrawDebugDemo = true;
    static bool bDrawUI = true;
    static bool bEnableWireframeRaster = false;

    if (KeyboardState::GetKey(LV_KEY_F3)) { bForceDrawDebugDemo = !bForceDrawDebugDemo; }
    if (KeyboardState::GetKey(LV_KEY_F4)) { bEnableWireframeRaster = !bEnableWireframeRaster; }
    if (KeyboardState::GetKey(LV_KEY_F8)) { bDrawGame = !bDrawGame; }

    DrawBatch.ClearCmdsAll();

    if (bEnableWireframeRaster) { DX_ImmContext->RSSetState(DX_WireframeRasterizerState); }
    else { DX_ImmContext->RSSetState(DX_RasterizerState); }
    DX_ImmContext->OMSetBlendState(nullptr, nullptr, DefaultSampleMask);
    DX_ImmContext->OMSetDepthStencilState(nullptr, 0);
    DX_ImmContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);
    v4f ClearColor = { 30.0f / 255.0f, 30.0f / 255.0f, 45.0f / 255.0f, 1.0f };
    constexpr float fClearDepth = 1.0f;
    DX_ImmContext->ClearRenderTargetView(DX_RenderTargetView, &ClearColor.X);
    DX_ImmContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, fClearDepth, 0);

    if (bDrawDebugAxis)
    {
        DrawDebugAxis();
    }

    ID3D11Buffer* World_ViewProjBuffers[] = { DX_WorldBuffer, DX_ViewProjBuffer };
    ID3D11SamplerState* DefaultSampler[] = { DX_DefaultSamplerState };
    SetShaderConstantBuffers(DX_ImmContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);
    SetShaderSamplers(DX_ImmContext, ARRAY_SIZE(DefaultSampler), DefaultSampler);

    if (bDrawGame)
    {
        GameManager::Draw(GlobalGFXContext);
        DrawBatch2D(*GlobalGFXContext.DrawBatch, BulletLimboSpriteSheet.LvTex.SRV, true);
    }
    if (!bDrawGame || bForceDrawDebugDemo)
    {
        DrawDebugDemo();
    }
    if (bDrawUI)
    {
        UserInterface::Draw(DrawBatch, ProggyCleanFont);
        DrawBatch2D(DrawBatch, LvTestTexture.SRV, true);
    }

    UINT SyncInterval = 0, PresentFlags = 0;
    DXGI_PRESENT_PARAMETERS PresentParams = {};
    DXGI_SwapChain1->Present1(SyncInterval, PresentFlags, &PresentParams);
}

void DrawDebugAxis()
{
    DX_ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    GlobalGFXContext.UpdateShaderViewProj(&GameCamera);

    ID3D11Buffer* World_ViewProj_Unicolor_Buffers[] = { DX_WorldBuffer, DX_ViewProjBuffer, DX_UnicolorBuffer };
    SetShaderConstantBuffers(DX_ImmContext, ARRAY_SIZE(World_ViewProj_Unicolor_Buffers), World_ViewProj_Unicolor_Buffers);
    GlobalGFXContext.SetShaderConstantBuffers_WVP();

    static constexpr float Length = 1000.0f;
    static constexpr float MinSize = 0.1f;

    auto UpdateUnicolor = [](v4f* UnicolorData) -> void
    {
        UpdateShaderResource(DX_ImmContext, DX_UnicolorBuffer, UnicolorData, sizeof(m4f));
    };

    v4f Red{ 1.0f, 0.0f, 0.0f, 1.0f };
    v4f Green{ 0.0f, 1.0f, 0.0f, 1.0f };
    v4f Blue{ 0.0f, 0.0f, 1.0f, 1.0f };
    // X-axis
    {
        m4f CubeWorld = m4f::Scale(Length, MinSize, MinSize) * m4f::Trans(Length / 2.0f, 0.0f, 0.0f);
        GlobalGFXContext.UpdateShaderWorld(&CubeWorld);
        UpdateUnicolor(&Red);
        DrawMesh(DX_ImmContext, DrawStateUnicolor, MeshStateCubeMin);
    }

    // Y-axis
    {
        m4f CubeWorld = m4f::Scale(MinSize, Length, MinSize) * m4f::Trans(0.0f, Length / 2.0f, 0.0f);
        GlobalGFXContext.UpdateShaderWorld(&CubeWorld);
        UpdateUnicolor(&Green);
        DrawMesh(DX_ImmContext, DrawStateUnicolor, MeshStateCubeMin);
    }

    // Z-axis
    {
        m4f CubeWorld = m4f::Scale(MinSize, MinSize, Length) * m4f::Trans(0.0f, 0.0f, Length / 2.0f);
        GlobalGFXContext.UpdateShaderWorld(&CubeWorld);
        UpdateUnicolor(&Blue);
        DrawMesh(DX_ImmContext, DrawStateUnicolor, MeshStateCubeMin);
    }
}

void DrawDebugDemo()
{
    static bool bDrawInstLines = false;
    static bool bDrawShapes = true;
    static bool bDrawTexQuad = false;
    static bool bDrawInstRects = false;
    static bool bDrawSphere = true;
    static bool bDrawCube = false;
    static bool bDrawInstVoxels = false;
    static bool bDrawText = false;
    static bool bDrawTextSheet = true;
    static bool bDrawInstRotationDemo = false;

    static float RotationX = 0.0f;
    static float RotationY = 0.0f;
    static constexpr float RotSpeed = (1.0f / 60.0f) / 25.0f;

    ID3D11ShaderResourceView* ProggyCleanFontTextureSRV[] = { ProggyCleanFont.LvTex2D.SRV };
    ID3D11SamplerState* DefaultSampler[] = { DX_DefaultSamplerState };
    ID3D11ShaderResourceView* TestTextureSRV[] = { LvTestTexture.SRV };

    GlobalGFXContext.UpdateShaderViewProj(&OrthoCamera);

    GlobalGFXContext.SetShaderConstantBuffers_WVP();

    if (bDrawShapes)
    {
        DX_ImmContext->OMSetBlendState(nullptr, nullptr, DefaultSampleMask);

        m4f TriangleWorld = m4f::Scale(128.0f, 128.0f, 1.0f) * m4f::Trans(+256.0f, -256.0f, 0.0f);
        GlobalGFXContext.UpdateShaderWorld(&TriangleWorld);

        DrawMesh(DX_ImmContext, DrawStateColor, MeshStateTriangle);

        m4f CircleWorld = m4f::Scale(64.0f, 64.0f, 1.0f) * m4f::Trans(+384.0f, -256.0f, 0.0f);
        GlobalGFXContext.UpdateShaderWorld(&CircleWorld);
        DrawMesh(DX_ImmContext, DrawStateColor, MeshStateCircle);

    }

    if (bDrawTexQuad)
    {
        DX_ImmContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
        m4f TexQuadWorld = m4f::Scale(100.0f, 100.0f, 1.0f) * m4f::Trans(+256.0f, +256.0f, 0.0f);
        GlobalGFXContext.UpdateShaderWorld(&TexQuadWorld);
        SetShaderResourceViews(DX_ImmContext, ARRAY_SIZE(TestTextureSRV), TestTextureSRV);

        DrawMesh(DX_ImmContext, DrawStateTexture, MeshStateRect);
    }
    DX_ImmContext->OMSetDepthStencilState(DX_Draw2DDepthStencilState, 0);

    if (bDrawInstRects)
    {
        const float TestInstRectSize = 50.0f;
        const float RightRectX = (float)AppWidth - TestInstRectSize;
        const float BottomRectY = (float)AppHeight - TestInstRectSize;
        InstRectTextureData InstRectTextureDataArray[] =
        {
            { {0.0f, BottomRectY, TestInstRectSize, TestInstRectSize}, {0.0f, 0.0f, 0.5f, 0.5f} },
            { {RightRectX, BottomRectY, TestInstRectSize, TestInstRectSize}, {0.5f, 0.0f, 0.5f, 0.5f} },
            { {0.0f, 0.0f, TestInstRectSize, TestInstRectSize}, { 0.0f, 0.5f, 0.5f, 0.5f } },
            { {RightRectX, 0.0f, TestInstRectSize, TestInstRectSize}, {0.5f, 0.5f, 0.5f, 0.5f} }
        };

        DX_ImmContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
        GlobalGFXContext.UpdateShaderWorld(&DefaultSpriteWorld);
        SetShaderResourceViews(DX_ImmContext, ARRAY_SIZE(TestTextureSRV), TestTextureSRV);

        DrawMeshInstanced
        (
            DX_ImmContext,
            DrawStateInstRectTexture,
            MeshInstStateRect,
            ARRAY_SIZE(InstRectTextureDataArray),
            InstRectTextureDataArray
        );
    }

    if (bDrawSphere)
    {
        DX_ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
        m4f SphereWorld = m4f::Scale(4.0f) * m4f::RotAxisX(RotationX) * m4f::RotAxisY(RotationY) * m4f::Trans(-25.0f, -10.0f, 0.0f);
        GlobalGFXContext.UpdateShaderWorld(&SphereWorld);
        GlobalGFXContext.UpdateShaderViewProj(&GameCamera);
        GlobalGFXContext.SetShaderConstantBuffers_WVP();
        DrawMesh(DX_ImmContext, DrawStateColor, MeshStateSphere);
    }

    static bool bDrawOBJTests= true;
    if (bDrawOBJTests)
    {
        DX_ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
        m4f PyramidWorld = m4f::Scale(2.0f) * m4f::RotAxisX(RotationX) * m4f::RotAxisY(RotationY) * m4f::Trans(+10.0f, 0.0f, 0.0f);
        GlobalGFXContext.UpdateShaderWorld(&PyramidWorld);
        GlobalGFXContext.UpdateShaderViewProj(&GameCamera);
        GlobalGFXContext.SetShaderConstantBuffers_WVP();
        DrawMesh(DX_ImmContext, DrawStateColor, MeshStateOBJPyramid);

        m4f CylinderWorld = m4f::Scale(2.0f) * m4f::RotAxisX(RotationX) * m4f::RotAxisY(RotationY) * m4f::Trans(+5.0f, +5.0f, 0.0f);
        GlobalGFXContext.UpdateShaderWorld(&CylinderWorld);
        GlobalGFXContext.UpdateShaderViewProj(&GameCamera);
        GlobalGFXContext.SetShaderConstantBuffers_WVP();
        DrawMesh(DX_ImmContext, DrawStateColor, MeshStateOBJCylinder);
    }

    if (bDrawCube)
    {
        DX_ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

        m4f CubeWorld = m4f::RotAxisX(RotationX) * m4f::RotAxisY(RotationY);
        GlobalGFXContext.UpdateShaderWorld(&CubeWorld);
        GlobalGFXContext.UpdateShaderViewProj(&GameCamera);

        GlobalGFXContext.SetShaderConstantBuffers_WVP();

        static bool bCubeFacesColor = true;
        static bool bCubeFacesTexture = false;
        if (bCubeFacesColor)
        {
            DrawMesh(DX_ImmContext, DrawStateColor, MeshStateCubeFacesColor);
        }
        else if (bCubeFacesTexture)
        {
            SetShaderResourceViews(DX_ImmContext, ARRAY_SIZE(TestTextureSRV), TestTextureSRV);
            DrawMesh(DX_ImmContext, DrawStateTexture, MeshStateCubeFacesTex);
        }
        else
        {
            DrawMesh(DX_ImmContext, DrawStateColor, MeshStateCube);
        }
    }

    if (bDrawInstVoxels)
    {
        constexpr int NumInstVoxels = 12;
        constexpr float VoxelStoneHedgeSize = 5.0f;
        constexpr float MaxScale = 1.0f;

        for (int Idx = 0; Idx < NumInstVoxels; Idx++)
        {
            float Value = (float)Idx / (float)NumInstVoxels;
            float Radians = Value * fTAU;
            v4f Pos{ cosf(Radians + RotationX) * VoxelStoneHedgeSize, sinf(RotationY) * VoxelStoneHedgeSize * 0.25f, sinf(Radians) * VoxelStoneHedgeSize };
            v4f Color{ cosf(RotationX), sinf(RotationY), Value, 1.0f };
            float Scale = ((float)(Idx + 1) / (float)NumInstVoxels) * MaxScale;
            DrawBatch.AddVoxel(Pos, Color, Scale);
        }
        DrawBatch3D(DrawBatch, true);
    }

    if (bDrawInstLines)
    {
        LineF InstData_LinePos[]
        {
            {0.0f, 0.0f, AppWidth, AppHeight},
            {0.0f, AppHeight, AppWidth, 0.0f},
            {HalfWidth, 0.0f, HalfWidth, AppHeight},
            {0.0f, HalfHeight, AppWidth, HalfHeight},
        };
        fColor InstData_LineColors[]
        {
            {1.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f}
        };

        ASSERT(ARRAY_SIZE(InstData_LinePos) == ARRAY_SIZE(InstData_LineColors));
        for (int Idx = 0; Idx < ARRAY_SIZE(InstData_LinePos); Idx++)
        {
            DrawBatch.AddLine(InstData_LinePos[Idx], InstData_LineColors[Idx]);
        }
    }

    if (bDrawTextSheet)
    {
        DX_ImmContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
        //DX_ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
        m4f TexQuadWorld = m4f::Scale(200.0f, 400.0f, 1.0f) * m4f::Trans(-512.0f, -128.0f, 0.0f);
        GlobalGFXContext.UpdateShaderWorld(&TexQuadWorld);
        SetShaderResourceViews(DX_ImmContext, ARRAY_SIZE(ProggyCleanFontTextureSRV), ProggyCleanFontTextureSRV);

        DrawMesh(DX_ImmContext, DrawStateTexture, MeshStateRect);
    }

    if (bDrawText)
    {
        DX_ImmContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
        v2f TextOrigin{ AppWidth / 2.0f, AppHeight / 2.0f };
        const float TextScale = 5.0f;
        const char TextMsg[] = "HELLO WORLD";
        const char TextRow0[] = "ABCDEFGHIJKL";
        const char TextRow1[] = "MNOPQRSTUVWX";
        const char TextRow2[] = "YZ0123456789";
        ProggyCleanFont.Draw(DrawBatch, TextOrigin, TextScale, TextMsg, ARRAY_SIZE(TextMsg) - 1);
        TextOrigin.Y -= 50.0f;
        ProggyCleanFont.Draw(DrawBatch, TextOrigin, TextScale, TextRow0, ARRAY_SIZE(TextRow0) - 1);
        TextOrigin.Y -= 50.0f;
        ProggyCleanFont.Draw(DrawBatch, TextOrigin, TextScale, TextRow1, ARRAY_SIZE(TextRow1) - 1);
        TextOrigin.Y -= 50.0f;
        ProggyCleanFont.Draw(DrawBatch, TextOrigin, TextScale, TextRow2, ARRAY_SIZE(TextRow2) - 1);
    }

    if (bDrawInstRotationDemo)
    {
        constexpr int RotationDemoNum = 12;
        constexpr float RectSize = 50.0f;
        constexpr float CircleSize = 200.0f;
        const v2f Origin{ AppWidth / 2.0f, AppHeight * 0.75f };
        RectF Rect = { Origin.X, Origin.Y, RectSize, RectSize };
        RectF TexRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        float fTime = (float)Clock::Time();
        static bool bDrawCoolRotations = true;
        for (int Idx = 0; Idx < RotationDemoNum; Idx++)
        {
            float CurrAngle = (float)Idx / (float)RotationDemoNum * fTAU;
            RectF CurrRect = Rect;
            CurrRect.PosX += cosf(CurrAngle * fTime) * CircleSize * sinf(fTime);
            CurrRect.PosY += sinf(CurrAngle * fTime) * CircleSize * cosf(-fTime);
            fColor CurrColor{ 1.0f, 0.0f, 0.0f, 1.0f };
            DrawBatch.AddRect(CurrRect, CurrColor, CurrAngle);

            CurrRect.PosX += RectSize * sinf(fTime * 0.5f);
            CurrRect.PosY -= RectSize * sinf(fTime * 2.0f);
            DrawBatch.AddRect(CurrRect, TexRect, CurrAngle);
        }

        if (0)
        {
            const v2f TestOrigin{ 100.0f, 100.0f };
            for (int Idx = 0; Idx < 4; Idx++)
            {
                RectF CurrRect =
                {
                    TestOrigin.X + (Idx % 2 == 1 ? RectSize : -RectSize),
                    TestOrigin.Y + (Idx / 2 == 0 ? RectSize : -RectSize),
                    RectSize,
                    RectSize
                };
                float CurrAngle = fTime;
                fColor CurrColor =
                {
                    Idx & 1 ? 1.0f : 0.0f,
                    Idx & 2 ? 1.0f : 0.0f,
                    Idx & 3 ? 1.0f : 0.0f,
                    1.0f
                };
                DrawBatch.AddRect(CurrRect, CurrColor, CurrAngle);
            }

        }
    }

    DrawBatch2D(DrawBatch, LvTestTexture.SRV, true);
}

void DrawBatch2D(BatchDrawCmds& DrawBatch, ID3D11ShaderResourceView* TextureSRV, bool bClear)
{
    DX_ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    DX_ImmContext->OMSetDepthStencilState(DX_Draw2DDepthStencilState, 0);
    GlobalGFXContext.UpdateShaderWorld(&DefaultSpriteWorld);
    GlobalGFXContext.UpdateShaderViewProj(&OrthoCamera);

    ID3D11Buffer* World_ViewProjBuffers[] = { DX_WorldBuffer, DX_ViewProjBuffer };
    SetShaderConstantBuffers(DX_ImmContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);

    // Color:
    if (DrawBatch.ColorBatchCmds.Num > 0)
    {
        DrawMeshInstanced
        (
            DX_ImmContext,
            DrawStateInstRectColor,
            MeshInstStateRect,
            DrawBatch.ColorBatchCmds.Num,
            DrawBatch.ColorBatchCmds.Data
        );
    }
    if (DrawBatch.RotationColorBatchCmds.Num > 0)
    {
        DrawMeshInstanced
        (
            DX_ImmContext,
            DrawStateInstRectColorRotation,
            MeshInstStateRectRotation,
            DrawBatch.RotationColorBatchCmds.Num,
            DrawBatch.RotationColorBatchCmds.Data
        );
    }

    // Texture:
    DX_ImmContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
    if (DrawBatch.TextureBatchCmds.Num > 0) {} // TODO:
    if (DrawBatch.TextBatchCmds.Num > 0)
    {
        ASSERT(ProggyCleanFont.LvTex2D.SRV);
        SetShaderResourceViews(DX_ImmContext, 1, &ProggyCleanFont.LvTex2D.SRV);

        DrawMeshInstanced
        (
            DX_ImmContext,
            DrawStateInstRectTexture,
            MeshInstStateRect,
            DrawBatch.TextBatchCmds.Num,
            DrawBatch.TextBatchCmds.Data
        );
    }
    if (DrawBatch.RotationTextureBatchCmds.Num > 0)
    {
        if (TextureSRV)
        {
            SetShaderResourceViews(DX_ImmContext, 1, &TextureSRV);
        }

        DrawMeshInstanced
        (
            DX_ImmContext,
            DrawStateInstRectTextureRotation,
            MeshInstStateRectRotation,
            DrawBatch.RotationTextureBatchCmds.Num,
            DrawBatch.RotationTextureBatchCmds.Data
        );
    }
    if (bClear) { DrawBatch.ClearCmds2D(); }
}

void DrawBatch3D(BatchDrawCmds& DrawBatch, bool bClear)
{
    DX_ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    DX_ImmContext->OMSetDepthStencilState(DX_DefaultDepthStencilState, 0);
    //UpdateShaderWorld(DX_ImmContext, &World);
    GlobalGFXContext.UpdateShaderViewProj(&GameCamera);

    SetShaderConstantBuffers(DX_ImmContext, 1, &DX_ViewProjBuffer, 0);

    // Color:
    if (DrawBatch.ColorVoxelCmds.Num > 0)
    {
        DrawMeshInstanced
        (
            DX_ImmContext,
            DrawStateInstVoxelColor,
            MeshInstStateVoxelColor,
            DrawBatch.ColorVoxelCmds.Num,
            DrawBatch.ColorVoxelCmds.Data
        );
    }
    if (bClear) { DrawBatch.ClearCmds3D(); }
}

void Graphics::Init()
{
    D3D_FEATURE_LEVEL SupportedFeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT NumSupportedFeatureLevels = ARRAY_SIZE(SupportedFeatureLevels);

    CreateDXGIFactory1(DX_UUID_HELPER(IDXGIFactory2, DXGI_Factory2));

    UINT CreateDeviceFlags = 0;
#ifdef _DEBUG
    CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
    DX_CHECK(D3D11CreateDevice
    (
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        CreateDeviceFlags,
        SupportedFeatureLevels,
        NumSupportedFeatureLevels,
        D3D11_SDK_VERSION,
        &DX_Device,
        &UsedFeatureLevel,
        &DX_ImmContext
    ));

    DXGI_SAMPLE_DESC SampleDesc = {};
    SampleDesc.Count = 1;
    SampleDesc.Quality = 0;

    DXGI_SWAP_CHAIN_DESC1 SwapChainDesc1 = {};
    SwapChainDesc1.Width = AppWidth;
    SwapChainDesc1.Height = AppHeight;
    SwapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc1.Stereo = FALSE;
    SwapChainDesc1.SampleDesc = SampleDesc;
    SwapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc1.BufferCount = 2;
    SwapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
    SwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    SwapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    SwapChainDesc1.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    DX_CHECK(DXGI_Factory2->CreateSwapChainForHwnd(
        DX_Device,
        AppWindow,
        &SwapChainDesc1,
        nullptr,
        nullptr,
        &DXGI_SwapChain1
    ));

    DX_CHECK(DXGI_SwapChain1->GetBuffer(0, DX_UUID_HELPER(ID3D11Texture2D, DX_Backbuffer)));

    DX_CHECK(DX_Device->CreateRenderTargetView(DX_Backbuffer, nullptr, &DX_RenderTargetView));

    D3D11_RASTERIZER_DESC RasterDesc = {};
    RasterDesc.FillMode = D3D11_FILL_SOLID;
    RasterDesc.CullMode = D3D11_CULL_BACK;
    RasterDesc.FrontCounterClockwise = TRUE;
    RasterDesc.DepthClipEnable = TRUE;
    RasterDesc.ScissorEnable = FALSE;
    RasterDesc.MultisampleEnable = TRUE;
    RasterDesc.AntialiasedLineEnable = TRUE;
    DX_CHECK(DX_Device->CreateRasterizerState(&RasterDesc, &DX_RasterizerState));

    D3D11_RASTERIZER_DESC WireframeRasterDesc = RasterDesc;
    WireframeRasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    WireframeRasterDesc.CullMode = D3D11_CULL_NONE;
    DX_CHECK(DX_Device->CreateRasterizerState(&WireframeRasterDesc, &DX_WireframeRasterizerState));

    D3D11_TEXTURE2D_DESC DepthDesc = {};
    DepthDesc.Width = AppWidth;
    DepthDesc.Height = AppHeight;
    DepthDesc.MipLevels = 1;
    DepthDesc.ArraySize = 1;
    DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DepthDesc.SampleDesc = SampleDesc;
    DepthDesc.Usage = D3D11_USAGE_DEFAULT;
    DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    DepthDesc.CPUAccessFlags = 0;
    DepthDesc.MiscFlags = 0;
    DX_CHECK(DX_Device->CreateTexture2D(&DepthDesc, nullptr, &DX_DepthStencil));

    D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc = {};
    DepthStencilViewDesc.Format = DepthDesc.Format;
    DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DepthStencilViewDesc.Texture2D.MipSlice = 0;
    DX_CHECK(DX_Device->CreateDepthStencilView(DX_DepthStencil, &DepthStencilViewDesc, &DX_DepthStencilView));

    D3D11_DEPTH_STENCIL_DESC Instanced2DDepthStencilStateDesc = {};
    Instanced2DDepthStencilStateDesc.DepthEnable = TRUE;
    Instanced2DDepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    Instanced2DDepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    Instanced2DDepthStencilStateDesc.StencilEnable = FALSE;
    DX_Device->CreateDepthStencilState(&Instanced2DDepthStencilStateDesc, &DX_Draw2DDepthStencilState);

    { // AlphaBlendState:
        D3D11_RENDER_TARGET_BLEND_DESC AlphaRTBlendDesc = {};
        AlphaRTBlendDesc.BlendEnable = TRUE;
        AlphaRTBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        AlphaRTBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        AlphaRTBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
        AlphaRTBlendDesc.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
        AlphaRTBlendDesc.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        AlphaRTBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        AlphaRTBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        D3D11_BLEND_DESC AlphaBlendDesc = {};
        AlphaBlendDesc.AlphaToCoverageEnable = FALSE;
        AlphaBlendDesc.IndependentBlendEnable = FALSE;
        AlphaBlendDesc.RenderTarget[0] = AlphaRTBlendDesc;

        DX_CHECK(DX_Device->CreateBlendState(&AlphaBlendDesc, &DX_AlphaBlendState));
    }

    D3D11_VIEWPORT ViewportDesc = {};
    ViewportDesc.TopLeftX = 0.0f;
    ViewportDesc.TopLeftY = 0.0f;
    ViewportDesc.Width = (FLOAT)AppWidth;
    ViewportDesc.Height = (FLOAT)AppHeight;
    ViewportDesc.MinDepth = 0.0f;
    ViewportDesc.MaxDepth = 1.0f;
    DX_ImmContext->RSSetViewports(1, &ViewportDesc);

    const wchar_t* BaseShaderFilename = L"src/hlsl/BaseShader.hlsl";
    {
        const D3D_SHADER_MACRO DefinesVxColor[] =
        {
            "ENABLE_VERTEX_COLOR", "1",
            "ENABLE_VERTEX_TEXTURE", "0",
            "ENABLE_WVP_TRANSFORM", "1",
            "ENABLE_UNICOLOR", "0",
            nullptr, nullptr
        };
        D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        DrawStateColor = CreateDrawState(DX_Device, BaseShaderFilename, DefinesVxColor, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
    }

    {
        const D3D_SHADER_MACRO DefinesVxUnicolor[] =
        {
            "ENABLE_VERTEX_COLOR", "0",
            "ENABLE_VERTEX_TEXTURE", "0",
            "ENABLE_WVP_TRANSFORM", "1",
            "ENABLE_UNICOLOR", "1",
            nullptr, nullptr
        };
        D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        DrawStateUnicolor = CreateDrawState(DX_Device, BaseShaderFilename, DefinesVxUnicolor, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
    }

    {
        const D3D_SHADER_MACRO DefinesVxTexture[] =
        {
            "ENABLE_VERTEX_COLOR", "0",
            "ENABLE_VERTEX_TEXTURE", "1",
            "ENABLE_WVP_TRANSFORM", "1",
            "ENABLE_UNICOLOR", "0",
            nullptr, nullptr
        };
        D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        DrawStateTexture = CreateDrawState(DX_Device, BaseShaderFilename, DefinesVxTexture, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
    }

    const wchar_t* InstRectShaderFilename = L"src/hlsl/InstRectShader.hlsl";
    { // InstRectColor
        const D3D_SHADER_MACRO DefinesInst[] =
        {
            "INST_RECT_COLOR", "1",
            "INST_RECT_TEXTURE", "0",
            nullptr, nullptr
        };
        D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "RECT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
        };

        DrawStateInstRectColor = CreateDrawState(DX_Device, InstRectShaderFilename, DefinesInst, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
    }

    { // InstRectTexture
        const D3D_SHADER_MACRO DefinesInst[] =
        {
            "INST_RECT_COLOR", "0",
            "INST_RECT_TEXTURE", "1",
            nullptr, nullptr
        };
        D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "RECT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
        };

        DrawStateInstRectTexture = CreateDrawState(DX_Device, InstRectShaderFilename, DefinesInst, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
    }

    { // InstRectColorRotation
        const D3D_SHADER_MACRO DefinesInst[] =
        {
            "INST_RECT_COLOR", "1",
            "INST_RECT_TEXTURE", "0",
            "INST_RECT_ROTATION", "1",
            nullptr, nullptr
        };
        D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "RECT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "ANGLE", 0, DXGI_FORMAT_R32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        };

        DrawStateInstRectColorRotation = CreateDrawState
        (
            DX_Device,
            InstRectShaderFilename,
            DefinesInst,
            InputLayoutDesc,
            ARRAY_SIZE(InputLayoutDesc)
        );
    }

    { // InstRectColorRotation
        const D3D_SHADER_MACRO DefinesInst[] =
        {
            "INST_RECT_COLOR", "0",
            "INST_RECT_TEXTURE", "1",
            "INST_RECT_ROTATION", "1",
            nullptr, nullptr
        };
        D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "RECT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "ANGLE", 0, DXGI_FORMAT_R32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        };

        DrawStateInstRectTextureRotation = CreateDrawState
        (
            DX_Device,
            InstRectShaderFilename,
            DefinesInst,
            InputLayoutDesc,
            ARRAY_SIZE(InputLayoutDesc)
        );
    }

    const wchar_t* InstVoxelShaderFilename = L"src/hlsl/InstVoxelShader.hlsl";
    { // InstVoxelColor
        const D3D_SHADER_MACRO Defines[] =
        {
            nullptr, nullptr
        };
        D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "INSTPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "SCALE", 0, DXGI_FORMAT_R32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        };

        DrawStateInstVoxelColor = CreateDrawState
        (
            DX_Device,
            InstVoxelShaderFilename,
            Defines,
            InputLayoutDesc,
            ARRAY_SIZE(InputLayoutDesc)
        );
    }

    DrawBatch.Init();

    D3D11_BUFFER_DESC WorldBufferDesc = {};
    WorldBufferDesc.ByteWidth = sizeof(m4f);
    WorldBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    WorldBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    WorldBufferDesc.CPUAccessFlags = 0;
    DX_CHECK(DX_Device->CreateBuffer(&WorldBufferDesc, nullptr, &DX_WorldBuffer));
    D3D11_BUFFER_DESC ViewProjBufferDesc = {};
    ViewProjBufferDesc.ByteWidth = sizeof(m4f) * 2;
    ViewProjBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    ViewProjBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ViewProjBufferDesc.CPUAccessFlags = 0;
    DX_CHECK(DX_Device->CreateBuffer(&ViewProjBufferDesc, nullptr, &DX_ViewProjBuffer));

    D3D11_BUFFER_DESC UnicolorBufferDesc = {};
    UnicolorBufferDesc.ByteWidth = sizeof(v4f);
    UnicolorBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    UnicolorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    UnicolorBufferDesc.CPUAccessFlags = 0;
    DX_CHECK(DX_Device->CreateBuffer(&UnicolorBufferDesc, nullptr, &DX_UnicolorBuffer));

    MeshStateTriangle = LoadMeshStateTriangle();
    MeshStateCube = LoadMeshStateCube();
    MeshStateCubeMin = LoadMeshStateCubeMin();
    MeshStateCubeFacesColor = LoadMeshStateCubeFacesColor();
    MeshStateCubeFacesTex = LoadMeshStateCubeFacesTex();

    MeshStateCircle = LoadMeshStateUnitCircle();
    MeshStateSphere = LoadMeshStateUnitSphere();

    MeshInstStateRect = LoadMeshInstStateRect();
    MeshInstStateRectRotation = LoadMeshInstStateRectRotation();
    ASSERT(sizeof(InstRectColorData) == sizeof(InstRectTextureData));

    MeshInstStateVoxelColor = LoadMeshInstStateVoxel();

    { // Default sampler state:
        D3D11_TEXTURE_ADDRESS_MODE SamplerAddressMode = D3D11_TEXTURE_ADDRESS_WRAP;
        D3D11_SAMPLER_DESC DefaultSamplerDesc = {};
        DefaultSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        DefaultSamplerDesc.AddressU = SamplerAddressMode;
        DefaultSamplerDesc.AddressV = SamplerAddressMode;
        DefaultSamplerDesc.AddressW = SamplerAddressMode;
        DefaultSamplerDesc.MipLODBias = 0.0f;
        DefaultSamplerDesc.MaxAnisotropy = 0;
        DefaultSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        DefaultSamplerDesc.MinLOD = 0;
        DefaultSamplerDesc.MaxLOD = 0;
        DX_CHECK(DX_Device->CreateSamplerState(&DefaultSamplerDesc, &DX_DefaultSamplerState));
    }

    {
        ImageT DebugImage = {};
        GetDebugImage(DebugImage);
        LvDebugTexture = LoadTextureFromImage(DebugImage, DX_Device);
        LvTestTexture = LoadTextureBMP("Assets/TestTexture.bmp", DX_Device);
        SafeRelease(DebugImage);
    }

    ProggyCleanFont.Init(DX_Device, "Assets/Fonts/ProggyClean_36pt.bmp");

    MeshStateRect = LoadMeshStateRect();

    v3f CameraPos{ 5.0f, 10.0f, 10.0f };
    v3f CameraLookAt{ 0.0f, 0.0f, 0.0f };
    GameCamera.Persp(CameraPos, CameraLookAt);

    OrthoCamera.Ortho((float)AppWidth, (float)AppHeight, -2.0f);

    BulletLimboSpriteSheet = LoadSpriteSheet("Assets/Sprites/BulletLimbo_SpriteSheet.bmp", DX_Device, 1, 3);

    GlobalGFXContext = {
        DX_ImmContext,
        { DX_WorldBuffer, DX_ViewProjBuffer},
        &GameCamera,
        &DrawBatch,
        &DrawStateColor
    };

    constexpr bool bTestOBJLoading = true;
    if (bTestOBJLoading)
    {
        MeshStateOBJPyramid = LoadMeshOBJ("Assets/simple-pyramid-test.obj");
        MeshStateOBJCylinder = LoadMeshOBJ("Assets/simple-cylinder-test.obj");
    }
    
}

void Graphics::Term()
{
    SafeRelease(BulletLimboSpriteSheet);
    SafeRelease(ProggyCleanFont);

    SafeRelease(MeshStateTriangle);
    SafeRelease(MeshStateCube);
    SafeRelease(MeshStateCubeMin);
    SafeRelease(MeshStateCubeFacesColor);
    SafeRelease(MeshStateCubeFacesTex);
    SafeRelease(MeshStateRect);
    SafeRelease(MeshStateCircle);
    SafeRelease(MeshStateSphere);
    SafeRelease(MeshStateOBJPyramid);
    SafeRelease(MeshStateOBJCylinder);
    SafeRelease(MeshInstStateRect);
    SafeRelease(MeshInstStateRectRotation);
    SafeRelease(MeshInstStateVoxelColor);

    SafeRelease(LvDebugTexture);
    SafeRelease(LvTestTexture);

    SafeRelease(DX_WorldBuffer);
    SafeRelease(DX_ViewProjBuffer);
    SafeRelease(DX_UnicolorBuffer);
    SafeRelease(DX_DefaultSamplerState);

    SafeRelease(DrawStateColor);
    SafeRelease(DrawStateUnicolor);
    SafeRelease(DrawStateTexture);
    SafeRelease(DrawStateInstRectColor);
    SafeRelease(DrawStateInstRectTexture);
    SafeRelease(DrawStateInstRectColorRotation);
    SafeRelease(DrawStateInstRectTextureRotation);
    SafeRelease(DrawStateInstVoxelColor);

    SafeRelease(DXGI_SwapChain1);
    SafeRelease(DX_Backbuffer);
    SafeRelease(DX_RenderTargetView);
    SafeRelease(DX_RasterizerState);
    SafeRelease(DX_WireframeRasterizerState);
    SafeRelease(DX_DepthStencil);
    SafeRelease(DX_DepthStencilView);
    SafeRelease(DX_DefaultDepthStencilState);
    SafeRelease(DX_Draw2DDepthStencilState);
    SafeRelease(DX_DefaultBlendState);
    SafeRelease(DX_AlphaBlendState);

    SafeRelease(DXGI_Factory2);
    SafeRelease(DX_ImmContext);

    if (DX_Device)
    {
        ID3D11Debug* DX_Debug = nullptr;
        DX_Device->QueryInterface(DX_UUID_HELPER(ID3D11Debug, DX_Debug));
        if (DX_Debug) { DX_Debug->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL); }
        SafeRelease(DX_Debug);
    }

    SafeRelease(DX_Device);
}

ID3D11Device* Graphics::Device()
{
    return DX_Device;
}

ID3D11DeviceContext* Context()
{
    return DX_ImmContext;
}

}

