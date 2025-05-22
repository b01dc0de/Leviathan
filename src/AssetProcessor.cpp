#include "AssetProcessor.h"

namespace Leviathan
{

namespace MeshOBJ
{

struct FaceIndices
{
    static constexpr int MaxVerts = 4;
    int NumVerts;
    int vIdx[MaxVerts];
    //int tIdx[MaxVerts];
    //int nIdx[MaxVerts];
};

struct ParsedOBJ
{
    Array<v4f> Vertices;
    //Array<v2f> TexUVs;
    //Array<v3f> Normals;
    Array<FaceIndices> Faces;
    int NumPrimitives = 0;
};

MeshStateT CreateMeshStateFromOBJ(ParsedOBJ& OBJ)
{
    static constexpr v4f DefaultColor{ 53.0f / 255.0f, 239.0f / 255.0f, 105.0f / 255.0f, 1.0f };

    ASSERT(OBJ.Vertices.Num > 0 && OBJ.Faces.Num > 0);
    if (OBJ.Vertices.Num == 0 || OBJ.Faces.Num == 0) { return MeshStateT{}; }

    // TODO(CKA): Implement support for texture (+ normal!) vertices
    int NumVerts = OBJ.Vertices.Num;
    int NumInds = OBJ.NumPrimitives * 3;
    VxColor* Verts = new VxColor[NumVerts];
    unsigned int* Inds = new unsigned int[NumInds];

    for (int vIdx = 0; vIdx < OBJ.Vertices.Num; vIdx++)
    {
        Verts[vIdx].Pos = OBJ.Vertices[vIdx];
        Verts[vIdx].Col = DefaultColor;
    }
    int IndIdx = 0;
    for (int fIdx = 0; fIdx < OBJ.Faces.Num; fIdx++)
    {
        ASSERT(OBJ.Faces[fIdx].NumVerts >= 3);
        int NumTris = OBJ.Faces[fIdx].NumVerts - 2;
        for (int TriIdx = 0; TriIdx < NumTris; TriIdx++)
        {
            ASSERT(IndIdx < NumInds);
            Inds[IndIdx + 0] = OBJ.Faces[fIdx].vIdx[0];
            Inds[IndIdx + 1] = OBJ.Faces[fIdx].vIdx[TriIdx + 1];
            Inds[IndIdx + 2] = OBJ.Faces[fIdx].vIdx[TriIdx + 2];
            IndIdx += 3;
        }
    }

    MeshStateT Result = CreateMeshState
    (
        Graphics::Device(),
        sizeof(VxColor), // TODO: Support multiple vertex types
        NumVerts,
        Verts,
        NumInds,
        Inds
    );

    delete[] Verts;
    delete[] Inds;

    return Result;
}

MeshStateT ParseOBJFile(byte* Contents, size_t Size)
{
    ParsedOBJ OBJ;

    bool bError = false;
    int ReadIdx = 0;
    while (!bError && ReadIdx < Size)
    {
        switch (Contents[ReadIdx])
        {
            case 'v':
            {
                if (Contents[ReadIdx + 1] != ' ') { break; } // Not vertex position data

                v4f NewVx{ 0.0f, 0.0f, 0.0f, 1.0f };
                {
                    char* BeginFloat = (char*)Contents + ReadIdx + 1;
                    char* CharAfter = nullptr;
                    NewVx.X = strtof(BeginFloat, &CharAfter);
                    if (CharAfter && CharAfter > BeginFloat)
                    {
                        BeginFloat = CharAfter;
                        CharAfter = nullptr;
                        NewVx.Y = strtof(BeginFloat, &CharAfter);
                        if (CharAfter && CharAfter > BeginFloat)
                        {
                            BeginFloat = CharAfter;
                            CharAfter = nullptr;
                            NewVx.Z = strtof(BeginFloat, &CharAfter);
                        }
                        else { bError = true; }
                    }
                    else { bError = true; }
                }
                if (!bError)
                {
                    OBJ.Vertices.Add(NewVx);
                }
            } break;
            case 'f':
            {
                if (Contents[ReadIdx + 1] != ' ') { break; }

                char* DigitBegin = (char*)Contents + ReadIdx + 1;
                char* AfterDigit = nullptr;

                FaceIndices NewFace{};
                while (!bError && (NewFace.NumVerts < FaceIndices::MaxVerts) && (ReadIdx < Size && *DigitBegin && *DigitBegin != '\n' && *DigitBegin != '\r'))
                {
                    int NewIndex = strtol(DigitBegin, &AfterDigit, 10);
                    if (NewIndex > 0)
                    {
                        NewFace.vIdx[NewFace.NumVerts++] = NewIndex - 1;
                    }
                    else { ASSERT(NewIndex > 0); bError = true; }
                    if (!AfterDigit) { bError = true; }
                    else { DigitBegin = AfterDigit; AfterDigit = nullptr; }
                }

                if (NewFace.NumVerts > 2) { OBJ.NumPrimitives += NewFace.NumVerts - 2; }
                else { bError = true; }

                if (!bError)
                {
                    OBJ.Faces.Add(NewFace);
                }
            } break;
        }

        // Read to next line
        while (ReadIdx < Size && Contents[ReadIdx] && Contents[ReadIdx] != '\n') { ReadIdx++; } ReadIdx++;
    }

    MeshStateT Result{};
    if (!bError)
    {
        Result = CreateMeshStateFromOBJ(OBJ);
    }
    return Result;
}

}

MeshStateT LoadMeshOBJ(const char* FileName)
{
    FileContentsT OBJContents = LoadFileContents(FileName, true);
    MeshStateT Result{};

    if (OBJContents.Size > 0 && OBJContents.Contents)
    {
        Result = MeshOBJ::ParseOBJFile(OBJContents.Contents, OBJContents.Size);

        delete[] OBJContents.Contents;
    }

    return Result;
}

}