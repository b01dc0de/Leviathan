#include "DrawState.h"

namespace Leviathan
{
    MeshState CreateMeshState
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

        MeshState Result;

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

    void SafeRelease(MeshState& InMeshState)
    {
        DX_SAFE_RELEASE(InMeshState.VxBuffer);
        DX_SAFE_RELEASE(InMeshState.IxBuffer);
    }

}