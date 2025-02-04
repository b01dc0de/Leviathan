#include "Graphics.h"
#include "Camera.h"
#include "DrawState.h"
#include "Image.h"
#include "UserInterface.h"

namespace Leviathan
{
    IDXGIFactory2* DXGI_Factory2 = nullptr;
    ID3D11Device* DX_Device = nullptr;
    ID3D11DeviceContext* DX_ImmediateContext = nullptr;
    IDXGISwapChain1* DXGI_SwapChain1 = nullptr;
    ID3D11Texture2D* DX_Backbuffer = nullptr;
    ID3D11RenderTargetView* DX_RenderTargetView = nullptr;
    ID3D11RasterizerState* DX_RasterizerState = nullptr;
    ID3D11Texture2D* DX_DepthStencil = nullptr;
    ID3D11DepthStencilView* DX_DepthStencilView = nullptr;

    DrawStateT DrawStateColor;
    DrawStateT DrawStateTexture;

    ID3D11Buffer* DX_WBuffer = nullptr;
    ID3D11Buffer* DX_VPBuffer = nullptr;
    const int WBufferSlot = 0;
    const int VPBufferSlot = 1;

    MeshStateT MeshStateTriangle;
    MeshStateT MeshStateCube;
    MeshStateT MeshStateQuad;

    ID3D11Texture2D* DX_DebugTexture = nullptr;
    ID3D11ShaderResourceView* DX_DebugTextureSRV = nullptr;
    ID3D11SamplerState* DX_DefaultSamplerState = nullptr;

    D3D_FEATURE_LEVEL UsedFeatureLevel;

    // Direct2D:
    ID2D1Factory* D2_Factory = nullptr;
    IDXGISurface* DXGI_Surface = nullptr;
    ID2D1RenderTarget* D2_RenderTarget = nullptr;
    ID2D1LinearGradientBrush* D2_LinearGradientBrush = nullptr;
    ID2D1GradientStopCollection* D2_GradientStops = nullptr;
    ID2D1SolidColorBrush* D2_LightGrayBrush = nullptr;

    struct VxColor
    {
        v4f Pos;
        v4f Col;
    };

    struct VxTexture
    {
        v4f Pos;
        v2f TexUV;
    };

    struct VPData
    {
        m4f View;
        m4f Proj;
    };

