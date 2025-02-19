#include "DrawState.h"
#include "Image.h"

namespace Leviathan
{
    MeshStateT CreateMeshState
    (
        ID3D11Device* InDevice,
        size_t VertexSize,
        size_t NumVertices,
        void* VertexData,
        size_t NumIndices,
        unsigned int* IndexData
    )
    {
        ASSERT(VertexData);

        MeshStateT Result;

        Result.VertexSize = VertexSize;
        Result.NumVerts = NumVertices;
        Result.NumInds = NumIndices;

        size_t VxDataSize = VertexSize * NumVertices;
        D3D11_BUFFER_DESC VertexBufferDesc = { VxDataSize, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0 };
        D3D11_SUBRESOURCE_DATA VertexBufferInitData = { VertexData, 0, 0 };
        DX_CHECK(InDevice->CreateBuffer(&VertexBufferDesc, &VertexBufferInitData, &Result.VxBuffer));

        if (IndexData)
        {
            size_t IxDataSize = NumIndices * sizeof(unsigned int);
            D3D11_BUFFER_DESC IndexBufferDesc = { IxDataSize, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0 };
            D3D11_SUBRESOURCE_DATA IndexBufferInitData = { IndexData, 0, 0 };
            DX_CHECK(InDevice->CreateBuffer(&IndexBufferDesc, &IndexBufferInitData, &Result.IxBuffer));
        }

        return Result;
    }

    void SafeRelease(MeshStateT& InMeshState)
    {
        SafeRelease(InMeshState.VxBuffer);
        SafeRelease(InMeshState.IxBuffer);
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

    DrawStateT CreateDrawState
    (
        ID3D11Device* InDevice,
        const wchar_t* ShaderFilename,
        const D3D_SHADER_MACRO* Defines,
        const D3D11_INPUT_ELEMENT_DESC* InputElements,
        size_t NumInputElements
    )
    {
        ASSERT(InDevice);
        ASSERT(ShaderFilename);
        ASSERT(InputElements);

        ID3DBlob* VSBlob = nullptr;
        ID3DBlob* PSBlob = nullptr;

        DX_CHECK(CompileShaderHelper(ShaderFilename, "VSMain", "vs_5_0", &VSBlob, Defines));
        DX_CHECK(CompileShaderHelper(ShaderFilename, "PSMain", "ps_5_0", &PSBlob, Defines));

        DrawStateT Result;

        if (VSBlob && PSBlob)
        {
            DX_CHECK(InDevice->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &Result.VertexShader));
            DX_CHECK(InDevice->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &Result.PixelShader));

            DX_CHECK(InDevice->CreateInputLayout(InputElements, NumInputElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &Result.InputLayout));
        }
        SafeRelease(VSBlob);
        SafeRelease(PSBlob);

        return Result;
    }

    void SafeRelease(DrawStateT& InDrawState)
    {
        SafeRelease(InDrawState.InputLayout);
        SafeRelease(InDrawState.VertexShader);
        SafeRelease(InDrawState.PixelShader);
    }

    LvTexture2D LoadTextureFromImage(ImageT& Image, ID3D11Device* InDevice)
    {
        LvTexture2D Result = {};

        if (Image.PxBuffer)
        {
            ID3D11Texture2D* OutTexture = nullptr;
            ID3D11ShaderResourceView* OutTextureSRV = nullptr;

            D3D11_SUBRESOURCE_DATA DebugTextureDataDesc[] = { {} };
            DebugTextureDataDesc[0].pSysMem = Image.PxBuffer;
            DebugTextureDataDesc[0].SysMemPitch = sizeof(RGBA32) * Image.Width;
            DebugTextureDataDesc[0].SysMemSlicePitch = Image.PxBufferSize;
            D3D11_TEXTURE2D_DESC DebugTextureDesc = {};
            DebugTextureDesc.Width = Image.Width;
            DebugTextureDesc.Height = Image.Height;
            DebugTextureDesc.MipLevels = 1;
            DebugTextureDesc.ArraySize = 1;
            DebugTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            DebugTextureDesc.SampleDesc.Count = 1;
            DebugTextureDesc.SampleDesc.Quality = 0;
            DebugTextureDesc.Usage = D3D11_USAGE_DEFAULT;
            DebugTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            DebugTextureDesc.CPUAccessFlags = 0;
            DebugTextureDesc.MiscFlags = 0;
            DX_CHECK(InDevice->CreateTexture2D(&DebugTextureDesc, &DebugTextureDataDesc[0], &OutTexture));
            DX_CHECK(InDevice->CreateShaderResourceView(OutTexture, nullptr, &OutTextureSRV));

            if (OutTexture && OutTextureSRV)
            {
                Result.Texture = OutTexture;
                Result.SRV = OutTextureSRV;
                Result.Width = Image.Width;
                Result.Height = Image.Height;
            }
            else
            {
                SafeRelease(OutTexture);
                SafeRelease(OutTextureSRV);
            }
        }

        return Result;
    }

    LvTexture2D LoadTextureBMP(const char* Filename, ID3D11Device* InDevice)
    {
        LvTexture2D Result = {};

        ImageT TextureImage = {};
        LoadBMPFile(Filename, TextureImage);

        if (TextureImage.PxBuffer)
        {
            Result = LoadTextureFromImage(TextureImage, InDevice);
            delete[] TextureImage.PxBuffer;
        }

        return Result;
    }

    void SafeRelease(LvTexture2D& InLvTex2D)
    {
        SafeRelease(InLvTex2D.Texture);
        SafeRelease(InLvTex2D.SRV);
    }
}