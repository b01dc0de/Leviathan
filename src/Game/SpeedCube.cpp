#include "SpeedCube.h"

namespace Game
{
    using namespace Leviathan;

    namespace SpeedCubeState
    {
        constexpr int NumPiecesPerRow = 9;
        constexpr int NumRows = 3;
        constexpr int NumPieces = NumPiecesPerRow * NumRows;
        constexpr int MaxTurns = 4;

        MeshStateT SpeedCubeMesh;
        int IxPerPiece;

        struct Piece
        {
            int ID;
            int Turns[3];
        };

        const char* GetIDName(int ID);
        struct CubePieces
        {
            static constexpr int NumCenters = 6;
            static constexpr int NumEdges = 12;
            static constexpr int NumCorners = 8;
            Piece Centers[6];
            Piece Edges[12];
            Piece Corners[8];

            void Init()
            {
                {
                    Centers[0] = { 4 };
                    Centers[1] = { 10 };
                    Centers[2] = { 12 };
                    Centers[3] = { 14 };
                    Centers[4] = { 16 };
                    Centers[5] = { 22 };
                }
                {
                    Edges[0] = { 1 };
                    Edges[1] = { 3 };
                    Edges[2] = { 5 };
                    Edges[3] = { 7 };
                    Edges[4] = { 9 };
                    Edges[5] = { 11 };
                    Edges[6] = { 15 };
                    Edges[7] = { 17 };
                    Edges[8] = { 19 };
                    Edges[9] = { 21 };
                    Edges[10] = { 23 };
                    Edges[11] = { 25 };
                }
                {
                    Corners[0] = { 0 };
                    Corners[1] = { 2 };
                    Corners[2] = { 6 };
                    Corners[3] = { 8 };
                    Corners[4] = { 18 };
                    Corners[5] = { 20 };
                    Corners[6] = { 24 };
                    Corners[7] = { 26 };
                }
            }
            void DebugPrint()
            {
                char PrintBuffer[4096];
                sprintf_s(PrintBuffer, "Top Row: \n\t%s\t%s\t%s\n\t%s\t%s\t%s\n\t%s\t%s\t%s\n",
                    GetIDName(Corners[0].ID), GetIDName(Edges[0].ID), GetIDName(Corners[1].ID),
                    GetIDName(Edges[1].ID), GetIDName(Centers[0].ID), GetIDName(Edges[2].ID),
                    GetIDName(Corners[2].ID), GetIDName(Edges[3].ID), GetIDName(Corners[3].ID)
                );
                OutputDebugStringA(PrintBuffer);
            }
            struct TurnGroup
            {
                // Group[0]: Center
                // Group[1, 4]: Edges
                // Group[5, 8]: Corners
                Piece* Group[9];
                void TurnCW(int Axis, bool bPoleDirection)
                {
                    for (int GroupIdx = 0; GroupIdx < 9; GroupIdx++)
                    {
                        if (bPoleDirection) { Group[GroupIdx]->Turns[Axis]--; }
                        else { Group[GroupIdx]->Turns[Axis]++; }
                    }
                    Piece Tmp = *Group[4];
                    *Group[4] = *Group[3];
                    *Group[3] = *Group[2];
                    *Group[2] = *Group[1];
                    *Group[1] = Tmp;
                    Tmp = *Group[8];
                    *Group[8] = *Group[7];
                    *Group[7] = *Group[6];
                    *Group[6] = *Group[5];
                    *Group[5] = Tmp;
                }
                void TurnCCW(int Axis, bool bPoleDirection)
                {
                    for (int GroupIdx = 0; GroupIdx < 9; GroupIdx++)
                    {
                        Group[GroupIdx]->Turns[Axis]++;
                        if (bPoleDirection) { Group[GroupIdx]->Turns[Axis]++; }
                        else { Group[GroupIdx]->Turns[Axis]--; }
                    }
                    Piece Tmp = *Group[1];
                    *Group[1] = *Group[2];
                    *Group[2] = *Group[3];
                    *Group[3] = *Group[4];
                    *Group[4] = Tmp;
                    Tmp = *Group[5];
                    *Group[5] = *Group[6];
                    *Group[6] = *Group[7];
                    *Group[7] = *Group[8];
                    *Group[8] = Tmp;
                }
            };
            void Turn_Top(bool bClockWise)
            {
                TurnGroup TopGroup =
                {
                    &Centers[0],
                    &Edges[0], &Edges[1],
                    &Edges[3], &Edges[2],
                    &Corners[0], &Corners[2],
                    &Corners[3], &Corners[1]
                };
                if (bClockWise) { TopGroup.TurnCW(1, true); }
                else { TopGroup.TurnCCW(1, true); }
            }
            void Turn_Left(bool bClockWise)
            {
                TurnGroup LeftGroup =
                {
                    &Centers[2],
                    &Edges[1],
                    &Edges[4],
                    &Edges[9],
                    &Edges[6],
                    &Corners[0],
                    &Corners[4],
                    &Corners[6],
                    &Corners[2]
                };
                
                if (bClockWise) { LeftGroup.TurnCW(0, false); }
                else { LeftGroup.TurnCCW(0, false); }

                // Centers: 2
                // Edges: 
                //  1 -> 4
                //  4 -> 9
                //  9 -> 6
                //  6 -> 1
                // Corners:
            };
            void Turn_Right(bool bClockWise) {};
            void Turn_Front(bool bClockWise) {};
            void Turn_Bot(bool bClockWise) {};
            void Turn_Back(bool bClockWise) {};
        };

