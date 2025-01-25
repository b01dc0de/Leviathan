#include "Graphics.h"

namespace Leviathan
{

#define ARRAY_SIZE(Array) (sizeof(Array) / sizeof(Array[0]))

#define DX_CHECK(DXResult) if (FAILED(DXResult)) { DebugBreak(); }
#define DX_SAFE_RELEASE(Ptr) if (Ptr) { Ptr->Release(); Ptr = nullptr; }

    IDXGIFactory2* DXGI_Factory2 = nullptr;
    ID3D11Device* DX_Device = nullptr;
    ID3D11DeviceContext* DX_ImmediateContext = nullptr;
    IDXGISwapChain1* DXGI_SwapChain1 = nullptr;
    ID3D11Texture2D* DX_Backbuffer = nullptr;
    ID3D11RenderTargetView* DX_RenderTargetView = nullptr;
    ID3D11RasterizerState* DX_RasterizerState = nullptr;
    ID3D11Texture2D* DX_DepthStencil = nullptr;
    ID3D11DepthStencilView* DX_DepthStencilView = nullptr;

    ID3D11VertexShader* DX_VertexShaderColor = nullptr;
    ID3D11PixelShader* DX_PixelShaderColor = nullptr;
    ID3D11InputLayout* DX_InputLayoutColor = nullptr;

    ID3D11VertexShader* DX_VertexShaderTexture = nullptr;
    ID3D11PixelShader* DX_PixelShaderTexture = nullptr;
    ID3D11InputLayout* DX_InputLayoutTexture = nullptr;

    ID3D11Buffer* DX_WVPBuffer = nullptr;

    ID3D11Buffer* DX_VertexBufferTriangle = nullptr;
    ID3D11Buffer* DX_IndexBufferTriangle = nullptr;

    ID3D11Texture2D* DX_DebugTexture = nullptr;
    ID3D11ShaderResourceView* DX_DebugTextureSRV = nullptr;
    ID3D11SamplerState* DX_DefaultSamplerState = nullptr;
    ID3D11Buffer* DX_VertexBufferQuad = nullptr;
    ID3D11Buffer* DX_IndexBufferQuad = nullptr;

    D3D_FEATURE_LEVEL UsedFeatureLevel;

    struct v2f
    {
        float X;
        float Y;
    };

    struct v3f
    {
        float X;
        float Y;
        float Z;
    };

    struct v4f
    {
        float X;
        float Y;
        float Z;
        float W;
    };

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

    struct m4f
    {
        v4f R0;
        v4f R1;
        v4f R2;
        v4f R3;
    };
    struct WVPData
    {
        m4f World;
        m4f View;
        m4f Proj;
    };

    struct RGBA32
    {
        unsigned char R;
        unsigned char G;
        unsigned char B;
        unsigned char A;
    };
    struct ImageT
    {
        unsigned int Width;
        unsigned int Height;
        size_t PxCount;
        size_t PxBytes;
        RGBA32* PxBuffer;
    };
    void GetDebugImage(ImageT& OutImage)
    {
        unsigned int DebugImageLength = 16;
        OutImage.Width = DebugImageLength;
        OutImage.Height = DebugImageLength;
        OutImage.PxCount = OutImage.Width * OutImage.Height;
        OutImage.PxBytes = sizeof(RGBA32) * OutImage.PxCount;
        OutImage.PxBuffer = new RGBA32[OutImage.PxCount];

        const RGBA32 Pink{ 255u, 73u, 173u, 255u };
        const RGBA32 Black{ 0u, 0u, 0u, 255u };
        const RGBA32 Red{ 255u, 0u, 0u, 255u };
        const RGBA32 Green{ 0u, 255u, 0u, 255u };
        const RGBA32 Blue{ 0u, 0u, 255u, 255u };
        const RGBA32 White{ 255u, 255u, 255u, 255u };
        for (int PxIdx = 0; PxIdx < OutImage.PxCount; PxIdx++)
        {
            int PxRow = PxIdx / OutImage.Width;
            int PxCol = PxIdx % OutImage.Width;
            if (PxRow == 0 && PxCol == 0)
            {
                OutImage.PxBuffer[PxIdx] = Red;
            }
            else if (PxRow == 0 && PxCol == (OutImage.Width - 1))
            {
                OutImage.PxBuffer[PxIdx] = Green;
            }
            else if (PxRow == (OutImage.Height - 1) && PxCol == 0)
            {
                OutImage.PxBuffer[PxIdx] = Blue;
            }
            else if (PxRow == (OutImage.Height - 1) && PxCol == (OutImage.Width - 1))
            {
                OutImage.PxBuffer[PxIdx] = White;
            }
            else
            {
                bool bEvenCell = (PxRow + PxCol) % 2 == 0;
                OutImage.PxBuffer[PxIdx] = bEvenCell ? Black : Pink;
            }
        }
    }

