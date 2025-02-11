#include "DrawState.h"

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
        DX_SAFE_RELEASE(InMeshState.VxBuffer);
        DX_SAFE_RELEASE(InMeshState.IxBuffer);
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
        DX_SAFE_RELEASE(VSBlob);
        DX_SAFE_RELEASE(PSBlob);

        return Result;
    }

    void SafeRelease(DrawStateT& InDrawState)
    {
        DX_SAFE_RELEASE(InDrawState.InputLayout);
        DX_SAFE_RELEASE(InDrawState.VertexShader);
        DX_SAFE_RELEASE(InDrawState.PixelShader);
    }
}