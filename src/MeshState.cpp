#include "Graphics.h" // MeshState.h included

namespace Leviathan
{

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

constexpr float fUnit = 1.0f;
VxColor Vertices_Cube[] =
{
    {{-fUnit, +fUnit, +fUnit, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 0
    {{+fUnit, +fUnit, +fUnit, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // 1
    {{-fUnit, -fUnit, +fUnit, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // 2
    {{+fUnit, -fUnit, +fUnit, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 3

    {{-fUnit, +fUnit, -fUnit, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // 4
    {{+fUnit, +fUnit, -fUnit, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // 5
    {{-fUnit, -fUnit, -fUnit, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // 6
    {{+fUnit, -fUnit, -fUnit, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, // 7
};
VxMin Vertices_CubeMin[] =
{
    {-fUnit, +fUnit, +fUnit, 1.0f}, // 0
    {+fUnit, +fUnit, +fUnit, 1.0f}, // 1
    {-fUnit, -fUnit, +fUnit, 1.0f}, // 2
    {+fUnit, -fUnit, +fUnit, 1.0f}, // 3

    {-fUnit, +fUnit, -fUnit, 1.0f}, // 4
    {+fUnit, +fUnit, -fUnit, 1.0f}, // 5
    {-fUnit, -fUnit, -fUnit, 1.0f}, // 6
    {+fUnit, -fUnit, -fUnit, 1.0f}, // 7
};
unsigned int Indices_Cube[] =
{
    // Front
    0, 2, 1,
    1, 2, 3,
    // Back
    5, 7, 4,
    4, 7, 6,
    // Top
    4, 0, 5,
    5, 0, 1,
    // Bottom
    2, 6, 3,
    3, 6, 7,
    // Left
    4, 6, 0,
    0, 6, 2,
    // Right
    1, 3, 5,
    5, 3, 7,
};
constexpr v4f Color_White{ 1.0f, 1.0f, 1.0f, 1.0f };
constexpr v4f Color_Yellow{ 1.0f, 1.0f, 0.0f, 1.0f };
constexpr v4f Color_Green{ 0.0f, 1.0f, 0.0f, 1.0f };
constexpr v4f Color_Blue{ 0.0f, 0.0f, 1.0f, 1.0f };
constexpr v4f Color_Orange{ 1.0f, 0.5f, 0.0f, 1.0f };
constexpr v4f Color_Red{ 1.0f, 0.0f, 0.0f, 1.0f };
VxColor Vertices_CubeFacesColor[] =
{
    // Front: 0, 2, 1, 3
    { {-fUnit, +fUnit, +fUnit, 1.0f}, Color_Green }, // 0
    { {+fUnit, +fUnit, +fUnit, 1.0f}, Color_Green }, // 1
    { {-fUnit, -fUnit, +fUnit, 1.0f}, Color_Green }, // 2
    { {+fUnit, -fUnit, +fUnit, 1.0f}, Color_Green }, // 3
    // Back: 5, 7, 4, 6
    { {+fUnit, +fUnit, -fUnit, 1.0f}, Color_Blue }, // 5
    { {-fUnit, +fUnit, -fUnit, 1.0f}, Color_Blue }, // 4
    { {+fUnit, -fUnit, -fUnit, 1.0f}, Color_Blue }, // 7
    { {-fUnit, -fUnit, -fUnit, 1.0f}, Color_Blue }, // 6
    // Top: 4, 0, 5, 1
    { {-fUnit, +fUnit, -fUnit, 1.0f}, Color_White }, // 4
    { {+fUnit, +fUnit, -fUnit, 1.0f}, Color_White }, // 5
    { {-fUnit, +fUnit, +fUnit, 1.0f}, Color_White }, // 0
    { {+fUnit, +fUnit, +fUnit, 1.0f}, Color_White }, // 1
    // Bottom: 2, 6, 3, 7
    { {-fUnit, -fUnit, +fUnit, 1.0f}, Color_Yellow }, // 2
    { {+fUnit, -fUnit, +fUnit, 1.0f}, Color_Yellow }, // 3
    { {-fUnit, -fUnit, -fUnit, 1.0f}, Color_Yellow }, // 6
    { {+fUnit, -fUnit, -fUnit, 1.0f}, Color_Yellow }, // 7
    // Left: 4, 6, 0, 2
    { {-fUnit, +fUnit, -fUnit, 1.0f}, Color_Orange }, // 4
    { {-fUnit, +fUnit, +fUnit, 1.0f}, Color_Orange }, // 0
    { {-fUnit, -fUnit, -fUnit, 1.0f}, Color_Orange }, // 6
    { {-fUnit, -fUnit, +fUnit, 1.0f}, Color_Orange }, // 2
    // Right: 1, 3, 5, 7
    { {+fUnit, +fUnit, +fUnit, 1.0f}, Color_Red }, // 1
    { {+fUnit, +fUnit, -fUnit, 1.0f}, Color_Red }, // 5
    { {+fUnit, -fUnit, +fUnit, 1.0f}, Color_Red }, // 3
    { {+fUnit, -fUnit, -fUnit, 1.0f}, Color_Red }, // 7
};
VxTexture Vertices_CubeFacesTex[] =
{
    // Front: 0, 2, 1, 3
    { {-fUnit, +fUnit, +fUnit, 1.0f}, {0.0f, 0.0f} }, // 0
    { {+fUnit, +fUnit, +fUnit, 1.0f}, {1.0f, 0.0f} }, // 1
    { {-fUnit, -fUnit, +fUnit, 1.0f}, {0.0f, 1.0f} }, // 2
    { {+fUnit, -fUnit, +fUnit, 1.0f}, {1.0f, 1.0f} }, // 3
    // Back: 5, 7, 4, 6
    { {+fUnit, +fUnit, -fUnit, 1.0f}, {0.0f, 0.0f} }, // 5
    { {-fUnit, +fUnit, -fUnit, 1.0f}, {1.0f, 0.0f} }, // 4
    { {+fUnit, -fUnit, -fUnit, 1.0f}, {0.0f, 1.0f} }, // 7
    { {-fUnit, -fUnit, -fUnit, 1.0f}, {1.0f, 1.0f} }, // 6
    // Top: 4, 0, 5, 1
    { {-fUnit, +fUnit, -fUnit, 1.0f}, {0.0f, 0.0f} }, // 4
    { {+fUnit, +fUnit, -fUnit, 1.0f}, {1.0f, 0.0f} }, // 5
    { {-fUnit, +fUnit, +fUnit, 1.0f}, {0.0f, 1.0f} }, // 0
    { {+fUnit, +fUnit, +fUnit, 1.0f}, {1.0f, 1.0f} }, // 1
    // Bottom: 2, 6, 3, 7
    { {-fUnit, -fUnit, +fUnit, 1.0f}, {0.0f, 0.0f} }, // 2
    { {+fUnit, -fUnit, +fUnit, 1.0f}, {1.0f, 0.0f} }, // 3
    { {-fUnit, -fUnit, -fUnit, 1.0f}, {0.0f, 1.0f} }, // 6
    { {+fUnit, -fUnit, -fUnit, 1.0f}, {1.0f, 1.0f} }, // 7
    // Left: 4, 6, 0, 2
    { {-fUnit, +fUnit, -fUnit, 1.0f}, {0.0f, 0.0f} }, // 4
    { {-fUnit, +fUnit, +fUnit, 1.0f}, {1.0f, 0.0f} }, // 0
    { {-fUnit, -fUnit, -fUnit, 1.0f}, {0.0f, 1.0f} }, // 6
    { {-fUnit, -fUnit, +fUnit, 1.0f}, {1.0f, 1.0f} }, // 2
    // Right: 1, 3, 5, 7
    { {+fUnit, +fUnit, +fUnit, 1.0f}, {0.0f, 0.0f} }, // 1
    { {+fUnit, +fUnit, -fUnit, 1.0f}, {1.0f, 0.0f} }, // 5
    { {+fUnit, -fUnit, +fUnit, 1.0f}, {0.0f, 1.0f} }, // 3
    { {+fUnit, -fUnit, -fUnit, 1.0f}, {1.0f, 1.0f} }, // 7
};
unsigned int Indices_CubeFaces[] =
{
    // Front
    0, 2, 1,
    1, 2, 3,
    // Back
    0 + 4, 2 + 4, 1 + 4,
    1 + 4, 2 + 4, 3 + 4,
    // Top
    0 + 8, 2 + 8, 1 + 8,
    1 + 8, 2 + 8, 3 + 8,
    // Bottom
    0 + 12, 2 + 12, 1 + 12,
    1 + 12, 2 + 12, 3 + 12,
    // Left
    0 + 16, 2 + 16, 1 + 16,
    1 + 16, 2 + 16, 3 + 16,
    // Right
    0 + 20, 2 + 20, 1 + 20,
    1 + 20, 2 + 20, 3 + 20,
};

VxTexture Vertices_Rect[] =
{
    { {-0.5f, +0.5f, +0.5f, +1.0f}, { 0.0f, 0.0f } },
    { {+0.5f, +0.5f, +0.5f, +1.0f}, { 1.0f, 0.0f } },
    { {-0.5f, -0.5f, +0.5f, +1.0f}, { 0.0f, 1.0f } },
    { {+0.5f, -0.5f, +0.5f, +1.0f}, { 1.0f, 1.0f } },
};
VxMin Vertices_PlatonicRect[] =
{
    { { 0.0f, 1.0f, +0.5f, +1.0f } },
    { { 1.0f, 1.0f, +0.5f, +1.0f } },
    { { 0.0f, 0.0f, +0.5f, +1.0f } },
    { { 1.0f, 0.0f, +0.5f, +1.0f } },
};
VxMin Vertices_RotationRect[] =
{
    {-0.5f, +0.5f, +0.5f, +1.0f},
    {+0.5f, +0.5f, +0.5f, +1.0f},
    {-0.5f, -0.5f, +0.5f, +1.0f},
    {+0.5f, -0.5f, +0.5f, +1.0f},
};
unsigned int Indices_Rect[] =
{
    0, 2, 1,
    1, 2, 3
};

MeshStateT CreateMeshState
(
    ID3D11Device* InDevice,
    size_t VertexSize,
    size_t NumVertices,
    void* VertexData,
    size_t NumIndices,
    unsigned int* IndexData,
    D3D_PRIMITIVE_TOPOLOGY Format
)
{
    ASSERT(VertexData);

    MeshStateT Result;

    Result.VertexSize = VertexSize;
    Result.NumVerts = NumVertices;
    Result.Format = Format;
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

MeshInstStateT CreateMeshInstState
(
    ID3D11Device* InDevice,
    size_t VertexSize,
    size_t InstDataSize,
    size_t MaxInstCount,
    size_t NumVertices,
    void* VertexData,
    size_t NumIndices,
    unsigned int* IndexData,
    D3D_PRIMITIVE_TOPOLOGY Format
)
{
    ASSERT(VertexSize);
    ASSERT(InstDataSize);
    ASSERT(MaxInstCount);
    ASSERT(NumVertices);
    ASSERT(VertexData);

    MeshInstStateT Result;

    Result.VertexSize = VertexSize;
    Result.NumVerts = NumVertices;
    Result.NumInds = NumIndices;
    Result.InstDataSize = InstDataSize;
    Result.MaxInstCount = MaxInstCount;
    Result.InstBufferSize = InstDataSize * MaxInstCount;
    Result.Format = Format;

    size_t VxDataSize = VertexSize * NumVertices;
    D3D11_BUFFER_DESC VertexBufferDesc = { VxDataSize, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0 };
    D3D11_SUBRESOURCE_DATA VertexBufferInitData = { VertexData, 0, 0 };
    DX_CHECK(InDevice->CreateBuffer(&VertexBufferDesc, &VertexBufferInitData, &Result.VxBuffer));

    D3D11_BUFFER_DESC InstBufferDesc = { Result.InstBufferSize, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, 0 };
    DX_CHECK(InDevice->CreateBuffer(&InstBufferDesc, nullptr, &Result.InstBuffer));

    if (IndexData)
    {
        size_t IxDataSize = NumIndices * sizeof(unsigned int);
        D3D11_BUFFER_DESC IndexBufferDesc = { IxDataSize, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0 };
        D3D11_SUBRESOURCE_DATA IndexBufferInitData = { IndexData, 0, 0 };
        DX_CHECK(InDevice->CreateBuffer(&IndexBufferDesc, &IndexBufferInitData, &Result.IxBuffer));
    }

    return Result;
}

MeshStateT LoadMeshStateTriangle()
{
    return CreateMeshState
    (
        Graphics::Device(),
        sizeof(VxColor),
        ARRAY_SIZE(Vertices_Triangle),
        Vertices_Triangle,
        ARRAY_SIZE(Indices_Triangle),
        Indices_Triangle
    );
}
MeshStateT LoadMeshStateRect()
{
    return CreateMeshState
    (
        Graphics::Device(),
        sizeof(VxTexture),
        ARRAY_SIZE(Vertices_Rect),
        Vertices_Rect,
        ARRAY_SIZE(Indices_Rect),
        Indices_Rect
    );
}
MeshStateT LoadMeshStateCube()
{
    return CreateMeshState
    (
        Graphics::Device(),
        sizeof(VxColor),
        ARRAY_SIZE(Vertices_Cube),
        Vertices_Cube,
        ARRAY_SIZE(Indices_Cube),
        Indices_Cube
    );
}
MeshStateT LoadMeshStateCubeFacesTex()
{
    return CreateMeshState
    (
        Graphics::Device(),
        sizeof(VxTexture),
        ARRAY_SIZE(Vertices_CubeFacesTex),
        Vertices_CubeFacesTex,
        ARRAY_SIZE(Indices_CubeFaces),
        Indices_CubeFaces
    );
}

MeshStateT LoadMeshStateUnitCircle()
{
    static constexpr int NumPoints = 32;

    int NumVerts = NumPoints + 1;
    int NumTris = NumPoints;
    int NumInds = NumTris * 3;

    VxColor* Vertices_Circle = new VxColor[NumVerts];
    unsigned int* Indices_Circle = new unsigned int[NumInds];

    for (int TriIdx = 0; TriIdx < NumTris; TriIdx++)
    {
        Indices_Circle[(TriIdx * 3) + 0] = 0;
        if (TriIdx == NumTris - 1)
        {
            Indices_Circle[(TriIdx * 3) + 1] = TriIdx + 1;
            Indices_Circle[(TriIdx * 3) + 2] = 1;
        }
        else
        {
            Indices_Circle[(TriIdx * 3) + 1] = TriIdx + 1;
            Indices_Circle[(TriIdx * 3) + 2] = TriIdx + 2;
        }
    }

    constexpr float Default_Z = +0.5f;
    constexpr float Default_W = +1.0f;
    constexpr v4f Default_vColor{ 203.0f/255.0f, 166.0f/255.0f, 247.0f/255.0f, 1.0f };
    // NOTE(CKA): Same default color as InputVisualizer's FG

    for (int VxIdx = 0; VxIdx < NumVerts; VxIdx++)
    {
        Vertices_Circle[VxIdx].Pos = { 0.0f, 0.0f, Default_Z, Default_W };
        Vertices_Circle[VxIdx].Col = Default_vColor;
    }

    for (int PointIdx = 0; PointIdx < NumPoints; PointIdx++)
    {
        float CurrAngle = ((float)PointIdx / (float)NumPoints) * fTAU;
        float X = cosf(CurrAngle);
        float Y = sinf(CurrAngle);
        
        // NOTE(CKA): First vertex is 0 (center of circle)
        Vertices_Circle[PointIdx + 1].Pos.X = X;
        Vertices_Circle[PointIdx + 1].Pos.Y = Y;
    };

    MeshStateT CircleMeshState = CreateMeshState(
        Graphics::Device(),
        sizeof(VxColor),
        NumVerts,
        Vertices_Circle,
        NumInds,
        Indices_Circle);

    delete[] Vertices_Circle;
    delete[] Indices_Circle;

    return CircleMeshState;
}

MeshStateT LoadMeshStateCubeFacesColor()
{
    return CreateMeshState
    (
        Graphics::Device(),
        sizeof(VxColor),
        ARRAY_SIZE(Vertices_CubeFacesColor),
        Vertices_CubeFacesColor,
        ARRAY_SIZE(Indices_CubeFaces),
        Indices_CubeFaces
    );
}

MeshInstStateT LoadMeshInstStateVoxel()
{
    return CreateMeshInstState
    (
        Graphics::Device(),
        sizeof(VxMin),
        sizeof(InstVoxelColorData),
        BatchDrawCmds::DefaultSize,
        ARRAY_SIZE(Vertices_CubeMin),
        Vertices_CubeMin,
        ARRAY_SIZE(Indices_Cube),
        Indices_Cube
    );
}

MeshInstStateT LoadMeshInstStateRect()
{
    return CreateMeshInstState
    (
        Graphics::Device(),
        sizeof(VxMin),
        sizeof(InstRectColorData),
        BatchDrawCmds::DefaultSize,
        ARRAY_SIZE(Vertices_PlatonicRect),
        Vertices_PlatonicRect,
        ARRAY_SIZE(Indices_Rect),
        Indices_Rect
    );
}

MeshInstStateT LoadMeshInstStateRectRotation()
{
    return CreateMeshInstState
    (
        Graphics::Device(),
        sizeof(VxMin),
        sizeof(InstRectColorRotationData),
        BatchDrawCmds::DefaultSize,
        ARRAY_SIZE(Vertices_RotationRect),
        Vertices_RotationRect,
        ARRAY_SIZE(Indices_Rect),
        Indices_Rect,
        D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );
}

}