        CubePieces TheCube;

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
        bool bCCW = KeyboardState::GetKey(LV_KEY_SHIFT, true);
        if (KeyboardState::GetKey(LV_KEY_ARROW_UP))
        {
            TheCube.Turn_Top(!bCCW);
            TheCube.DebugPrint();
        }
        else if (KeyboardState::GetKey(LV_KEY_ARROW_LEFT))
        {
            TheCube.Turn_Left(!bCCW);
            TheCube.DebugPrint();
        }
        else if (KeyboardState::GetKey(LV_KEY_ARROW_DOWN))
        {
            TheCube.Turn_Front(!bCCW);
        }
        else if (KeyboardState::GetKey(LV_KEY_ARROW_RIGHT))
        {
            TheCube.Turn_Right(!bCCW);
        }

        if (KeyboardState::GetKey(LV_KEY_Q))
        {
            TheCube.Corners[0].Turns[0]++;
        }
        else if (KeyboardState::GetKey(LV_KEY_W))
        {
            TheCube.Corners[0].Turns[1]++;
        }
        else if (KeyboardState::GetKey(LV_KEY_E))
        {
            TheCube.Corners[0].Turns[2]++;
        }
    }

    void SpeedCube::Draw(GameGraphicsContext & GFXContext)
    {
        GFXContext.ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
        ID3D11Buffer* World_ViewProjBuffers[] = { GFXContext.WorldBuffer, GFXContext.ViewProjBuffer };
        SetShaderConstantBuffers(GFXContext.ImmContext, ARRAY_SIZE(World_ViewProjBuffers), World_ViewProjBuffers);
        UpdateShaderViewProj(GFXContext.ImmContext, GFXContext.GameCamera);

        static constexpr float RotSpeed = (1.0f / 60.0f) / 100.0f;
        static float FakeTime = 0.0f;
        FakeTime += RotSpeed;

        auto GetTurnMatrix = [](int *Turns) -> m4f
        {
            constexpr float TurnsToRadians = fPI / 2.0f;
            return m4f::RotAxisX(Turns[0] * TurnsToRadians) *
                m4f::RotAxisY(Turns[1] * TurnsToRadians) *
                m4f::RotAxisZ(Turns[2] * TurnsToRadians);
        };

        // Draw each cube piece
        {
            for (int CenterIdx = 0; CenterIdx < 6; CenterIdx++)
            {
                switch (CenterIdx)
                {
                    case 0:
                    case 2: { } break;
                    default: { continue; } break;
                }
                Piece& Curr = TheCube.Centers[CenterIdx];
                m4f PieceWorld = GetTurnMatrix(Curr.Turns);

                UpdateShaderWorld(GFXContext.ImmContext, &PieceWorld);
                int StartIx = Curr.ID * IxPerPiece;
                DrawMeshIxRange(GFXContext.ImmContext, *GFXContext.DrawStateColor, SpeedCubeMesh, StartIx, IxPerPiece);
            }
            for (int EdgeIdx = 0; EdgeIdx < 12; EdgeIdx++)
            {
                switch (EdgeIdx)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 6:
                    case 9: { } break;
                    default: { continue; } break;
                }
                Piece& Curr = TheCube.Edges[EdgeIdx];
                m4f PieceWorld = GetTurnMatrix(Curr.Turns);

                UpdateShaderWorld(GFXContext.ImmContext, &PieceWorld);
                int StartIx = Curr.ID * IxPerPiece;
                DrawMeshIxRange(GFXContext.ImmContext, *GFXContext.DrawStateColor, SpeedCubeMesh, StartIx, IxPerPiece);
            }
            for (int CornerIdx = 0; CornerIdx < 8; CornerIdx++)
            {
                switch (CornerIdx)
                {
                    case 0: {} break;
                        /*
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 6: { } break;
                        */
                    default: { continue; } break;
                }
                Piece& Curr = TheCube.Corners[CornerIdx];
                m4f PieceWorld = GetTurnMatrix(Curr.Turns);

                UpdateShaderWorld(GFXContext.ImmContext, &PieceWorld);
                int StartIx = Curr.ID * IxPerPiece;
                DrawMeshIxRange(GFXContext.ImmContext, *GFXContext.DrawStateColor, SpeedCubeMesh, StartIx, IxPerPiece);
            }
        }
    }

    void SpeedCube::Init()
    {
        LoadSpeedCubeMesh();
        TheCube.Init();
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

    const char* SpeedCubeState::GetIDName(int ID)
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

        switch (ID)
        {
            // Internal Core
            case 13: { return "Core"; } // All faces are internal
                   break;

            // Center 
            case 4: { return "White"; }
            case 10: { return "Blue"; }
            case 12: { return "Orange"; }
            case 14: { return "Red"; }
            case 16: { return "Green"; }
            case 22: { return "Yellow"; }
                   break;

            // Corner
            //  0 -> White/Blue/Orange
            //  2 -> White/Blue/Red
            //  6 -> White/Green/Orange
            //  8 -> White/Green/Red
            case 0: { return "White/Blue/Orange"; }
            case 2: { return "White/Blue/Red"; }
            case 6: { return "White/Green/Orange"; }
            case 8: { return "White/Green/Red"; }
            //  18 -> Yellow/Blue/Orange
            //  20 -> Yellow/Blue/Red
            //  24 -> Yellow/Green/Orange
            //  26 -> Yellow/Green/Red
            case 18: { return "Yellow/Blue/Orange"; }
            case 20: { return "Yellow/Blue/Red"; }
            case 24: { return "Yellow/Green/Orange"; }
            case 26: { return "Yellow/Green/Red"; }
                   break;

            // Edge
            //  1 -> White/Blue
            //  3 -> White/Orange
            //  5 -> White/Red
            //  7 -> White/Green
            case 1: { return "White/Blue"; }
            case 3: { return "White/Orange"; }
            case 5: { return "White/Red"; }
            case 7: { return "White/Green"; }
            //  9 -> Blue/Orange
            //  11 -> Blue/Red
            //  15 -> Green/Orange
            //  17 -> Green/Red
            case 9: { return "Blue/Orange"; }
            case 11: { return "Blue/Red"; }
            case 15: { return "Green/Orange"; }
            case 17: { return "Green/Red"; }
            //  19 -> Yellow/Blue
            //  21 -> Yellow/Orange
            //  23 -> Yellow/Red
            //  25 -> Yellow/Green
            case 19: { return "Yellow/Blue"; }
            case 21: { return "Yellow/Orange"; }
            case 23: { return "Yellow/Red"; }
            case 25: { return "Yellow/Green"; }
                   break;

            default: { ASSERT(false); } // shouldn't get here!
        }

        ASSERT(false); // shouldn't get here!

        return "ERROR";
    }

}

