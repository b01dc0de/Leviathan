#ifndef LEVIATHAN_DRAWSTATE_H
#define LEVIATHAN_DRAWSTATE_H

#include "Leviathan.h"
#include "Image.h"
#include "MeshState.h"

namespace Leviathan
{

struct DrawStateT
{
    ID3D11InputLayout* InputLayout = nullptr;
    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader* PixelShader = nullptr;
};

struct PipelineStateT
{
    Array<ID3D11Buffer*> ConstantBuffers;
    Array<ID3D11SamplerState*> Samplers;
    Array<ID3D11ShaderResourceView*> SRVs;

    static constexpr int MaxConstantBuffers = D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT;
    static constexpr int MaxSamplers = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT;
    static constexpr int MaxSRVs = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
};

void UpdateShaderResource(ID3D11DeviceContext* Context, ID3D11Resource* Resource, void* pData, size_t DataSize);
void SetVSState(ID3D11DeviceContext* Context, PipelineStateT& Pipeline);
void SetPSState(ID3D11DeviceContext* Context, PipelineStateT& Pipeline);
void SetShaderConstantBuffers(ID3D11DeviceContext* Context, uint NumConstantBuffers, ID3D11Buffer** ConstantBuffers, uint StartSlot = 0);
void SetShaderSamplers(ID3D11DeviceContext* Context, uint NumSamplers, ID3D11SamplerState** Samplers, uint StartSlot = 0);
void SetShaderResourceViews(ID3D11DeviceContext* Context, uint NumSRVs, ID3D11ShaderResourceView** SRVs, uint StartSlot = 0);

DrawStateT CreateDrawState
(
    ID3D11Device* InDevice,
    const wchar_t* ShaderFilename,
    const D3D_SHADER_MACRO* Defines,
    const D3D11_INPUT_ELEMENT_DESC* InputElements,
    size_t NumInputElements
);

void DrawMesh(ID3D11DeviceContext* Context, DrawStateT& PipelineState, MeshStateT& Mesh);
void DrawMeshIxRange(ID3D11DeviceContext* Context, DrawStateT& PipelineState, MeshStateT& Mesh, int StartIx, int NumIx);
void DrawMeshInstanced
(
    ID3D11DeviceContext* Context,
    DrawStateT& PipelineState,
    MeshInstStateT& MeshInst,
    size_t NumInsts,
    void* pInstData = nullptr
);

struct LvTexture2D
{
    ID3D11Texture2D* Texture = nullptr;
    ID3D11ShaderResourceView* SRV = nullptr;
    int Width = 0;
    int Height = 0;
};

LvTexture2D LoadTextureFromImage(ImageT& Image, ID3D11Device* InDevice);
LvTexture2D LoadTextureBMP(const char* Filename, ID3D11Device* InDevice);

struct LvSpriteSheet
{
    LvTexture2D LvTex;
    int NumRows = 0;
    int NumCols = 0;
    int RectCount = 0;
    RectF* RectUVs = nullptr;
};

LvSpriteSheet LoadSpriteSheet(const char* Filename, ID3D11Device* InDevice, int Rows, int Cols);

void SafeRelease(MeshStateT& InMeshState);
void SafeRelease(MeshInstStateT& InMeshInstState);
void SafeRelease(DrawStateT& InDrawState);
void SafeRelease(LvTexture2D& InLvTex2D);
void SafeRelease(LvSpriteSheet& InSpriteSheet);

} // namespace Leviathan

#endif // LEVIATHAN_DRAWSTATE_H

