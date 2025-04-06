#include "SpeedCube.h"

namespace Game
{
    using namespace Leviathan;

    namespace SpeedCubeState
    {
        MeshStateT SpeedCubeMesh;
        int IxPerPiece;

        bool FaceIsExternal(int PieceIdx, int FaceIdx);
        void LoadSpeedCubeMesh();
    }

    bool SpeedCubeState::FaceIsExternal(int PieceIdx, int FaceIdx)
    {
        // FaceIdx:
        //      0 -> Front / Green
        //      1 -> Back / Blue
        //      2 -> Top / White
        //      3 -> Bottom / Yellow
        //      4 -> Left / Orange
        //      5 -> Right / Red
        constexpr int Face_Green = 0;
        constexpr int Face_Blue = 1;
        constexpr int Face_White = 2;
        constexpr int Face_Yellow = 3;
        constexpr int Face_Orange = 4;
        constexpr int Face_Red = 5;

        switch (PieceIdx)
        {
            // Internal Core
            case 13: { return false; } // All faces are internal
                   break;

            // Center 
            //  4 -> White
            //  10 -> Blue
            //  12 -> Orange
            //  14 -> Red
            //  16 -> Green
            //  22 -> Yellow
            case 4: { return FaceIdx == Face_White; }
            case 10: { return FaceIdx == Face_Blue; }
            case 12: { return FaceIdx == Face_Orange; }
            case 14: { return FaceIdx == Face_Red; }
            case 16: { return FaceIdx == Face_Green; }
            case 22: { return FaceIdx == Face_Yellow; }
                   break;

           // Corner
           //  0 -> White/Blue/Orange
           //  2 -> White/Blue/Red
           //  6 -> White/Green/Orange
           //  8 -> White/Green/Red
            case 0: { return FaceIdx == Face_White || FaceIdx == Face_Blue || FaceIdx == Face_Orange; }
            case 2: { return FaceIdx == Face_White || FaceIdx == Face_Blue || FaceIdx == Face_Red; }
            case 6: { return FaceIdx == Face_White || FaceIdx == Face_Green || FaceIdx == Face_Orange; }
            case 8: { return FaceIdx == Face_White || FaceIdx == Face_Green || FaceIdx == Face_Red; }
            //  18 -> Yellow/Blue/Orange
            //  20 -> Yellow/Blue/Red
            //  24 -> Yellow/Green/Orange
            //  26 -> Yellow/Green/Red
            case 18: { return FaceIdx == Face_Yellow || FaceIdx == Face_Blue || FaceIdx == Face_Orange; }
            case 20: { return FaceIdx == Face_Yellow || FaceIdx == Face_Blue || FaceIdx == Face_Red; }
            case 24: { return FaceIdx == Face_Yellow || FaceIdx == Face_Green || FaceIdx == Face_Orange; }
            case 26: { return FaceIdx == Face_Yellow || FaceIdx == Face_Green || FaceIdx == Face_Red; }
                   break;

            // Edge
            //  1 -> White/Blue
            //  3 -> White/Orange
            //  5 -> White/Red
            //  7 -> White/Green
            case 1: { return FaceIdx == Face_White || FaceIdx == Face_Blue; }
            case 3: { return FaceIdx == Face_White || FaceIdx == Face_Orange; }
            case 5: { return FaceIdx == Face_White || FaceIdx == Face_Red; }
            case 7: { return FaceIdx == Face_White || FaceIdx == Face_Green; }
            //  9 -> Blue/Orange
            //  11 -> Blue/Red
            //  15 -> Green/Orange
            //  17 -> Green/Red
            case 9: { return FaceIdx == Face_Blue || FaceIdx == Face_Orange; }
            case 11: { return FaceIdx == Face_Blue || FaceIdx == Face_Red; }
            case 15: { return FaceIdx == Face_Green || FaceIdx == Face_Orange; }
            case 17: { return FaceIdx == Face_Green || FaceIdx == Face_Red; }
            //  19 -> Yellow/Blue
            //  21 -> Yellow/Orange
            //  23 -> Yellow/Red
            //  25 -> Yellow/Green
            case 19: { return FaceIdx == Face_Yellow || FaceIdx == Face_Blue; }
            case 21: { return FaceIdx == Face_Yellow || FaceIdx == Face_Orange; }
            case 23: { return FaceIdx == Face_Yellow || FaceIdx == Face_Red; }
            case 25: { return FaceIdx == Face_Yellow || FaceIdx == Face_Green; }
                   break;

            default: { ASSERT(false); } // shouldn't get here!
        }

        ASSERT(false); // shouldn't get here!

        return false;
    };

