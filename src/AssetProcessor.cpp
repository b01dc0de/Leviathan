#include "AssetProcessor.h"

namespace Leviathan
{

namespace MeshOBJ
{

struct PerFaceData
{
    int vIdx0;
    int tIdx0;
    int nIdx0;

    int vIdx1;
    int tIdx1;
    int nIdx1;

    int vIdx2;
    int tIdx2;
    int nIdx2;
};

struct ParsedOBJ
{
    Array<v4f> Vertices;
    Array<v2f> TexUVs;
    Array<v3f> Normals;
    Array<PerFaceData> Faces;
};

MeshStateT CreateMeshStateFromOBJ(ParsedOBJ& OBJ)
{
    ASSERT(OBJ.Vertices.Num > 0 && OBJ.Faces.Num > 0);
    if (OBJ.Vertices.Num == 0 || OBJ.Faces.Num == 0) { return MeshStateT{}; }

    // TODO(CKA): Implement support for texture (+ normal!) vertices
    int NumVerts = OBJ.Vertices.Num;
    int NumInds = OBJ.Faces.Num * 3;
    VxMin* Verts = new VxMin[NumVerts];
    unsigned int* Inds = new unsigned int[NumInds];

    for (int vIdx = 0; vIdx < OBJ.Vertices.Num; vIdx++)
    {
        Verts[vIdx].Pos = OBJ.Vertices[vIdx];
    }
    int IndIdx = 0;
    for (int fIdx = 0; fIdx < OBJ.Faces.Num; fIdx++)
    {
        Inds[IndIdx + 0] = OBJ.Faces[fIdx].vIdx0;
        Inds[IndIdx + 1] = OBJ.Faces[fIdx].vIdx1;
        Inds[IndIdx + 2] = OBJ.Faces[fIdx].vIdx2;
        IndIdx += 3;
    }

    MeshStateT Result = CreateMeshState
    (
        Graphics::Device(),
        sizeof(VxMin), // TODO: Support multiple vertex types
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

    struct FaceInds
    {
        int vIdx = 0;
        int tIdx = 0;
        int nIdx = 0;
    };
    Array<FaceInds> CurrFace;

    bool bError = false;
    int ReadIdx = 0;
    while (!bError && ReadIdx < Size)
    {
        switch (Contents[ReadIdx])
        {
            case 'm':
            case 'o':
            case 'u': break;
            case 'v':
            {
                if (Contents[ReadIdx + 1] == 't') { break; } // vertex texture
                if (Contents[ReadIdx + 1] == 'n') { break; } // vertex normal
                else if (Contents[ReadIdx + 1] == ' ') { } // vertex position
                else
                {
                    // not vertex position data
                    ASSERT(false);
                    bError = true;
                }

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

                CurrFace.Empty();
                while (!bError && (ReadIdx < Size && *DigitBegin && *DigitBegin != '\n' && *DigitBegin != '\r'))
                {
                    int NewIndex = strtol(DigitBegin, &AfterDigit, 10);

                    if (NewIndex > 0 && AfterDigit)
                    {
                        FaceInds NewFaceInds{};
                        NewFaceInds.vIdx = NewIndex - 1;
                        if (*AfterDigit == '/') { 
                            do { AfterDigit++; }
                            while (*AfterDigit && *AfterDigit != ' ' && *AfterDigit != '\n' && *AfterDigit != '\r');
                        }

                        CurrFace.Add(NewFaceInds);

                        DigitBegin = AfterDigit;
                        AfterDigit = nullptr;
                    }
                    else
                    {
                        bError = true;
                        ASSERT(false);
                    }
                }

                if (CurrFace.Num > 2)
                {
                    PerFaceData NewFace{};
                    NewFace.vIdx0 = CurrFace[0].vIdx;
                    NewFace.tIdx0 = CurrFace[0].tIdx;
                    NewFace.nIdx0 = CurrFace[0].vIdx;

                    int NumFaces = CurrFace.Num - 2;
                    for (int FaceIdx = 0; FaceIdx < NumFaces; FaceIdx++)
                    {
                        NewFace.vIdx1 = CurrFace[FaceIdx + 1].vIdx;
                        NewFace.tIdx1 = CurrFace[FaceIdx + 1].tIdx;
                        NewFace.nIdx1 = CurrFace[FaceIdx + 1].nIdx;

                        NewFace.vIdx2 = CurrFace[FaceIdx + 2].vIdx;
                        NewFace.tIdx2 = CurrFace[FaceIdx + 2].tIdx;
                        NewFace.nIdx2 = CurrFace[FaceIdx + 2].nIdx;
                        OBJ.Faces.Add(NewFace);
                    }
                }
                else { bError = true; }
            } break;
        }

        // Read to next line
        while (ReadIdx < Size && Contents[ReadIdx] && Contents[ReadIdx] != '\n') { ReadIdx++; } ReadIdx++;
    }

    ASSERT(!bError);

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