    VxColor Vertices_Triangle[] =
    {
        { {+0.0f, +0.5f, +0.5f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },
        { {-0.5f, -0.5f, +0.5f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
        { {+0.5f, -0.5f, +0.5f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },
    };
    unsigned int Indices_Triangle[] =
    {
        0, 1, 2
    };

    constexpr float fUnit = 1.0f;
    VxColor Vertices_Cube[] =
    {
        {{-fUnit, +fUnit, +fUnit, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 0
        {{+fUnit, +fUnit, +fUnit, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // 1
        {{-fUnit, -fUnit, +fUnit, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // 2
        {{+fUnit, -fUnit, +fUnit, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 3

        {{-fUnit, +fUnit, -fUnit, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // 4
        {{+fUnit, +fUnit, -fUnit, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // 5
        {{-fUnit, -fUnit, -fUnit, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // 6
        {{+fUnit, -fUnit, -fUnit, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, // 7
    };
    unsigned int Indices_Cube[] =
    {
        // Front
        0, 2, 1,
        1, 2, 3,
        // Back
        5, 7, 4,
        4, 7, 6,
        // Top
        4, 0, 5,
        5, 0, 1,
        // Bottom
        2, 6, 3,
        3, 6, 7,
        // Left
        4, 6, 0,
        0, 6, 2,
        // Right
        1, 3, 5,
        5, 3, 7,
    };

    VxTexture Vertices_Quad[] =
    {
        { {-0.5f, +0.5f, +0.5f, +1.0f}, { 0.0f, 0.0f } },
        { {+0.5f, +0.5f, +0.5f, +1.0f}, { 1.0f, 0.0f } },
        { {-0.5f, -0.5f, +0.5f, +1.0f}, { 0.0f, 1.0f } },
        { {+0.5f, -0.5f, +0.5f, +1.0f}, { 1.0f, 1.0f } },
    };
    unsigned int Indices_Quad[] =
    {
        0, 2, 1,
        1, 2, 3
    };

    Camera GameCamera;

    void Graphics::UpdateAndDraw()
    {
        DX_ImmediateContext->RSSetState(DX_RasterizerState);
        DX_ImmediateContext->OMSetRenderTargets(1, &DX_RenderTargetView, DX_DepthStencilView);
        v4f ClearColor = { 30.0f / 255.0f, 30.0f / 255.0f, 45.0f / 255.0f, 1.0f };
        float fClearDepth = 1.0f;
        DX_ImmediateContext->ClearRenderTargetView(DX_RenderTargetView, &ClearColor.X);
        DX_ImmediateContext->ClearDepthStencilView(DX_DepthStencilView, D3D11_CLEAR_DEPTH, fClearDepth, 0);

        m4f IdentityMatrix = {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };

        m4f W = IdentityMatrix;
        VPData VP = { IdentityMatrix, IdentityMatrix };
        DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &W, sizeof(m4f), 0);
        DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &VP, sizeof(VPData), 0);

        static bool bD2Background = true;
        if (bD2Background)
        { // Direct2D background
            DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
            DX_CHECK(DXGI_SwapChain1->GetDesc(&SwapChainDesc));

            D2D1_SIZE_F TargetSize = D2_RenderTarget->GetSize();

            D2_RenderTarget->BeginDraw();

            static bool bDrawGradientBG = true;
            if (bDrawGradientBG)
            {
                D2_LinearGradientBrush->SetTransform(D2D1::Matrix3x2F::Scale(TargetSize));
                D2D1_RECT_F Rect = D2D1::RectF(0.0f, 0.0f, TargetSize.width, TargetSize.height);
                D2_RenderTarget->FillRectangle(&Rect, D2_LinearGradientBrush);
            }

            static bool bDrawGridBG = true;
            if (bDrawGridBG)
            {
                const int GridX = 16;
                const int GridY = 9;
                const float CellSizeX = TargetSize.width / (float)GridX;
                const float CellSizeY = TargetSize.height / (float)GridY;
                const FLOAT StrokeWidth = 1.0f;
                ID2D1StrokeStyle* StrokeStyle = nullptr;
                for (int ColIdx = 1; ColIdx < GridX; ColIdx++)
                {
                    D2D1_POINT_2F StartPoint = D2D1::Point2F(CellSizeX * ColIdx, 0.0f);
                    D2D1_POINT_2F EndPoint = D2D1::Point2F(CellSizeX * ColIdx, TargetSize.height);
                    D2_RenderTarget->DrawLine(StartPoint, EndPoint, D2_LightGrayBrush, StrokeWidth, StrokeStyle);
                }
                for (int RowIdx = 1; RowIdx < GridY; RowIdx++)
                {
                    D2D1_POINT_2F StartPoint = D2D1::Point2F(0.0f, CellSizeY * RowIdx);
                    D2D1_POINT_2F EndPoint = D2D1::Point2F(TargetSize.width, CellSizeY * RowIdx);
                    D2_RenderTarget->DrawLine(StartPoint, EndPoint, D2_LightGrayBrush, StrokeWidth, StrokeStyle);
                }
            }

            DX_CHECK(D2_RenderTarget->EndDraw());
        }

        static bool bDrawTriangle = false;
        if (bDrawTriangle)
        { // Draw triangle
            UINT Offset = 0;
            const UINT Stride = sizeof(VxColor);
            DX_ImmediateContext->IASetInputLayout(DrawStateColor.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, 1, &MeshStateTriangle.VxBuffer, &Stride, &Offset);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateTriangle.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateColor.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WBufferSlot, 1, &DX_WBuffer);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateColor.PixelShader, nullptr, 0);

            DX_ImmediateContext->DrawIndexed(MeshStateTriangle.NumInds, 0u, 0u);
        }

        static bool bDrawTexQuad = false;
        if (bDrawTexQuad)
        { // Draw tex quad
            UINT Offset = 0;
            const UINT Stride = sizeof(VxTexture);
            DX_ImmediateContext->IASetInputLayout(DrawStateTexture.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, 1, &MeshStateQuad.VxBuffer, &Stride, &Offset);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateQuad.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateTexture.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WBufferSlot, 1, &DX_WBuffer);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateTexture.PixelShader, nullptr, 0);
            DX_ImmediateContext->PSSetShaderResources(0, 1, &DX_DebugTextureSRV);
            DX_ImmediateContext->PSSetSamplers(0, 1, &DX_DefaultSamplerState);

            DX_ImmediateContext->DrawIndexed(MeshStateQuad.NumInds, 0u, 0u);
        }

        { // Draw cube
            static float RotationX = 0.0f;
            static float RotationY = 0.0f;
            static constexpr float RotSpeed = (1.0f / 60.0f) / 10.0f;
            RotationX += RotSpeed;
            RotationY += RotSpeed * 0.5f;
            m4f CubeWorld = m4f::RotAxisX(RotationX) * m4f::RotAxisY(RotationY);
            DX_ImmediateContext->UpdateSubresource(DX_WBuffer, 0, nullptr, &CubeWorld, sizeof(m4f), 0);
            DX_ImmediateContext->UpdateSubresource(DX_VPBuffer, 0, nullptr, &GameCamera.View, sizeof(Camera), 0);

            UINT Offset = 0;
            const UINT Stride = sizeof(VxColor);
            DX_ImmediateContext->IASetInputLayout(DrawStateColor.InputLayout);
            DX_ImmediateContext->IASetVertexBuffers(0, 1, &MeshStateCube.VxBuffer, &Stride, &Offset);
            DX_ImmediateContext->IASetIndexBuffer(MeshStateCube.IxBuffer, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DrawStateColor.VertexShader, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WBufferSlot, 1, &DX_WBuffer);
            DX_ImmediateContext->VSSetConstantBuffers(VPBufferSlot, 1, &DX_VPBuffer);
            DX_ImmediateContext->PSSetShader(DrawStateColor.PixelShader, nullptr, 0);

            DX_ImmediateContext->DrawIndexed(MeshStateCube.NumInds, 0u, 0u);
        }

        static bool bDrawUI = true;
        if (bDrawUI)
        {
            UserInterface::Draw(D2_RenderTarget);
        }

        UINT SyncInterval = 0, PresentFlags = 0;
        DXGI_PRESENT_PARAMETERS PresentParams = {};
        DXGI_SwapChain1->Present1(SyncInterval, PresentFlags, &PresentParams);
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
            &DX_ImmediateContext
        ));

        UINT FrameRefreshRate = 60;
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

        D3D11_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc = {};
        RenderTargetBlendDesc.BlendEnable = TRUE;
        RenderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        RenderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        RenderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
        RenderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
        RenderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
        RenderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        RenderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALPHA;
        D3D11_BLEND_DESC BlendDesc = {};
        BlendDesc.RenderTarget[0] = RenderTargetBlendDesc;
        ID3D11BlendState* DX_BlendState = nullptr;
        DX_CHECK(DX_Device->CreateBlendState(&BlendDesc, &DX_BlendState));

        D3D11_VIEWPORT ViewportDesc = {};
        ViewportDesc.TopLeftX = 0.0f;
        ViewportDesc.TopLeftY = 0.0f;
        ViewportDesc.Width = (FLOAT)AppWidth;
        ViewportDesc.Height = (FLOAT)AppHeight;
        ViewportDesc.MinDepth = 0.0f;
        ViewportDesc.MaxDepth = 1.0f;
        DX_ImmediateContext->RSSetViewports(1, &ViewportDesc);

        {
            const D3D_SHADER_MACRO DefinesVxColor[] =
            {
                "ENABLE_VERTEX_COLOR", "1",
                "ENABLE_VERTEX_TEXTURE", "0",
                "ENABLE_WVP_TRANSFORM", "1",
                nullptr, nullptr
            };
            const wchar_t* ShaderFilename = L"src/hlsl/BaseShader.hlsl";
            D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            DrawStateColor = CreateDrawState(DX_Device, ShaderFilename, DefinesVxColor, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
        }

        {
            const D3D_SHADER_MACRO DefinesVxTexture[] =
            {
                "ENABLE_VERTEX_COLOR", "0",
                "ENABLE_VERTEX_TEXTURE", "1",
                "ENABLE_WVP_TRANSFORM", "1",
                nullptr, nullptr
            };
            const wchar_t* ShaderFilename = L"src/hlsl/BaseShader.hlsl";
            D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            DrawStateTexture = CreateDrawState(DX_Device, ShaderFilename, DefinesVxTexture, InputLayoutDesc, ARRAY_SIZE(InputLayoutDesc));
        }

        D3D11_BUFFER_DESC WorldBufferDesc = {};
        WorldBufferDesc.ByteWidth = sizeof(m4f);
        WorldBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        WorldBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        WorldBufferDesc.CPUAccessFlags = 0;
        D3D11_BUFFER_DESC ViewProjBufferDesc = {};
        ViewProjBufferDesc.ByteWidth = sizeof(VPData);
        ViewProjBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        ViewProjBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        ViewProjBufferDesc.CPUAccessFlags = 0;
        DX_CHECK(DX_Device->CreateBuffer(&WorldBufferDesc, nullptr, &DX_WBuffer));
        DX_CHECK(DX_Device->CreateBuffer(&ViewProjBufferDesc, nullptr, &DX_VPBuffer));

        MeshStateTriangle = CreateMeshState
        (
            DX_Device,
            sizeof(VxColor),
            ARRAY_SIZE(Vertices_Triangle),
            Vertices_Triangle,
            ARRAY_SIZE(Indices_Triangle),
            Indices_Triangle
        );
        MeshStateCube = CreateMeshState
        (
            DX_Device,
            sizeof(VxColor),
            ARRAY_SIZE(Vertices_Cube),
            Vertices_Cube,
            ARRAY_SIZE(Indices_Cube),
            Indices_Cube
        );

        {
            ImageT DebugImage = {};
            GetDebugImage(DebugImage);

            D3D11_SUBRESOURCE_DATA DebugTextureDataDesc[] = { {} };
            DebugTextureDataDesc[0].pSysMem = DebugImage.PxBuffer;
            DebugTextureDataDesc[0].SysMemPitch = sizeof(RGBA32) * DebugImage.Width;
            DebugTextureDataDesc[0].SysMemSlicePitch = DebugImage.PxBytes;
            D3D11_TEXTURE2D_DESC DebugTextureDesc = {};
            DebugTextureDesc.Width = DebugImage.Width;
            DebugTextureDesc.Height = DebugImage.Height;
            DebugTextureDesc.MipLevels = 1;
            DebugTextureDesc.ArraySize = 1;
            DebugTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            DebugTextureDesc.SampleDesc.Count = 1;
            DebugTextureDesc.SampleDesc.Quality = 0;
            DebugTextureDesc.Usage = D3D11_USAGE_DEFAULT;
            DebugTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            DebugTextureDesc.CPUAccessFlags = 0;
            DebugTextureDesc.MiscFlags = 0;
            DX_CHECK(DX_Device->CreateTexture2D(&DebugTextureDesc, &DebugTextureDataDesc[0], &DX_DebugTexture));
            DX_CHECK(DX_Device->CreateShaderResourceView(DX_DebugTexture, nullptr, &DX_DebugTextureSRV));

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

        MeshStateQuad = CreateMeshState
        (
            DX_Device,
            sizeof(VxTexture),
            ARRAY_SIZE(Vertices_Quad),
            Vertices_Quad,
            ARRAY_SIZE(Indices_Quad),
            Indices_Quad
        );

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
            D2D1_GRADIENT_STOP GradientBrushStops[] =
            {
                {0.0f, D2D1::ColorF(D2D1::ColorF::MidnightBlue, 1)},
                {1.0f, D2D1::ColorF(D2D1::ColorF::DarkViolet, 1)},
            };
            DX_CHECK(D2_RenderTarget->CreateGradientStopCollection(GradientBrushStops, ARRAY_SIZE(GradientBrushStops), D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &D2_GradientStops));
            DX_CHECK(D2_RenderTarget->CreateLinearGradientBrush(
                D2D1::LinearGradientBrushProperties(D2D1::Point2F(0.0f, 0.0f), D2D1::Point2F(1.0, 1.0f)),
                D2_GradientStops, &D2_LinearGradientBrush));

            D2D1_COLOR_F LightGrayColor = D2D1::ColorF(0xE1E6EF);
            D2_RenderTarget->CreateSolidColorBrush(LightGrayColor, &D2_LightGrayBrush);
        }

        v3f CameraPos{5.0f, 5.0f, -5.0f};
        v3f CameraLookAt{ 0.0f, 0.0f, 0.0f };
        GameCamera.Persp(CameraPos, CameraLookAt);

        UserInterface::Init(D2_RenderTarget);
    }

    void Graphics::Term()
    {
        UserInterface::Term();

        { // Direct2D:
            DX_SAFE_RELEASE(D2_Factory);
            DX_SAFE_RELEASE(DXGI_Surface);
            DX_SAFE_RELEASE(D2_RenderTarget);
            DX_SAFE_RELEASE(D2_LinearGradientBrush);
            DX_SAFE_RELEASE(D2_GradientStops);
            DX_SAFE_RELEASE(D2_LightGrayBrush);
        }

        SafeRelease(MeshStateTriangle);
        SafeRelease(MeshStateCube);
        SafeRelease(MeshStateQuad);

        DX_SAFE_RELEASE(DX_WBuffer);
        DX_SAFE_RELEASE(DX_VPBuffer);

        SafeRelease(DrawStateColor);
        SafeRelease(DrawStateTexture);

        DX_SAFE_RELEASE(DXGI_SwapChain1);
        DX_SAFE_RELEASE(DX_Backbuffer);
        DX_SAFE_RELEASE(DX_RenderTargetView);
        DX_SAFE_RELEASE(DX_RasterizerState);
        DX_SAFE_RELEASE(DX_DepthStencil);
        DX_SAFE_RELEASE(DX_DepthStencilView);

        DX_SAFE_RELEASE(DXGI_Factory2);
        DX_SAFE_RELEASE(DX_ImmediateContext);

        if (DX_Device)
        {
            ID3D11Debug* DX_Debug = nullptr;
            DX_Device->QueryInterface(DX_UUID_HELPER(ID3D11Debug, DX_Debug));
            if (DX_Debug) { DX_Debug->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL); }
            DX_SAFE_RELEASE(DX_Debug);
        }

        DX_SAFE_RELEASE(DX_Device);
    }
}