    void SpeedCubeState::LoadSpeedCubeMesh()
    {
        constexpr v4f FaceColor_White{ 1.0f, 1.0f, 1.0f, 1.0f };
        constexpr v4f FaceColor_Yellow{ 1.0f, 1.0f, 0.0f, 1.0f };
        constexpr v4f FaceColor_Green{ 0.0f, 1.0f, 0.0f, 1.0f };
        constexpr v4f FaceColor_Blue{ 0.0f, 0.0f, 1.0f, 1.0f };
        constexpr v4f FaceColor_Orange{ 1.0f, 0.5f, 0.0f, 1.0f };
        constexpr v4f FaceColor_Red{ 1.0f, 0.0f, 0.0f, 1.0f };
        float fUnit = 1.0f;
        VxColor Vertices_CubeFacesColor[] =
        {
            // Front: 0, 2, 1, 3
            { {-fUnit, +fUnit, +fUnit, 1.0f}, FaceColor_Green }, // 0
            { {+fUnit, +fUnit, +fUnit, 1.0f}, FaceColor_Green }, // 1
            { {-fUnit, -fUnit, +fUnit, 1.0f}, FaceColor_Green }, // 2
            { {+fUnit, -fUnit, +fUnit, 1.0f}, FaceColor_Green }, // 3
            // Back: 5, 7, 4, 6
            { {+fUnit, +fUnit, -fUnit, 1.0f}, FaceColor_Blue }, // 5
            { {-fUnit, +fUnit, -fUnit, 1.0f}, FaceColor_Blue }, // 4
            { {+fUnit, -fUnit, -fUnit, 1.0f}, FaceColor_Blue }, // 7
            { {-fUnit, -fUnit, -fUnit, 1.0f}, FaceColor_Blue }, // 6
            // Top: 4, 0, 5, 1
            { {-fUnit, +fUnit, -fUnit, 1.0f}, FaceColor_White }, // 4
            { {+fUnit, +fUnit, -fUnit, 1.0f}, FaceColor_White }, // 5
            { {-fUnit, +fUnit, +fUnit, 1.0f}, FaceColor_White }, // 0
            { {+fUnit, +fUnit, +fUnit, 1.0f}, FaceColor_White }, // 1
            // Bottom: 2, 6, 3, 7
            { {-fUnit, -fUnit, +fUnit, 1.0f}, FaceColor_Yellow }, // 2
            { {+fUnit, -fUnit, +fUnit, 1.0f}, FaceColor_Yellow }, // 3
            { {-fUnit, -fUnit, -fUnit, 1.0f}, FaceColor_Yellow }, // 6
            { {+fUnit, -fUnit, -fUnit, 1.0f}, FaceColor_Yellow }, // 7
            // Left: 4, 6, 0, 2
            { {-fUnit, +fUnit, -fUnit, 1.0f}, FaceColor_Orange }, // 4
            { {-fUnit, +fUnit, +fUnit, 1.0f}, FaceColor_Orange }, // 0
            { {-fUnit, -fUnit, -fUnit, 1.0f}, FaceColor_Orange }, // 6
            { {-fUnit, -fUnit, +fUnit, 1.0f}, FaceColor_Orange }, // 2
            // Right: 1, 3, 5, 7
            { {+fUnit, +fUnit, +fUnit, 1.0f}, FaceColor_Red }, // 1
            { {+fUnit, +fUnit, -fUnit, 1.0f}, FaceColor_Red }, // 5
            { {+fUnit, -fUnit, +fUnit, 1.0f}, FaceColor_Red }, // 3
            { {+fUnit, -fUnit, -fUnit, 1.0f}, FaceColor_Red }, // 7
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

        constexpr int NumPiecesPerRow = 9;
        constexpr int NumRows = 3;
        constexpr int NumPieces = NumPiecesPerRow * NumRows;

        VxColor Vertices_SpeedCube[NumPieces][ARRAY_SIZE(Vertices_CubeFacesColor)] = { };
        unsigned int Indices_SpeedCube[NumPieces][ARRAY_SIZE(Indices_CubeFaces)] = { };

        float SpeedCubeUnit = fUnit * 2.0f + 0.1f;
        for (int PieceIdx = 0; PieceIdx < NumPieces; PieceIdx++)
        {
            int RowIdx = PieceIdx / NumPiecesPerRow;
            int PieceIdxWithinRow = PieceIdx % NumPiecesPerRow;

            float PieceX = -SpeedCubeUnit + (float)(PieceIdxWithinRow % 3) * SpeedCubeUnit;
            float PieceY = +SpeedCubeUnit - (float)RowIdx * SpeedCubeUnit;
            float PieceZ = -SpeedCubeUnit + (float)(PieceIdxWithinRow / 3) * SpeedCubeUnit;
            v4f vOffset = { PieceX, PieceY, PieceZ, 0.0f };
            for (int VxIdx = 0; VxIdx < ARRAY_SIZE(Vertices_CubeFacesColor); VxIdx++)
            {
                Vertices_SpeedCube[PieceIdx][VxIdx] =
                {
                    Vertices_CubeFacesColor[VxIdx].Pos + vOffset, // v4f Pos
                    Vertices_CubeFacesColor[VxIdx].Col, // v4f Col
                };

                constexpr int VxPerFace = 4;
                int FaceIdx = VxIdx / VxPerFace;
                if (!FaceIsExternal(PieceIdx, FaceIdx))
                {
                    Vertices_SpeedCube[PieceIdx][VxIdx].Col = { 0.0f, 0.0f, 0.0f, 1.0f };
                }
            }

            int PieceFirstIx = ARRAY_SIZE(Vertices_CubeFacesColor) * PieceIdx;
            for (int IxIdx = 0; IxIdx < ARRAY_SIZE(Indices_CubeFaces); IxIdx++)
            {
                Indices_SpeedCube[PieceIdx][IxIdx] = Indices_CubeFaces[IxIdx] + PieceFirstIx;
            }
        }

        IxPerPiece = ARRAY_SIZE(Indices_CubeFaces);

        int TotalVxCount = ARRAY_SIZE(Vertices_SpeedCube) * ARRAY_SIZE(Vertices_CubeFacesColor);
        int TotalIxCount = ARRAY_SIZE(Indices_SpeedCube) * ARRAY_SIZE(Indices_CubeFaces);

        SpeedCubeMesh = CreateMeshState
        (
            Graphics::Device(),
            sizeof(VxColor),
            TotalVxCount,
            Vertices_SpeedCube,
            TotalIxCount,
            Indices_SpeedCube[0]
        );
    }

    using namespace SpeedCubeState;

    void SpeedCube::Update()
    {

    }

    void SpeedCube::Draw(GameGraphicsContext & GFXContext)
    {
        GFXContext.ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
        ID3D11Buffer* World_ViewProjBuffers[] = { GFXContext.WorldBuffer, GFXContext.ViewProjBuffer };
        SetShaderConstantBuffers(GFXContext.ImmContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);
        UpdateShaderViewProj(GFXContext.ImmContext, GFXContext.GameCamera);

        static float RotationX = 0.0f;
        static float RotationY = 0.0f;
        static constexpr float RotSpeed = (1.0f / 60.0f) / 100.0f;
        RotationX += RotSpeed;
        RotationY += RotSpeed * 0.5f;
        m4f SpeedCubeRotation = m4f::RotAxisX(RotationX) * m4f::RotAxisY(RotationY);

        m4f PieceWorld = SpeedCubeRotation;
        UpdateShaderWorld(GFXContext.ImmContext, &PieceWorld);

        constexpr int NumPiecesPerRow = 9;
        constexpr int NumRows = 3;
        constexpr int NumPieces = NumPiecesPerRow * NumRows;
        for (int PieceIdx = 0; PieceIdx < NumPieces; PieceIdx++)
        {
            int StartIx = PieceIdx * IxPerPiece;
            DrawMeshIxRange(GFXContext.ImmContext, *GFXContext.DrawStateColor, SpeedCubeMesh, StartIx, IxPerPiece);
        }
    }

    void SpeedCube::Init()
    {
        LoadSpeedCubeMesh();
    }

    void SpeedCube::Term()
    {
        SafeRelease(SpeedCubeMesh);
    }
}

