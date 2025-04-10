#include "Graphics.h"
#include "Image.h"

namespace Leviathan
{

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

    static const char* VxShaderMain = "VSMain";
    static const char* VxShaderProfile = "vs_5_0";
    static const char* PxShaderMain = "PSMain";
    static const char* PxShaderProfile = "ps_5_0";
    DX_CHECK(CompileShaderHelper(ShaderFilename, VxShaderMain, VxShaderProfile, &VSBlob, Defines));
    DX_CHECK(CompileShaderHelper(ShaderFilename, PxShaderMain, PxShaderProfile, &PSBlob, Defines));

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

void UpdateShaderResource
(
    ID3D11DeviceContext* Context,
    ID3D11Resource* Resource,
    void* pData,
    size_t DataSize
)
{
    ASSERT(pData);
    ASSERT(DataSize);
    constexpr UINT Subresource = 0;
    Context->UpdateSubresource(Resource, Subresource, nullptr, pData, (UINT)DataSize, 0);
}


void SetVSState(ID3D11DeviceContext* Context, PipelineStateT& Pipeline)
{
    ASSERT(Context);
    constexpr UINT StartSlot = 0;
    if (Pipeline.ConstantBuffers.Num > 0)
    {
        ASSERT(Pipeline.ConstantBuffers.Num <= PipelineStateT::MaxConstantBuffers);
        Context->VSSetConstantBuffers(StartSlot, Pipeline.ConstantBuffers.Num, Pipeline.ConstantBuffers.Data);
    }
    if (Pipeline.Samplers.Num > 0)
    {
        ASSERT(Pipeline.Samplers.Num <= PipelineStateT::MaxSamplers);
        Context->VSSetSamplers(StartSlot, Pipeline.Samplers.Num, Pipeline.Samplers.Data);
    }
    if (Pipeline.SRVs.Num > 0)
    {
        ASSERT(Pipeline.SRVs.Num <= PipelineStateT::MaxSRVs);
        Context->VSSetShaderResources(StartSlot, Pipeline.SRVs.Num, Pipeline.SRVs.Data);
    }
}

void SetShaderConstantBuffers(ID3D11DeviceContext* Context, uint NumConstantBuffers, ID3D11Buffer** ConstantBuffers, uint StartSlot)
{
    if (NumConstantBuffers > 0)
    {
        ASSERT(ConstantBuffers);
        ASSERT(NumConstantBuffers <= PipelineStateT::MaxConstantBuffers);
        Context->VSSetConstantBuffers(StartSlot, NumConstantBuffers, ConstantBuffers);
        Context->PSSetConstantBuffers(StartSlot, NumConstantBuffers, ConstantBuffers);
    }
}
void SetShaderSamplers(ID3D11DeviceContext* Context, uint NumSamplers, ID3D11SamplerState** Samplers, uint StartSlot)
{
    if (NumSamplers > 0)
    {
        ASSERT(Samplers);
        ASSERT(NumSamplers <= PipelineStateT::MaxSamplers);
        Context->VSSetSamplers(StartSlot, NumSamplers, Samplers);
        Context->PSSetSamplers(StartSlot, NumSamplers, Samplers);
    }
}
void SetShaderResourceViews(ID3D11DeviceContext* Context, uint NumSRVs, ID3D11ShaderResourceView** SRVs, uint StartSlot)
{
    if (NumSRVs > 0)
    {
        ASSERT(SRVs);
        ASSERT(NumSRVs <= PipelineStateT::MaxSRVs);
        Context->VSSetShaderResources(StartSlot, NumSRVs, SRVs);
        Context->PSSetShaderResources(StartSlot, NumSRVs, SRVs);
    }
}

void SetPSState(ID3D11DeviceContext* Context, PipelineStateT& Pipeline)
{
    ASSERT(Context);
    constexpr UINT StartSlot = 0;
    if (Pipeline.ConstantBuffers.Num > 0)
    {
        ASSERT(Pipeline.ConstantBuffers.Num <= PipelineStateT::MaxConstantBuffers);
        Context->PSSetConstantBuffers(StartSlot, Pipeline.ConstantBuffers.Num, Pipeline.ConstantBuffers.Data);
    }
    if (Pipeline.Samplers.Num > 0)
    {
        ASSERT(Pipeline.Samplers.Num <= PipelineStateT::MaxSamplers);
        Context->PSSetSamplers(StartSlot, Pipeline.Samplers.Num, Pipeline.Samplers.Data);
    }
    if (Pipeline.SRVs.Num > 0)
    {
        ASSERT(Pipeline.SRVs.Num <= PipelineStateT::MaxSRVs);
        Context->PSSetShaderResources(StartSlot, Pipeline.SRVs.Num, Pipeline.SRVs.Data);
    }
}

void DrawMesh(ID3D11DeviceContext* Context, DrawStateT& PipelineState, MeshStateT& Mesh)
{
    ASSERT(Context);

    UINT VxStride = Mesh.VertexSize;
    UINT VxOffset = 0;
    Context->IASetInputLayout(PipelineState.InputLayout);
    Context->IASetVertexBuffers(0, 1, &Mesh.VxBuffer, &VxStride, &VxOffset);
    if (Mesh.IxBuffer) { Context->IASetIndexBuffer(Mesh.IxBuffer, MeshStateT::IxFormat, 0); }
    Context->IASetPrimitiveTopology(MeshStateT::FormatTriangleList);

    Context->VSSetShader(PipelineState.VertexShader, nullptr, 0);
    Context->PSSetShader(PipelineState.PixelShader, nullptr, 0);

    if (Mesh.IxBuffer)
    {
        constexpr UINT StartIdx = 0;
        constexpr UINT StartVx = 0;
        Context->DrawIndexed(Mesh.NumInds, StartIdx, StartVx);
    }
    else
    {
        constexpr UINT StartVx = 0;
        Context->Draw(Mesh.NumVerts, StartVx);
    }
}
void DrawMeshIxRange(ID3D11DeviceContext* Context, DrawStateT& PipelineState, MeshStateT& Mesh, int StartIx, int NumIx)
{
    ASSERT(Context);
    ASSERT(Mesh.IxBuffer);
    ASSERT(0 <= StartIx && StartIx < Mesh.NumInds);
    ASSERT(StartIx % 3 == 0 && NumIx % 3 == 0);
    ASSERT(StartIx + NumIx <= Mesh.NumInds);

    UINT VxStride = Mesh.VertexSize;
    UINT VxOffset = 0;
    Context->IASetInputLayout(PipelineState.InputLayout);
    Context->IASetVertexBuffers(0, 1, &Mesh.VxBuffer, &VxStride, &VxOffset);
    if (Mesh.IxBuffer) { Context->IASetIndexBuffer(Mesh.IxBuffer, MeshStateT::IxFormat, 0); }
    Context->IASetPrimitiveTopology(MeshStateT::FormatTriangleList);

    Context->VSSetShader(PipelineState.VertexShader, nullptr, 0);
    Context->PSSetShader(PipelineState.PixelShader, nullptr, 0);

    if (Mesh.IxBuffer)
    {
        constexpr UINT StartVx = 0;
        Context->DrawIndexed(NumIx, StartIx, StartVx);
    }
}

void DrawMeshInst(ID3D11DeviceContext* Context, MeshInstStateT& MeshInst, size_t NumInsts)
{
    ASSERT(NumInsts * MeshInst.InstDataSize < MeshInst.InstBufferSize);
    constexpr UINT StartIdx = 0;
    constexpr UINT StartVx = 0;
    constexpr UINT StartInst = 0;
    if (MeshInst.IxBuffer) { Context->DrawIndexedInstanced(MeshInst.NumInds, NumInsts, StartIdx, StartVx, StartInst); }
    else { Context->DrawInstanced(MeshInst.NumVerts, NumInsts, StartVx, StartInst); }
}
void DrawMeshInstanced
(
    ID3D11DeviceContext* Context,
    DrawStateT& PipelineState,
    MeshInstStateT& MeshInst,
    size_t NumInsts,
    void* pInstData
)
{
    ASSERT(Context);
    ASSERT(MeshInst.VxBuffer);
    ASSERT(MeshInst.InstBuffer);
    ASSERT(NumInsts > 0);

    ID3D11Buffer* VxInstBuffers[] = { MeshInst.VxBuffer, MeshInst.InstBuffer };
    UINT VxInstStrides[] = { MeshInst.VertexSize, MeshInst.InstDataSize };
    UINT VxInstOffsets[] = { 0, 0 };
    Context->IASetInputLayout(PipelineState.InputLayout);
    Context->IASetVertexBuffers(0, 2, VxInstBuffers, VxInstStrides, VxInstOffsets);
    if (MeshInst.IxBuffer) { Context->IASetIndexBuffer(MeshInst.IxBuffer, MeshStateT::IxFormat, 0); }
    Context->IASetPrimitiveTopology(MeshInst.Format);

    Context->VSSetShader(PipelineState.VertexShader, nullptr, 0);
    Context->PSSetShader(PipelineState.PixelShader, nullptr, 0);

    if (pInstData)
    {
        ASSERT(MeshInst.MaxInstCount * MeshInst.InstDataSize == MeshInst.InstBufferSize);
        size_t NumInstPerCall = MeshInst.MaxInstCount;
        u8* InstReadPtr = (u8*)pInstData;
        int NumRemainingInsts = NumInsts;
        while (NumRemainingInsts > 0)
        {
            size_t CurrDrawNumInsts = Min(NumRemainingInsts, NumInstPerCall);
            { // Map instance data
                D3D11_MAPPED_SUBRESOURCE InstDataMapWrite = {};
                DX_CHECK(Context->Map(MeshInst.InstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &InstDataMapWrite));
                memcpy(InstDataMapWrite.pData, InstReadPtr, CurrDrawNumInsts * MeshInst.InstDataSize);
                Context->Unmap(MeshInst.InstBuffer, 0);
            }

            // Draw instances
            DrawMeshInst(Context, MeshInst, CurrDrawNumInsts);

            InstReadPtr += CurrDrawNumInsts * MeshInst.InstDataSize;
            NumRemainingInsts -= CurrDrawNumInsts;
        }
    }
    else
    {
        DrawMeshInst(Context, MeshInst, NumInsts);
    }
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

RectF* InitRectUVs(int Rows, int Cols)
{
    ASSERT(Rows > 0);
    ASSERT(Cols > 0);
    int RectCount = Rows * Cols;
    RectF* RectUVs = new RectF[RectCount];
    float RectHeight = 1.0f / Rows;
    float RectWidth = 1.0f / Cols;
    int RectIdx = 0;
    for (int RowIdx = 0; RowIdx < Rows; RowIdx++)
    {
        for (int ColIdx = 0; ColIdx < Cols; ColIdx++)
        {
            RectUVs[RectIdx] = RectF
            {
                RectWidth * ColIdx, RectHeight * RowIdx,
                RectWidth, RectHeight
            };
        }
    }
    return RectUVs;
}

LvSpriteSheet LoadSpriteSheet(const char* Filename, ID3D11Device* InDevice, int Rows, int Cols)
{
    LvSpriteSheet Result = {};

    Result.LvTex = LoadTextureBMP(Filename, InDevice);
    if (Result.LvTex.Texture)
    {
        Result.NumRows = Rows;
        Result.NumCols = Cols;
        Result.RectCount = Rows * Cols;
        Result.RectUVs = InitRectUVs(Rows, Cols);
    }

    return Result;
}

void SafeRelease(MeshStateT& InMeshState)
{
    SafeRelease(InMeshState.VxBuffer);
    SafeRelease(InMeshState.IxBuffer);
}

void SafeRelease(MeshInstStateT& InMeshInstState)
{
    SafeRelease(InMeshInstState.VxBuffer);
    SafeRelease(InMeshInstState.InstBuffer);
    SafeRelease(InMeshInstState.IxBuffer);
}

void SafeRelease(DrawStateT& InDrawState)
{
    SafeRelease(InDrawState.InputLayout);
    SafeRelease(InDrawState.VertexShader);
    SafeRelease(InDrawState.PixelShader);
}

void SafeRelease(LvTexture2D& InLvTex2D)
{
    SafeRelease(InLvTex2D.Texture);
    SafeRelease(InLvTex2D.SRV);
}

void SafeRelease(LvSpriteSheet& InSpriteSheet)
{
    SafeRelease(InSpriteSheet.LvTex);
    if (InSpriteSheet.RectUVs)
    {
        delete[] InSpriteSheet.RectUVs;
        InSpriteSheet.RectUVs = nullptr;
    }
}

}

