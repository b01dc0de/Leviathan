#include "Graphics.h"
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
        DrawStateT DrawStateTexture;
        DrawStateT DrawStateInstRectColor;
        DrawStateT DrawStateInstRectTexture;
        DrawStateT DrawStateInstRectColorRotation;
        DrawStateT DrawStateInstRectTextureRotation;

        ID3D11Buffer* DX_WorldBuffer = nullptr;
        ID3D11Buffer* DX_ViewProjBuffer = nullptr;
        const int WBufferSlot = 0;
        const int VPBufferSlot = 1;

        MeshStateT MeshStateTriangle;
        MeshStateT MeshStateCube;
        MeshStateT MeshStateRect;
        MeshInstStateT MeshInstStateRect;
        MeshInstStateT MeshInstStateRectRotation;

        LvSpriteSheet BulletLimboSpriteSheet;

        LvTexture2D LvDebugTexture;
        LvTexture2D LvTestTexture;
        ID3D11SamplerState* DX_DefaultSamplerState = nullptr;

        D3D_FEATURE_LEVEL UsedFeatureLevel;

        // Direct2D:
        ID2D1Factory* D2_Factory = nullptr;
        IDXGISurface* DXGI_Surface = nullptr;
        ID2D1RenderTarget* D2_RenderTarget = nullptr;
    }

