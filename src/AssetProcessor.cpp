#include "AssetProcessor.h"

namespace Leviathan
{

namespace MeshOBJ
{

struct VxInds
{
    int pIdx = 0;
    int tIdx = 0;
    int nIdx = 0;
};

struct PerFaceData
{
    VxInds v0;
    VxInds v1;
    VxInds v2;
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

    static constexpr bool bSupportNormals = true;
    static constexpr bool bSupportTexcoords = false;

    MeshStateT Result{};

    if (bSupportNormals && bSupportTexcoords) { ASSERT(false); } // TODO
    else if (bSupportNormals && OBJ.Normals.Num > 0)
    {
        /*
            NOTE(CKA): Currently my solution is to just inline all vertex data individually
                and later clean this up so we're not having duplicate vertex data
        */
        size_t VxSize = sizeof(VxMinNormal);
        size_t NumVerts = OBJ.Faces.Num * 3;
        VxMinNormal* VxData = new VxMinNormal[NumVerts];

        int vIdx = 0;
        for (int fIdx = 0; fIdx < OBJ.Faces.Num; fIdx++)
        {
            VxData[vIdx + 0].Pos = OBJ.Vertices[OBJ.Faces[fIdx].v0.pIdx];
            VxData[vIdx + 0].Normal = OBJ.Normals[OBJ.Faces[fIdx].v0.nIdx];

            VxData[vIdx + 1].Pos = OBJ.Vertices[OBJ.Faces[fIdx].v1.pIdx];
            VxData[vIdx + 1].Normal = OBJ.Normals[OBJ.Faces[fIdx].v1.nIdx];

            VxData[vIdx + 2].Pos = OBJ.Vertices[OBJ.Faces[fIdx].v2.pIdx];
            VxData[vIdx + 2].Normal = OBJ.Normals[OBJ.Faces[fIdx].v2.nIdx];

            vIdx += 3;
        }

        ASSERT(vIdx == NumVerts);
        Result = CreateMeshState
        (
            Graphics::Device(),
            VxSize, NumVerts, VxData,
            0, nullptr
        );

        delete[] VxData;
    }
    else if (bSupportTexcoords) { ASSERT(false); } // TODO
    else
    {
        size_t VxSize = sizeof(VxMin);
        size_t NumVerts = OBJ.Vertices.Num;
        size_t NumInds = OBJ.Faces.Num * 3;
        VxMin* VxData = new VxMin[NumVerts];
        unsigned int* IxData = new unsigned int[NumInds];

        for (int vIdx = 0; vIdx < OBJ.Vertices.Num; vIdx++)
        {
            VxData[vIdx].Pos = OBJ.Vertices[vIdx];
        }
        int IndIdx = 0;
        for (int fIdx = 0; fIdx < OBJ.Faces.Num; fIdx++)
        {
            IxData[IndIdx + 0] = OBJ.Faces[fIdx].v0.pIdx;
            IxData[IndIdx + 1] = OBJ.Faces[fIdx].v1.pIdx;
            IxData[IndIdx + 2] = OBJ.Faces[fIdx].v2.pIdx;
            IndIdx += 3;
        }

        Result = CreateMeshState
        (
            Graphics::Device(),
            VxSize, NumVerts, VxData,
            NumInds, IxData
        );

        delete[] VxData;
        delete[] IxData;
    }

