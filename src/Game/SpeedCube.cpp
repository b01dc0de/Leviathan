#include "SpeedCube.h"

namespace Game
{
    using namespace Leviathan;

    namespace SpeedCubeState
    {
        constexpr int NumPiecesPerRow = 9;
        constexpr int NumRows = 3;
        constexpr int NumPieces = NumPiecesPerRow * NumRows;

        MeshStateT SpeedCubeMesh;
        int IxPerPiece;

        enum CubePieceType
        {
            Piece_None,
            Piece_Center,
            Piece_Edge,
            Piece_Corner,
        };

        enum CubeFaceColor
        {
            Face_White,
            Face_Yellow,
            Face_Green,
            Face_Blue,
            Face_Orange,
            Face_Red,
        };

        enum PieceFaceDirection
        {
            FaceDir_PosX,
            FaceDir_NegX,
            FaceDir_PosY,
            FaceDir_NegY,
            FaceDir_PosZ,
            FaceDir_NegZ,
        };

        PieceFaceDirection RotateFaceDir_X(PieceFaceDirection StartDir, bool bClockWise)
        {
            switch (StartDir)
            {
                case FaceDir_PosX: case FaceDir_NegX: { return StartDir; }
                case FaceDir_PosY: { return bClockWise ? FaceDir_NegZ : FaceDir_PosZ; }
                case FaceDir_NegY: { return bClockWise ? FaceDir_PosZ : FaceDir_NegZ; }
                case FaceDir_PosZ: { return bClockWise ? FaceDir_PosY : FaceDir_NegY; }
                case FaceDir_NegZ: { return bClockWise ? FaceDir_NegY : FaceDir_PosY; }
            }
            ASSERT(false);
            return StartDir;
        }

        PieceFaceDirection RotateFaceDir_Y(PieceFaceDirection StartDir, bool bClockWise)
        {
            switch (StartDir)
            {
                case FaceDir_PosX: { return bClockWise ? FaceDir_PosZ : FaceDir_NegZ; }
                case FaceDir_NegX: { return bClockWise ? FaceDir_NegZ : FaceDir_PosZ; }
                case FaceDir_PosY: case FaceDir_NegY: { return StartDir; }
                case FaceDir_PosZ: { return bClockWise ? FaceDir_NegX : FaceDir_PosX; }
                case FaceDir_NegZ: { return bClockWise ? FaceDir_PosX : FaceDir_NegX; }
            }
            ASSERT(false);
            return StartDir;
        }

        PieceFaceDirection RotateFaceDir_Z(PieceFaceDirection StartDir, bool bClockWise)
        {
            switch (StartDir)
            {
                case FaceDir_PosX: { return bClockWise ? FaceDir_NegY : FaceDir_PosY; }
                case FaceDir_NegX: { return bClockWise ? FaceDir_PosY : FaceDir_NegY; }
                case FaceDir_PosY: { return bClockWise ? FaceDir_PosX : FaceDir_NegX; }
                case FaceDir_NegY: { return bClockWise ? FaceDir_NegX : FaceDir_PosX; }
                case FaceDir_PosZ: case FaceDir_NegZ: { return StartDir; }
            }
            ASSERT(false); return StartDir;
        }

        struct CenterPiece
        {
            CubeFaceColor ColorA;
            PieceFaceDirection DirA;
        };

        struct EdgePiece
        {
            CubeFaceColor ColorA;
            PieceFaceDirection DirA;
            CubeFaceColor ColorB;
            PieceFaceDirection DirB;
        };

        struct CornerPiece
        {
            CubeFaceColor ColorA;
            PieceFaceDirection DirA;
            CubeFaceColor ColorB;
            PieceFaceDirection DirB;
            CubeFaceColor ColorC;
            PieceFaceDirection DirC;
        };


        struct CubePieces
        {
            static constexpr int NumCenters = 6;
            static constexpr int NumEdges = 12;
            static constexpr int NumCorners = 8;
            CenterPiece Centers[6];
            EdgePiece Edges[12];
            CornerPiece Corners[8];

