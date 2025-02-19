#ifndef LEVIATHAN_DRAWSTATE_H
#define LEVIATHAN_DRAWSTATE_H

#include "Leviathan.h"
#include "Image.h"

namespace Leviathan
{
    struct MeshStateT
    {
        size_t VertexSize = 0;
        size_t NumVerts = 0;
        size_t NumInds = 0;
        ID3D11Buffer* VxBuffer = nullptr;
        ID3D11Buffer* IxBuffer = nullptr;
    };

    MeshStateT CreateMeshState
    (
        ID3D11Device* InDevice,
        size_t VertexSize,
        size_t NumVertices,
        void* VertexData,
        size_t NumIndices,
        unsigned int* IndexData
    );
    void SafeRelease(MeshStateT& InMeshState);

    struct DrawStateT
    {
        ID3D11InputLayout* InputLayout = nullptr;
        ID3D11VertexShader* VertexShader = nullptr;
        ID3D11PixelShader* PixelShader = nullptr;
    };

    DrawStateT CreateDrawState
    (
        ID3D11Device* InDevice,
        const wchar_t* ShaderFilename,
        const D3D_SHADER_MACRO* Defines,
        const D3D11_INPUT_ELEMENT_DESC* InputElements,
        size_t NumInputElements
    );
    void SafeRelease(DrawStateT& InDrawState);

    struct LvTexture2D
    {
        ID3D11Texture2D* Texture = nullptr;
        ID3D11ShaderResourceView* SRV = nullptr;
        int Width = 0;
        int Height = 0;
    };

    LvTexture2D LoadTextureFromImage(ImageT& Image, ID3D11Device* InDevice);
    LvTexture2D LoadTextureBMP(const char* Filename, ID3D11Device* InDevice);
    void SafeRelease(LvTexture2D& InLvTex2D);
}

#endif // LEVIATHAN_DRAWSTATE_H