    int CompileShaderHelper
    (
        LPCWSTR SourceFileName,
        LPCSTR EntryPointFunction,
        LPCSTR ShaderProfile,
        ID3DBlob** OutShaderBlob,
        const D3D_SHADER_MACRO* Defines
    )
    {
        if (SourceFileName == nullptr || EntryPointFunction == nullptr || ShaderProfile == nullptr || OutShaderBlob == nullptr)
        {
            return E_INVALIDARG;
        }
        
        HRESULT Result = S_OK;

        UINT ShaderCompileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    #if _DEBUG
        ShaderCompileFlags |= D3DCOMPILE_DEBUG;
        ShaderCompileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
    #endif // _DEBUG

        ID3DBlob* ShaderBlob = nullptr;
        ID3DBlob* ErrorBlob = nullptr;

        Result = D3DCompileFromFile
        (
            SourceFileName,
            Defines,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            EntryPointFunction,
            ShaderProfile,
            ShaderCompileFlags,
            0,
            &ShaderBlob,
            &ErrorBlob
        );

        if (FAILED(Result) && ShaderBlob)
        {
            ShaderBlob->Release();
            ShaderBlob = nullptr;
        }
        if (ErrorBlob)
        {
            OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
            ErrorBlob->Release();
        }

        *OutShaderBlob = ShaderBlob;

        return Result;
    }

    void InitGraphics()
    {
        D3D_FEATURE_LEVEL SupportedFeatureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };
        UINT NumSupportedFeatureLevels = ARRAY_SIZE(SupportedFeatureLevels);

        CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)&DXGI_Factory2);

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

        DX_CHECK(DXGI_SwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&DX_Backbuffer));

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
            ID3DBlob* VSBlob = nullptr;
            ID3DBlob* PSBlob = nullptr;

            const D3D_SHADER_MACRO DefinesVxColor[] =
            {
                "ENABLE_VERTEX_COLOR", "1",
                "ENABLE_VERTEX_TEXTURE", "0",
                nullptr, nullptr
            };
            DX_CHECK(CompileShaderHelper(L"src/hlsl/BaseShader.hlsl", "VSMain", "vs_5_0", &VSBlob, DefinesVxColor));
            DX_CHECK(CompileShaderHelper(L"src/hlsl/BaseShader.hlsl", "PSMain", "ps_5_0", &PSBlob, DefinesVxColor));

            if (VSBlob && PSBlob)
            {
                DX_CHECK(DX_Device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &DX_VertexShaderColor));
                DX_CHECK(DX_Device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &DX_PixelShaderColor));

                D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
                {
                    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                };
                UINT NumInputElements = ARRAY_SIZE(InputLayoutDesc);
                DX_CHECK(DX_Device->CreateInputLayout(InputLayoutDesc, NumInputElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &DX_InputLayoutColor));
            }
            DX_SAFE_RELEASE(VSBlob);
            DX_SAFE_RELEASE(PSBlob);
        }

        {
            ID3DBlob* VSBlob = nullptr;
            ID3DBlob* PSBlob = nullptr;

            const D3D_SHADER_MACRO DefinesVxTexture[] =
            {
                "ENABLE_VERTEX_COLOR", "0",
                "ENABLE_VERTEX_TEXTURE", "1",
                nullptr, nullptr
            };
            DX_CHECK(CompileShaderHelper(L"src/hlsl/BaseShader.hlsl", "VSMain", "vs_5_0", &VSBlob, DefinesVxTexture));
            DX_CHECK(CompileShaderHelper(L"src/hlsl/BaseShader.hlsl", "PSMain", "ps_5_0", &PSBlob, DefinesVxTexture));

            if (VSBlob && PSBlob)
            {
                DX_CHECK(DX_Device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &DX_VertexShaderTexture));
                DX_CHECK(DX_Device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &DX_PixelShaderTexture));

                D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
                {
                    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                };
                UINT NumInputElements = ARRAY_SIZE(InputLayoutDesc);
                DX_CHECK(DX_Device->CreateInputLayout(InputLayoutDesc, NumInputElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &DX_InputLayoutTexture));
            }
            DX_SAFE_RELEASE(VSBlob);
            DX_SAFE_RELEASE(PSBlob);
        }

        D3D11_BUFFER_DESC WVPBufferDesc = {};
        WVPBufferDesc.ByteWidth = sizeof(WVPData);
        WVPBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        WVPBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        WVPBufferDesc.CPUAccessFlags = 0;
        DX_CHECK(DX_Device->CreateBuffer(&WVPBufferDesc, nullptr, &DX_WVPBuffer));

        {
            D3D11_BUFFER_DESC VertexBufferDesc = { sizeof(Vertices_Triangle), D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0 };
            D3D11_SUBRESOURCE_DATA VertexBufferInitData = { Vertices_Triangle, 0, 0 };
            DX_CHECK(DX_Device->CreateBuffer(&VertexBufferDesc, &VertexBufferInitData, &DX_VertexBufferTriangle));

            D3D11_BUFFER_DESC IndexBufferDesc = { sizeof(Indices_Triangle), D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0 };
            D3D11_SUBRESOURCE_DATA IndexBufferInitData = { Indices_Triangle, 0, 0 };
            DX_CHECK(DX_Device->CreateBuffer(&IndexBufferDesc, &IndexBufferInitData, &DX_IndexBufferTriangle));
        }

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

        {
            D3D11_BUFFER_DESC VertexBufferDesc = { sizeof(Vertices_Quad), D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0 };
            D3D11_SUBRESOURCE_DATA VertexBufferInitData = { Vertices_Quad, 0, 0 };
            DX_CHECK(DX_Device->CreateBuffer(&VertexBufferDesc, &VertexBufferInitData, &DX_VertexBufferQuad));

            D3D11_BUFFER_DESC IndexBufferDesc = { sizeof(Indices_Quad), D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0 };
            D3D11_SUBRESOURCE_DATA IndexBufferInitData = { Indices_Quad, 0, 0 };
            DX_CHECK(DX_Device->CreateBuffer(&IndexBufferDesc, &IndexBufferInitData, &DX_IndexBufferQuad));
        }

        {

        }
    }

    void UpdateAndDraw()
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
        WVPData WVP = { IdentityMatrix, IdentityMatrix, IdentityMatrix };

        const int WVPBufferSlot = 0;
        DX_ImmediateContext->UpdateSubresource(DX_WVPBuffer, 0, nullptr, &WVP, sizeof(WVPData), 0);

        { // Draw triangle
            UINT Offset = 0;
            const UINT Stride = sizeof(VxColor);
            DX_ImmediateContext->IASetInputLayout(DX_InputLayoutColor);
            DX_ImmediateContext->IASetVertexBuffers(0, 1, &DX_VertexBufferTriangle, &Stride, &Offset);
            DX_ImmediateContext->IASetIndexBuffer(DX_IndexBufferTriangle, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DX_VertexShaderColor, nullptr, 0);
            DX_ImmediateContext->PSSetShader(DX_PixelShaderColor, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WVPBufferSlot, 1, &DX_WVPBuffer);

            DX_ImmediateContext->DrawIndexed(ARRAY_SIZE(Indices_Triangle), 0u, 0u);
        }

        { // Draw tex quad
            UINT Offset = 0;
            const UINT Stride = sizeof(VxTexture);
            DX_ImmediateContext->IASetInputLayout(DX_InputLayoutTexture);
            DX_ImmediateContext->IASetVertexBuffers(0, 1, &DX_VertexBufferQuad, &Stride, &Offset);
            DX_ImmediateContext->IASetIndexBuffer(DX_IndexBufferQuad, DXGI_FORMAT_R32_UINT, 0);
            DX_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            DX_ImmediateContext->VSSetShader(DX_VertexShaderTexture, nullptr, 0);
            DX_ImmediateContext->VSSetConstantBuffers(WVPBufferSlot, 1, &DX_WVPBuffer);
            DX_ImmediateContext->PSSetShader(DX_PixelShaderTexture, nullptr, 0);
            DX_ImmediateContext->PSSetShaderResources(0, 1, &DX_DebugTextureSRV);
            DX_ImmediateContext->PSSetSamplers(0, 1, &DX_DefaultSamplerState);

            DX_ImmediateContext->DrawIndexed(ARRAY_SIZE(Indices_Quad), 0u, 0u);
        }

        UINT SyncInterval = 0;
        UINT PresentFlags = 0;
        DXGI_PRESENT_PARAMETERS PresentParams = {};
        DXGI_SwapChain1->Present1(SyncInterval, PresentFlags, &PresentParams);
    }

    void TermGraphics()
    {
        DX_SAFE_RELEASE(DX_VertexBufferTriangle);
        DX_SAFE_RELEASE(DX_IndexBufferTriangle);

        DX_SAFE_RELEASE(DX_WVPBuffer);

        DX_SAFE_RELEASE(DX_VertexShaderColor);
        DX_SAFE_RELEASE(DX_PixelShaderColor);
        DX_SAFE_RELEASE(DX_InputLayoutColor);

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
            DX_Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&DX_Debug);
            if (DX_Debug) { DX_Debug->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL); }
            DX_SAFE_RELEASE(DX_Debug);
        }

        DX_SAFE_RELEASE(DX_Device);
    }
}