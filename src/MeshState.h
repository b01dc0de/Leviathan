#ifndef LEVIATHAN_MESHSTATE_H
#define LEVIATHAN_MESHSTATE_H

#include "Leviathan.h"
#include "Draw2D.h"
#include "Math.h"

namespace Leviathan
{
    struct VxMin
    {
        v4f Pos;
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

    MeshStateT LoadMeshStateTriangle();
    MeshStateT LoadMeshStateRect();
    MeshStateT LoadMeshStateCube();
    MeshStateT LoadMeshStateCubeFacesColor();
    MeshStateT LoadMeshStateCubeFacesTex();
    MeshStateT LoadMeshStateSpeedCube(int* IxPerPiece);
    MeshInstStateT LoadMeshInstStateVoxel();
    MeshInstStateT LoadMeshInstStateRect();
    MeshInstStateT LoadMeshInstStateRectRotation();
}

#endif // LEVIATHAN_MESHSTATE_H

