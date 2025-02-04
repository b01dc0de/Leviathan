#ifndef LEVIATHAN_DRAWSTATE_H
#define LEVIATHAN_DRAWSTATE_H

#include "Leviathan.h"

namespace Leviathan
{
    struct MeshState
    {
        size_t VertexSize = 0;
        size_t NumVerts = 0;
        size_t NumInds = 0;
        ID3D11Buffer* VxBuffer = nullptr;
        ID3D11Buffer* IxBuffer = nullptr;
    };

    MeshState CreateMeshState
    (
        ID3D11Device* InDevice,
        size_t VertexSize,
        size_t NumVertices,
        void* VertexData,
        size_t NumIndices,
        unsigned int* IndexData
    );
    void SafeRelease(MeshState& InMeshState);
}

#endif // LEVIATHAN_DRAWSTATE_H