            void Init()
            {
                #define FACE_COLDIR_WHITE() Face_White, FaceDir_PosY
                #define FACE_COLDIR_YELLOW() Face_Yellow, FaceDir_NegY
                #define FACE_COLDIR_GREEN() Face_Green, FaceDir_PosZ
                #define FACE_COLDIR_BLUE() Face_Blue, FaceDir_NegZ
                #define FACE_COLDIR_ORANGE() Face_Orange, FaceDir_NegX
                #define FACE_COLDIR_RED() Face_Red, FaceDir_PosX
                {
                    Centers[0] = { FACE_COLDIR_WHITE()};
                    Centers[1] = { FACE_COLDIR_BLUE()};
                    Centers[2] = { FACE_COLDIR_ORANGE()};
                    Centers[3] = { FACE_COLDIR_RED()};
                    Centers[4] = { FACE_COLDIR_GREEN()};
                    Centers[5] = { FACE_COLDIR_YELLOW()};
                }
                {
                    Edges[0] = { FACE_COLDIR_WHITE(), FACE_COLDIR_BLUE() };
                    Edges[1] = { FACE_COLDIR_WHITE(), FACE_COLDIR_ORANGE() };
                    Edges[2] = { FACE_COLDIR_WHITE(), FACE_COLDIR_RED() };
                    Edges[3] = { FACE_COLDIR_WHITE(), FACE_COLDIR_GREEN() };

                    Edges[4] = { FACE_COLDIR_BLUE(), FACE_COLDIR_ORANGE() };
                    Edges[5] = { FACE_COLDIR_BLUE(), FACE_COLDIR_RED() };
                    Edges[6] = { FACE_COLDIR_GREEN(), FACE_COLDIR_ORANGE() };
                    Edges[7] = { FACE_COLDIR_GREEN(), FACE_COLDIR_RED() };

                    Edges[8] = { FACE_COLDIR_YELLOW(), FACE_COLDIR_BLUE() };
                    Edges[9] = { FACE_COLDIR_YELLOW(), FACE_COLDIR_ORANGE() };
                    Edges[10] = { FACE_COLDIR_YELLOW(), FACE_COLDIR_RED() };
                    Edges[11] = { FACE_COLDIR_YELLOW(), FACE_COLDIR_GREEN() };
                }
                {
                    Corners[0] = { FACE_COLDIR_WHITE(), FACE_COLDIR_BLUE(), FACE_COLDIR_ORANGE() };
                    Corners[1] = { FACE_COLDIR_WHITE(), FACE_COLDIR_BLUE(), FACE_COLDIR_RED() };
                    Corners[2] = { FACE_COLDIR_WHITE(), FACE_COLDIR_GREEN(), FACE_COLDIR_ORANGE() };
                    Corners[3] = { FACE_COLDIR_WHITE(), FACE_COLDIR_GREEN(), FACE_COLDIR_RED() };

                    Corners[4] = { FACE_COLDIR_YELLOW(), FACE_COLDIR_BLUE(), FACE_COLDIR_ORANGE() };
                    Corners[5] = { FACE_COLDIR_YELLOW(), FACE_COLDIR_BLUE(), FACE_COLDIR_RED() };
                    Corners[6] = { FACE_COLDIR_YELLOW(), FACE_COLDIR_GREEN(), FACE_COLDIR_ORANGE() };
                    Corners[7] = { FACE_COLDIR_YELLOW(), FACE_COLDIR_GREEN(), FACE_COLDIR_RED() };
                }
            }
            void Turn_Top_CW()
            {
                // Centers: 0
                // Edges:
                //  0 -> 2
                //  1 -> 0
                //  3 -> 1
                //  2 -> 3
                // Corners:
                //  0 -> 1
                //  1 -> 3
                //  2 -> 0
                //  3 -> 2
            }
            void Turn_Top_CCW() {};
            void Turn_Bot_CW() {};
            void Turn_Bot_CCW() {};
            void Turn_Left_CW() {};
            void Turn_Left_CCW() {};
            void Turn_Right_CW() {};
            void Turn_Right_CCW() {};
            void Turn_Front_CW() {};
            void Turn_Front_CCW() {};
            void Turn_Back_CW() {};
            void Turn_Back_CCW() {};
        };

        void LoadSpeedCubeMesh();

        bool FaceIsExternal(int PieceIdx, int FaceIdx);
    }

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

        // TODO: Move this off the stack
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
        //RotationX += RotSpeed;
        //RotationY += RotSpeed * 0.5f;

        static float FakeTime = 0.0f;
        FakeTime += RotSpeed;

        static bool bRotateTopBottom = true;
        static bool bRotateLeftRight = true;
        static bool bRotateFrontBack = true;

        auto GetRow = [](int PieceIdx) -> int
        {
            return PieceIdx / NumPiecesPerRow;
        };
        auto IsLeft = [](int PieceIdx) -> bool
        {
            return (PieceIdx % NumPiecesPerRow) % 3 == 2;
        };
        auto IsRight = [](int PieceIdx) -> bool
        {
            return (PieceIdx % NumPiecesPerRow) % 3 == 0;
        };
        auto IsFront = [](int PieceIdx) -> bool
        {
            return (PieceIdx % NumPiecesPerRow) / 3 == 2;
        };
        auto IsBack = [](int PieceIdx) -> bool
        {
            return (PieceIdx % NumPiecesPerRow) / 3 == 0;
        };

        for (int PieceIdx = 0; PieceIdx < NumPieces; PieceIdx++)
        {
            m4f PieceWorld = m4f::Identity();
            if (bRotateTopBottom)
            {
                if (GetRow(PieceIdx) == 0) { PieceWorld = PieceWorld * m4f::RotAxisY(FakeTime); }
                else if (GetRow(PieceIdx) == 2) { PieceWorld = PieceWorld * m4f::RotAxisY(-FakeTime); }
            }
            //else if (bRotateLeftRight)
            if (bRotateLeftRight)
            {
                if (IsLeft(PieceIdx)) { PieceWorld = PieceWorld * m4f::RotAxisX(FakeTime); }
                else if (IsRight(PieceIdx)) { PieceWorld = PieceWorld * m4f::RotAxisX(-FakeTime); }
            }
            //else if (bRotateFrontBack)
            if (bRotateFrontBack)
            {
                if (IsFront(PieceIdx)) { PieceWorld = PieceWorld * m4f::RotAxisZ(FakeTime); }
                else if (IsBack(PieceIdx)) { PieceWorld = PieceWorld * m4f::RotAxisZ(-FakeTime); }
            }

            UpdateShaderWorld(GFXContext.ImmContext, &PieceWorld);

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

}

