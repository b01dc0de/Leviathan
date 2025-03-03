#ifndef LEVIATHAN_DRAWSTATE_H
#define LEVIATHAN_DRAWSTATE_H

#include "Leviathan.h"
#include "Image.h"
#include "Draw2D.h"

namespace Leviathan
{
    struct MeshStateT
    {
        size_t VertexSize = 0;
        size_t NumVerts = 0;
        size_t NumInds = 0;
        D3D_PRIMITIVE_TOPOLOGY Format = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
        ID3D11Buffer* VxBuffer = nullptr;
        ID3D11Buffer* IxBuffer = nullptr;

        static constexpr DXGI_FORMAT IxFormat = DXGI_FORMAT_R32_UINT;
        static constexpr D3D_PRIMITIVE_TOPOLOGY FormatTriangleList = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        static constexpr D3D_PRIMITIVE_TOPOLOGY FormatLineList = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    };

    struct MeshInstStateT
    {
        size_t VertexSize = 0;
        size_t NumVerts = 0;
        size_t NumInds = 0;
        size_t InstDataSize = 0;
        size_t MaxInstCount = 0;
        size_t InstBufferSize = 0;
        D3D_PRIMITIVE_TOPOLOGY Format = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
        ID3D11Buffer* VxBuffer = nullptr;
        ID3D11Buffer* InstBuffer = nullptr;
        ID3D11Buffer* IxBuffer = nullptr;
    };

    MeshStateT CreateMeshState
    (
        ID3D11Device* InDevice,
        size_t VertexSize,
        size_t NumVertices,
        void* VertexData,
        size_t NumIndices = 0,
        unsigned int* IndexData = nullptr,
        D3D_PRIMITIVE_TOPOLOGY Format = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    MeshInstStateT CreateMeshInstState
    (
        ID3D11Device* InDevice,
        size_t VertexSize,
        size_t InstDataSize,
        size_t MaxInstCount,
        size_t NumVertices,
        void* VertexData,
        size_t NumIndices = 0,
        unsigned int* IndexData = nullptr,
        D3D_PRIMITIVE_TOPOLOGY Format = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

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
}

#endif // LEVIATHAN_DRAWSTATE_H

