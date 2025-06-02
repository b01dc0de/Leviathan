#include "AssetProcessor.h"

namespace Leviathan
{

namespace MeshFormatObj
{

struct FaceVertex
{
    int pIdx = 0;
    int tIdx = 0;
    int nIdx = 0;
};

struct PerFaceData
{
    FaceVertex v0;
    FaceVertex v1;
    FaceVertex v2;
};

enum struct ObjStatementType
{
    None,
    Comment,
    MtlLib,
    ObjDecl,
    VertexPosition,
    VertexTexcoord,
    VertexNormal,
    UseMtl,
    Face,
};

bool TryMatchString(const char* A, size_t LengthA, const char* B)
{
    bool bResult = true;
    for (size_t Idx = 0; Idx < LengthA; Idx++, A++, B++)
    {
        if (*A == '\0' || *B == '\0' || *A != *B)
        {
            bResult = false;
            break;
        }
    }
    return bResult;
}

struct ObjParseContext
{
    char* Contents;
    size_t Size;
    size_t ReadIdx;
    bool bError;

    Array<v4f> Vertices;
    Array<v2f> TexUVs;
    Array<v3f> Normals;
    Array<PerFaceData> Faces;

    Array<FaceVertex> CurrFace;

    ObjParseContext(FileContentsT& InFileContents)
    {
        Contents = (char*)InFileContents.Contents;
        Size = InFileContents.Size;
        ReadIdx = 0;
        bError = false;
    }
    void Parse()
    {
        ASSERT(!bError && Contents && ReadIdx < Size);
        ASSERT(Vertices.Num == 0 && TexUVs.Num == 0 && Normals.Num == 0 && Faces.Num == 0);
        while (!bError && Contents && Contents[ReadIdx] && ReadIdx < Size)
        {
            ObjStatementType Type = ReadStatementType();
            ReadStatementArgs(Type);
        }
    }
    ObjStatementType ReadStatementType()
    {
        ASSERT(!bError && Contents && ReadIdx < Size);

        ObjStatementType Result = ObjStatementType::None;
        size_t CharsRead = 0;

        switch (Contents[ReadIdx])
        {
            case '#': { Result = ObjStatementType::Comment; CharsRead = 1; } break;
            case 'm':
            {
                if (TryMatchString("mtllib", 6, Contents + ReadIdx))
                {
                    Result = ObjStatementType::MtlLib;
                    CharsRead = 6;
                }
            } break;
            case 'o': { Result = ObjStatementType::ObjDecl; CharsRead = 1; } break;
            case 'v': 
            {
                ASSERT(ReadIdx + 1 < Size);
                if (Contents[ReadIdx + 1] == ' ')
                {
                    Result = ObjStatementType::VertexPosition;
                    CharsRead = 1;
                }
                else if (Contents[ReadIdx + 1] == 't')
                {
                    Result = ObjStatementType::VertexTexcoord;
                    CharsRead = 2;
                }
                else if (Contents[ReadIdx + 1] == 'n')
                {
                    Result = ObjStatementType::VertexNormal;
                    CharsRead = 2;
                }
            } break;
            case 'u':
            {
                if (TryMatchString("usemtl", 6, Contents + ReadIdx))
                {
                    Result = ObjStatementType::UseMtl;
                    CharsRead = 6;
                }
            } break;
            case 'f':
            {
                Result = ObjStatementType::Face;
                CharsRead = 1;
            } break;
        }

        ReadIdx += CharsRead;

        return Result;
    }
    size_t ReadFloat(float& Value)
    {
        ASSERT(!bError && Contents && ReadIdx < Size);

        char* BeginFloat = (char*)Contents + ReadIdx;
        char* CharAfter = nullptr;

        size_t CharsRead = 0;

        float ParsedValue = strtof(BeginFloat, &CharAfter);
        if (CharAfter && CharAfter > BeginFloat)
        {
            Value = ParsedValue;
            CharsRead = CharAfter - BeginFloat;
        }
        return CharsRead;
    }
    size_t ReadFaceVertex(FaceVertex& Vx)
    {
        ASSERT(!bError && Contents && ReadIdx < Size);
        ASSERT(Contents[ReadIdx] == ' ');

        char* DigitBegin = (char*)Contents + ReadIdx + 1;
        char* AfterDigit = nullptr;

        size_t CharsRead = 0;

        int Index = strtol(DigitBegin, &AfterDigit, 10);

        if (Index > 0 && AfterDigit)
        {
            Vx.pIdx = Index;
            if (*AfterDigit == '/')
            {
                AfterDigit++;
                if (*AfterDigit != '/')
                {
                    char* TexIdxBegin = AfterDigit; AfterDigit = nullptr;
                    Index = strtol(TexIdxBegin, &AfterDigit, 10);
                    if (Index > 0 && AfterDigit) { Vx.tIdx = Index; }
                    else { bError = true; ASSERT(false); }
                }

                if (*AfterDigit == '/')
                {
                    AfterDigit++;
                    char* NormalIdxBegin = AfterDigit;
                    AfterDigit = nullptr;
                    Index = strtol(NormalIdxBegin, &AfterDigit, 10);
                    if (Index > 0 && AfterDigit) { Vx.nIdx = Index; }
                    else { bError = true; ASSERT(false); }
                }
            }

            CharsRead = AfterDigit - (Contents + ReadIdx);
        }
        else
        {
            bError = true;
            ASSERT(false);
        }

        return CharsRead;
    }
    void ReadStatementArgs(ObjStatementType Type)
    {
        ASSERT(!bError && Contents && ReadIdx < Size);
        ASSERT(Type == ObjStatementType::None || Type == ObjStatementType::Comment || Contents[ReadIdx] == ' ');

        switch (Type)
        {
            case ObjStatementType::None:
            case ObjStatementType::Comment: { } break;
            case ObjStatementType::MtlLib: { } break;
            case ObjStatementType::ObjDecl: { } break;
            case ObjStatementType::VertexPosition:
            {
                float Pos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
                // TODO(CKA): Support reading optional W value if present
                for (int FloatIdx = 0; FloatIdx < 3; FloatIdx++)
                {
                    size_t CharsRead = ReadFloat(Pos[FloatIdx]);
                    if (CharsRead == 0) { bError = true; break; }
                    else { ReadIdx += CharsRead; }
                }
                if (!bError) { Vertices.Add(v4f{Pos[0], Pos[1], Pos[2], Pos[3]}); }
            } break;
            case ObjStatementType::VertexTexcoord:
            {
                float UV[] = { 0.0f, 0.0f };
                for (int FloatIdx = 0; FloatIdx < ARRAY_SIZE(UV); FloatIdx++)
                {
                    size_t CharsRead = ReadFloat(UV[FloatIdx]);
                    if (CharsRead == 0) { bError = true; break; }
                    else { ReadIdx += CharsRead; }
                }
                if (!bError)
                {
                    UV[1] = 1.0f - UV[1];
                    TexUVs.Add(v2f{UV[0], UV[1]});
                }
            } break;
            case ObjStatementType::VertexNormal:
            {
                float Normal[] = { 0.0f, 0.0f, 0.0f };
                for (int FloatIdx = 0; FloatIdx < 3; FloatIdx++)
                {
                    size_t CharsRead = ReadFloat(Normal[FloatIdx]);
                    if (CharsRead == 0) { bError = true; break; }
                    else { ReadIdx += CharsRead; }
                }
                if (!bError) { Normals.Add(v3f{Normal[0], Normal[1], Normal[2]}); }
            } break;
            case ObjStatementType::UseMtl: { } break;
            case ObjStatementType::Face:
            {
                CurrFace.Empty();
                while (ReadIdx < Size && Contents[ReadIdx] == ' ')
                {
                    FaceVertex Vx{};
                    size_t CharsRead = ReadFaceVertex(Vx);
                    if (CharsRead > 0)
                    {
                        CurrFace.Add(Vx);
                        ReadIdx += CharsRead;
                    }
                }
                if (CurrFace.Num > 2)
                {
                    PerFaceData NewFace{};
                    NewFace.v0.pIdx = CurrFace[0].pIdx - 1;
                    NewFace.v0.tIdx = CurrFace[0].tIdx - 1;
                    NewFace.v0.nIdx = CurrFace[0].nIdx - 1;
                    for (int fIdx = 0; fIdx < (CurrFace.Num - 2); fIdx++)
                    {
                        NewFace.v1.pIdx = CurrFace[fIdx + 1].pIdx - 1;
                        NewFace.v1.tIdx = CurrFace[fIdx + 1].tIdx - 1;
                        NewFace.v1.nIdx = CurrFace[fIdx + 1].nIdx - 1;

                        NewFace.v2.pIdx = CurrFace[fIdx + 2].pIdx - 1;
                        NewFace.v2.tIdx = CurrFace[fIdx + 2].tIdx - 1;
                        NewFace.v2.nIdx = CurrFace[fIdx + 2].nIdx - 1;
                        Faces.Add(NewFace);
                    }
                }
                else { bError = true; }
            } break;
        }

        // Read until new line (or end)
        while (ReadIdx < Size && Contents[ReadIdx++] != '\n') { }
    }
    MeshStateT ConstructMesh(bool bUseUVs, bool bUseNormals)
    {
        ASSERT(Vertices.Num > 0 && Faces.Num > 0);
        if (Vertices.Num == 0 || Faces.Num == 0) { return MeshStateT{}; }

        static constexpr bool bEnableTexcoordSupport = true;
        static constexpr bool bEnableNormalSupport = true;

        MeshStateT Result{};

        bool bTexcoords = bEnableTexcoordSupport && bUseUVs && (0 < TexUVs.Num);
        bool bNormals = bEnableNormalSupport && bUseNormals && (0 < Normals.Num);
        if (bTexcoords && bNormals)
        {
            size_t VxSize = sizeof(VxTextureNormal);
            size_t NumVerts = Faces.Num * 3;
            VxTextureNormal* VxData = new VxTextureNormal[NumVerts];

            int vIdx = 0;
            for (int fIdx = 0; fIdx < Faces.Num; fIdx++)
            {
                VxData[vIdx + 0].Pos = Vertices[Faces[fIdx].v0.pIdx];
                VxData[vIdx + 0].TexUV = TexUVs[Faces[fIdx].v0.tIdx];
                VxData[vIdx + 0].Normal = Normals[Faces[fIdx].v0.nIdx];

                VxData[vIdx + 1].Pos = Vertices[Faces[fIdx].v1.pIdx];
                VxData[vIdx + 1].TexUV = TexUVs[Faces[fIdx].v1.tIdx];
                VxData[vIdx + 1].Normal = Normals[Faces[fIdx].v1.nIdx];

                VxData[vIdx + 2].Pos = Vertices[Faces[fIdx].v2.pIdx];
                VxData[vIdx + 2].TexUV = TexUVs[Faces[fIdx].v2.tIdx];
                VxData[vIdx + 2].Normal = Normals[Faces[fIdx].v2.nIdx];

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
        else if (bTexcoords)
        { 
            size_t VxSize = sizeof(VxTexture);
            size_t NumVerts = Faces.Num * 3;
            VxTexture* VxData = new VxTexture[NumVerts];

            int vIdx = 0;
            for (int fIdx = 0; fIdx < Faces.Num; fIdx++)
            {
                VxData[vIdx + 0].Pos = Vertices[Faces[fIdx].v0.pIdx];
                VxData[vIdx + 0].TexUV = TexUVs[Faces[fIdx].v0.tIdx];

                VxData[vIdx + 1].Pos = Vertices[Faces[fIdx].v1.pIdx];
                VxData[vIdx + 1].TexUV = TexUVs[Faces[fIdx].v1.tIdx];

                VxData[vIdx + 2].Pos = Vertices[Faces[fIdx].v2.pIdx];
                VxData[vIdx + 2].TexUV = TexUVs[Faces[fIdx].v2.tIdx];

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
        else if (bNormals)
        {
            size_t VxSize = sizeof(VxMinNormal);
            size_t NumVerts = Faces.Num * 3;
            VxMinNormal* VxData = new VxMinNormal[NumVerts];

            int vIdx = 0;
            for (int fIdx = 0; fIdx < Faces.Num; fIdx++)
            {
                VxData[vIdx + 0].Pos = Vertices[Faces[fIdx].v0.pIdx];
                VxData[vIdx + 0].Normal = Normals[Faces[fIdx].v0.nIdx];

                VxData[vIdx + 1].Pos = Vertices[Faces[fIdx].v1.pIdx];
                VxData[vIdx + 1].Normal = Normals[Faces[fIdx].v1.nIdx];

                VxData[vIdx + 2].Pos = Vertices[Faces[fIdx].v2.pIdx];
                VxData[vIdx + 2].Normal = Normals[Faces[fIdx].v2.nIdx];

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
        else
        {
            size_t VxSize = sizeof(VxMin);
            size_t NumVerts = Vertices.Num;
            size_t NumInds = Faces.Num * 3;
            VxMin* VxData = new VxMin[NumVerts];
            unsigned int* IxData = new unsigned int[NumInds];

            for (int vIdx = 0; vIdx < Vertices.Num; vIdx++)
            {
                VxData[vIdx].Pos = Vertices[vIdx];
            }
            int IndIdx = 0;
            for (int fIdx = 0; fIdx < Faces.Num; fIdx++)
            {
                IxData[IndIdx + 0] = Faces[fIdx].v0.pIdx;
                IxData[IndIdx + 1] = Faces[fIdx].v1.pIdx;
                IxData[IndIdx + 2] = Faces[fIdx].v2.pIdx;
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

};

} // namespace MeshFormatObj

MeshStateT LoadMeshOBJ(const char* FileName, bool bUseUVs, bool bUseNormals)
{
    FileContentsT ObjContents = LoadFileContents(FileName, true);
    MeshStateT Result{};

    if (ObjContents.Size > 0 && ObjContents.Contents)
    {
        MeshFormatObj::ObjParseContext ParseContext{ObjContents};
        ParseContext.Parse();
        ASSERT(!ParseContext.bError);
        if (!ParseContext.bError)
        {
            Result = ParseContext.ConstructMesh(bUseUVs, bUseNormals);
        }

        delete[] ObjContents.Contents;
    }

    return Result;
}

} // namespace Leviathan