    return Result;
}

// TODO(CKA): Make this more robust (w.r.t. NumFloats)
bool ParseFloatArgs(byte* Contents, size_t Size, int ReadIdx, v4f& OutVx, int NumFloatsExpected)
{
    ASSERT(0 < NumFloatsExpected && NumFloatsExpected <= 4);

    bool bError = false;

    char* BeginFloat = (char*)Contents + ReadIdx + 1;
    char* CharAfter = nullptr;

    for (int FloatIdx = 0; !bError && FloatIdx < NumFloatsExpected; FloatIdx++)
    {
        float ParsedValue = strtof(BeginFloat, &CharAfter);
        if (CharAfter && CharAfter > BeginFloat)
        {
            switch (FloatIdx)
            {
                case 0: { OutVx.X = ParsedValue; } break;
                case 1: { OutVx.Y = ParsedValue; } break;
                case 2: { OutVx.Z = ParsedValue; } break;
                case 3: { OutVx.W = ParsedValue; } break;
                default: { ASSERT(false); bError = true; break; }
            }
            BeginFloat = CharAfter;
            CharAfter = nullptr;
        }
        else
        {
            ASSERT(false);
            bError = true;
        }
    }

    return bError;
}

bool ParseFaceData(byte* Contents, size_t Size, int ReadIdx, Array<VxInds>& NewFace)
{
    ASSERT(NewFace.Num == 0);
    ASSERT(Contents[ReadIdx + 1] == ' ');

    char* DigitBegin = (char*)Contents + ReadIdx + 1;
    char* AfterDigit = nullptr;

    bool bError = false;

    while (!bError && (ReadIdx < Size && *DigitBegin && *DigitBegin != '\n' && *DigitBegin != '\r'))
    {
        int NewIndex = strtol(DigitBegin, &AfterDigit, 10);

        if (NewIndex > 0 && AfterDigit)
        {
            VxInds NewVxInds{};
            NewVxInds.pIdx = NewIndex - 1;
            if (*AfterDigit == '/')
            {
                AfterDigit++;
                if (*AfterDigit != '/')
                {
                    char* TexIdxBegin = AfterDigit;
                    AfterDigit = nullptr;
                    int tIdx = strtol(TexIdxBegin, &AfterDigit, 10);
                    if (tIdx > 0 && AfterDigit) { NewVxInds.tIdx = tIdx - 1; }
                    else { bError = true; ASSERT(false); }
                }

                if (*AfterDigit == '/')
                {
                    AfterDigit++;
                    char* NormalIdxBegin = AfterDigit;
                    AfterDigit = nullptr;
                    int nIdx = strtol(NormalIdxBegin, &AfterDigit, 10);
                    if (nIdx > 0 && AfterDigit) { NewVxInds.nIdx = nIdx - 1; }
                    else { bError = true; ASSERT(false); }
                }
            }

            NewFace.Add(NewVxInds);

            DigitBegin = AfterDigit;
            AfterDigit = nullptr;
        }
        else
        {
            bError = true;
            ASSERT(false);
        }
    }

    return bError;
}

MeshStateT ParseOBJFile(byte* Contents, size_t Size)
{
    ParsedOBJ OBJ;

    Array<VxInds> CurrFace;

    bool bError = false;
    int ReadIdx = 0;
    while (!bError && ReadIdx < Size)
    {
        switch (Contents[ReadIdx])
        {
            case 'v':
            {
                if (Contents[ReadIdx + 1] == ' ') // vertex position
                {
                    v4f NewVx{ 0.0f, 0.0f, 0.0f, 1.0f };
                    bError = ParseFloatArgs(Contents, Size, ReadIdx, NewVx, 3);
                    if (!bError) { OBJ.Vertices.Add(NewVx); }
                }
                else if (Contents[ReadIdx + 1] == 't') // vertex texture
                {
                    ReadIdx++;
                    v4f NewVx{ 0.0f, 0.0f, 0.0f, 0.0f };
                    bError = ParseFloatArgs(Contents, Size, ReadIdx, NewVx, 2);
                    if (!bError) { OBJ.TexUVs.Add(v2f{ NewVx.X, NewVx.Y }); }
                }
                else if (Contents[ReadIdx + 1] == 'n') // vertex normal
                {
                    ReadIdx++;
                    v4f NewVx{ 0.0f, 0.0f, 0.0f, 0.0f };
                    bError = ParseFloatArgs(Contents, Size, ReadIdx, NewVx, 3);
                    if (!bError) { OBJ.Normals.Add(v3f{ NewVx.X, NewVx.Y, NewVx.Z }); }
                }
                else { bError = true; } // not vertex position/texture/normal data

                ASSERT(!bError);

            } break;
            case 'f':
            {
                if (Contents[ReadIdx + 1] != ' ') { break; }

                char* DigitBegin = (char*)Contents + ReadIdx + 1;
                char* AfterDigit = nullptr;

                CurrFace.Empty();
                bError = ParseFaceData(Contents, Size, ReadIdx, CurrFace);

                ASSERT(!bError);

                if (!bError && CurrFace.Num > 2)
                {
                    PerFaceData NewFace{};
                    NewFace.v0.pIdx = CurrFace[0].pIdx;
                    NewFace.v0.tIdx = CurrFace[0].tIdx;
                    NewFace.v0.nIdx = CurrFace[0].nIdx;

                    int NumFaces = CurrFace.Num - 2;
                    for (int FaceIdx = 0; FaceIdx < NumFaces; FaceIdx++)
                    {
                        NewFace.v1.pIdx = CurrFace[FaceIdx + 1].pIdx;
                        NewFace.v1.tIdx = CurrFace[FaceIdx + 1].tIdx;
                        NewFace.v1.nIdx = CurrFace[FaceIdx + 1].nIdx;

                        NewFace.v2.pIdx = CurrFace[FaceIdx + 2].pIdx;
                        NewFace.v2.tIdx = CurrFace[FaceIdx + 2].tIdx;
                        NewFace.v2.nIdx = CurrFace[FaceIdx + 2].nIdx;
                        OBJ.Faces.Add(NewFace);
                    }
                }
            } break;
            // TODO(CKA): Implement
            case 'm':
            case 'o':
            case 'u': break;
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