#define DX_UUID_HELPER(Type, Ptr) __uuidof(Type), (void**)&Ptr

    struct SpriteTransform
    {
        v2f Scale{1.0f, 1.0f};
        v2f Pos{0.0f, 0.0f};
        float RotAngle = 0.0f;
    };

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

    float HalfWidth = (float)AppWidth / 2.0f;
    float HalfHeight = (float)AppHeight / 2.0f;
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
    VxMin Vertices_PlatonicLine[] =
    {
        {0.0f, 0.0f, +0.5f, 1.0f},
        {1.0f, 1.0f, +0.5f, 1.0f}
    };

    m4f GetMatrix(const SpriteTransform& InSprTrans)
    {
        return m4f::Scale(InSprTrans.Scale.X, InSprTrans.Scale.Y, 1.0f) * m4f::RotAxisZ(InSprTrans.RotAngle) * m4f::Trans(InSprTrans.Pos.X, InSprTrans.Pos.Y, 0.0f);
    }

    Camera OrthoCamera;
    Camera GameCamera;
    BatchDraw2D Draw2D;
    HandmadeTextSheet HandmadeText;

    void UpdateShaderWorld(ID3D11DeviceContext* Context, m4f* WorldData)
    {
        ID3D11Buffer* WorldBuffer = DX_WorldBuffer;
        UpdateShaderResource(Context, WorldBuffer, WorldData, sizeof(m4f));
    }
    void UpdateShaderViewProj(ID3D11DeviceContext* Context, Camera* CameraData)
    {
        ID3D11Buffer* ViewProjBuffer = DX_ViewProjBuffer;
        UpdateShaderResource(Context, ViewProjBuffer, CameraData, sizeof(Camera));
    }

    void DrawDebugDemo();
    void DrawBatch2D(BatchDraw2D& Draw2D, ID3D11ShaderResourceView* TextureSRV, bool bClear = false);

    static bool bDrawGame = true;
    static bool bForceDrawDebugDemo = true;
    static bool bDrawUI = true;
    static bool bEnableWireframeRaster = false;
    m4f DefaultSpriteWorld = m4f::Trans(-HalfWidth, -HalfHeight, 0.0f);
    constexpr UINT DefaultSampleMask = 0xFFFFFFFF;
    void Graphics::Draw()
    {
        if (KeyboardState::GetKey(LV_KEY_F3)) { bForceDrawDebugDemo = !bForceDrawDebugDemo; }
        if (KeyboardState::GetKey(LV_KEY_F4)) { bEnableWireframeRaster = !bEnableWireframeRaster; }

        Draw2D.Clear();

        if (bEnableWireframeRaster) { DX_ImmContext->RSSetState(DX_WireframeRasterizerState); }
        else { DX_ImmContext->RSSetState(DX_RasterizerState); }
        DX_ImmContext->OMSetBlendState(nullptr, nullptr, DefaultSampleMask);
        DX_ImmContext->OMSetDepthStencilState(nullptr, 0);
        DX_ImmContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);
        v4f ClearColor = { 30.0f / 255.0f, 30.0f / 255.0f, 45.0f / 255.0f, 1.0f };
        constexpr float fClearDepth = 1.0f;
        DX_ImmContext->ClearRenderTargetView(DX_RenderTargetView, &ClearColor.X);
        DX_ImmContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, fClearDepth, 0);

        ID3D11Buffer* World_ViewProjBuffers[] = { DX_WorldBuffer, DX_ViewProjBuffer };
        ID3D11SamplerState* DefaultSampler[] = { DX_DefaultSamplerState };
        SetShaderConstantBuffers(DX_ImmContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);
        SetShaderSamplers(DX_ImmContext, ARRAY_SIZE(DefaultSampler), DefaultSampler);
        
        if (!bDrawGame || bForceDrawDebugDemo)
        {
            DrawDebugDemo();
        }
        if (bDrawGame)
        {
            GameManager::Draw(Draw2D);
            DrawBatch2D(Draw2D, BulletLimboSpriteSheet.LvTex.SRV, true);
        }
        if (bDrawUI)
        {
            UserInterface::Draw(D2_RenderTarget, Draw2D);
            DrawBatch2D(Draw2D, LvTestTexture.SRV, true);
        }

        UINT SyncInterval = 0, PresentFlags = 0;
        DXGI_PRESENT_PARAMETERS PresentParams = {};
        DXGI_SwapChain1->Present1(SyncInterval, PresentFlags, &PresentParams);
    }

    void DrawDebugDemo()
    {
        static bool bDrawInstLines = true;
        static bool bDrawTriangle = true;
        static bool bDrawTexQuad = true;
        static bool bDrawInstRects = true;
        static bool bDrawCube = true;
        static bool bDrawHandmadeText = true;
        static bool bDrawInstRotationDemo = true;

        ID3D11Buffer* World_ViewProjBuffers[] = { DX_WorldBuffer, DX_ViewProjBuffer };
        ID3D11ShaderResourceView* HandmadeFontTextureSRV[] = { HandmadeText.LvTex2D.SRV };
        ID3D11SamplerState* DefaultSampler[] = { DX_DefaultSamplerState };
        ID3D11ShaderResourceView* TestTextureSRV[] = { LvTestTexture.SRV };

        UpdateShaderViewProj(DX_ImmContext, &OrthoCamera);

        SetShaderConstantBuffers(DX_ImmContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);

        if (bDrawTriangle)
        {
            DX_ImmContext->OMSetBlendState(nullptr, nullptr, DefaultSampleMask);
            m4f TriangleWorld = m4f::Scale(128.0f, 128.0f, 1.0f) * m4f::Trans(256.0f, -256.0f, 0.0f);
            UpdateShaderWorld(DX_ImmContext, &TriangleWorld);

            DrawMesh(DX_ImmContext, DrawStateColor, MeshStateTriangle);
        }

        if (bDrawTexQuad)
        {
            DX_ImmContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
            m4f TexQuadWorld = m4f::Scale(100.0f, 100.0f, 1.0f) * m4f::Trans(+256.0f, +256.0f, 0.0f);
            UpdateShaderWorld(DX_ImmContext, &TexQuadWorld);
            SetShaderResourceViews(DX_ImmContext, ARRAY_SIZE(TestTextureSRV), TestTextureSRV);

            DrawMesh(DX_ImmContext, DrawStateTexture, MeshStateRect);
        }
        DX_ImmContext->OMSetDepthStencilState(DX_Draw2DDepthStencilState, 0);

        if (bDrawInstRects)
        {
            DX_ImmContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
            UpdateShaderWorld(DX_ImmContext, &DefaultSpriteWorld);
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

        if (bDrawCube)
        {
            DX_ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

            static float RotationX = 0.0f;
            static float RotationY = 0.0f;
            static constexpr float RotSpeed = (1.0f / 60.0f) / 10.0f;
            RotationX += RotSpeed;
            RotationY += RotSpeed * 0.5f;
            m4f CubeWorld = m4f::RotAxisX(RotationX) * m4f::RotAxisY(RotationY);
            UpdateShaderWorld(DX_ImmContext, &CubeWorld);
            UpdateShaderViewProj(DX_ImmContext, &GameCamera);

            SetShaderConstantBuffers(DX_ImmContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);

            DrawMesh(DX_ImmContext, DrawStateColor, MeshStateCube);
        }

        if (bDrawInstLines)
        {
            ASSERT(ARRAY_SIZE(InstData_LinePos) == ARRAY_SIZE(InstData_LineColors));
            for (int Idx = 0; Idx < ARRAY_SIZE(InstData_LinePos); Idx++)
            {
                Draw2D.AddLine(InstData_LinePos[Idx], InstData_LineColors[Idx]);
            }
        }

        if (bDrawHandmadeText)
        {
            v2f TextOrigin{ AppWidth / 2.0f, AppHeight / 2.0f };
            const float TextScale = 300.0f;
            const char TextMsg[] = "HELLO WORLD";
            const char TextRow0[] = "ABCDEFGHIJKL";
            const char TextRow1[] = "MNOPQRSTUVWX";
            const char TextRow2[] = "YZ0123456789";
            HandmadeText.Draw(Draw2D, TextOrigin, TextScale, TextMsg, ARRAY_SIZE(TextMsg) - 1);
            TextOrigin.Y -= 60.0f;
            HandmadeText.Draw(Draw2D, TextOrigin, TextScale, TextRow0, ARRAY_SIZE(TextRow0) - 1);
            TextOrigin.Y -= 60.0f;
            HandmadeText.Draw(Draw2D, TextOrigin, TextScale, TextRow1, ARRAY_SIZE(TextRow1) - 1);
            TextOrigin.Y -= 60.0f;
            HandmadeText.Draw(Draw2D, TextOrigin, TextScale, TextRow2, ARRAY_SIZE(TextRow2) - 1);
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
                Draw2D.AddRect(CurrRect, CurrColor, CurrAngle);

                CurrRect.PosX += RectSize * sinf(fTime * 0.5f);
                CurrRect.PosY -= RectSize * sinf(fTime * 2.0f);
                Draw2D.AddRect(CurrRect, TexRect, CurrAngle);
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
                    Draw2D.AddRect(CurrRect, CurrColor, CurrAngle);
                }

            }
        }

        DrawBatch2D(Draw2D, LvTestTexture.SRV, true);
    }

    void DrawBatch2D(BatchDraw2D& Draw2D, ID3D11ShaderResourceView* TextureSRV, bool bClear)
    {
        DX_ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
        DX_ImmContext->OMSetDepthStencilState(DX_Draw2DDepthStencilState, 0);
        UpdateShaderWorld(DX_ImmContext, &DefaultSpriteWorld);
        UpdateShaderViewProj(DX_ImmContext, &OrthoCamera);

        ID3D11Buffer* World_ViewProjBuffers[] = { DX_WorldBuffer, DX_ViewProjBuffer };
        SetShaderConstantBuffers(DX_ImmContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);

        // Color:
        if (Draw2D.ColorBatchCmds.Num > 0)
        {
            DrawMeshInstanced
            (
                DX_ImmContext,
                DrawStateInstRectColor,
                MeshInstStateRect,
                Draw2D.ColorBatchCmds.Num,
                Draw2D.ColorBatchCmds.Data
            );
        }
        if (Draw2D.RotationColorBatchCmds.Num > 0)
        {
            DrawMeshInstanced
            (
                DX_ImmContext,
                DrawStateInstRectColorRotation,
                MeshInstStateRectRotation,
                Draw2D.RotationColorBatchCmds.Num,
                Draw2D.RotationColorBatchCmds.Data
            );
        }

        // Texture:
        DX_ImmContext->OMSetBlendState(DX_AlphaBlendState, nullptr, 0xFFFFFFFF);
        if (Draw2D.TextureBatchCmds.Num > 0) { } // TODO:
        if (Draw2D.TextBatchCmds.Num > 0)
        {
            ASSERT(HandmadeText.LvTex2D.SRV);
            SetShaderResourceViews(DX_ImmContext, 1, &HandmadeText.LvTex2D.SRV);

            DrawMeshInstanced
            (
                DX_ImmContext,
                DrawStateInstRectTexture,
                MeshInstStateRect,
                Draw2D.TextBatchCmds.Num,
                Draw2D.TextBatchCmds.Data
            );
        }
        if (Draw2D.RotationTextureBatchCmds.Num > 0)
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
                Draw2D.RotationTextureBatchCmds.Num,
                Draw2D.RotationTextureBatchCmds.Data
            );
        }
        if (bClear) { Draw2D.Clear(); }
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

        UINT CreateDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
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

        Draw2D.Init();

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

        MeshStateTriangle = LoadMeshStateTriangle();
        MeshStateCube = LoadMeshStateCube();
        MeshInstStateRect = LoadMeshInstStateRect();
        MeshInstStateRectRotation = LoadMeshInstStateRectRotation();
        ASSERT(sizeof(InstRectColorData) == sizeof(InstRectTextureData));

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

        ImageT DebugImage = {};
        GetDebugImage(DebugImage);
        LvDebugTexture = LoadTextureFromImage(DebugImage, DX_Device);
        LvTestTexture = LoadTextureBMP("Assets/TestTexture.bmp", DX_Device);

        HandmadeText.Init(DX_Device, "Assets/HandmadeTextFont.bmp", 12, 6);

        MeshStateRect = LoadMeshStateRect();

        { // Direct2D
            DX_CHECK(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2_Factory));
            DX_CHECK(DXGI_SwapChain1->GetBuffer(0, DX_UUID_HELPER(IDXGISurface, DXGI_Surface)));
            D2D1_RENDER_TARGET_PROPERTIES D2_RT_Properties = D2D1::RenderTargetProperties(
                D2D1_RENDER_TARGET_TYPE_DEFAULT,
                D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
                0, 0
            );
            DX_CHECK(D2_Factory->CreateDxgiSurfaceRenderTarget(
                DXGI_Surface,
                &D2_RT_Properties,
                &D2_RenderTarget 
            ));
        }

        v3f CameraPos{ 5.0f, 5.0f, -5.0f };
        v3f CameraLookAt{ 0.0f, 0.0f, 0.0f };
        GameCamera.Persp(CameraPos, CameraLookAt);

        OrthoCamera.Ortho((float)AppWidth, (float)AppHeight, -2.0f);

        UserInterface::Init(D2_RenderTarget);

        BulletLimboSpriteSheet = LoadSpriteSheet("Assets/Sprites/BulletLimbo_SpriteSheet.bmp", DX_Device, 1, 3);
    }

    void Graphics::Term()
    {
        UserInterface::Term();

        { // Direct2D:
            SafeRelease(D2_Factory);
            SafeRelease(DXGI_Surface);
            SafeRelease(D2_RenderTarget);
        }

        SafeRelease(MeshStateTriangle);
        SafeRelease(MeshStateCube);
        SafeRelease(MeshStateRect);
        SafeRelease(MeshInstStateRect);
        SafeRelease(MeshInstStateRectRotation);

        SafeRelease(LvDebugTexture);
        SafeRelease(LvTestTexture);

        SafeRelease(DX_WorldBuffer);
        SafeRelease(DX_ViewProjBuffer);

        SafeRelease(DX_DefaultSamplerState);

        SafeRelease(DrawStateColor);
        SafeRelease(DrawStateTexture);
        SafeRelease(DrawStateInstRectColor);
        SafeRelease(DrawStateInstRectTexture);
        SafeRelease(DrawStateInstRectColorRotation);
        SafeRelease(DrawStateInstRectTextureRotation);

        SafeRelease(DXGI_SwapChain1);
        SafeRelease(DX_Backbuffer);
        SafeRelease(DX_RenderTargetView);
        SafeRelease(DX_RasterizerState);
        SafeRelease(DX_WireframeRasterizerState);
        SafeRelease(DX_DepthStencil);
        SafeRelease(DX_DepthStencilView);
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

