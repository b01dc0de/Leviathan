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
    int Turns[3] = { 0, 0, 0 };
    m4f World = m4f::Identity();
};

struct TurnGroup
{
    // Group[0]: Center
    // Group[1, 4]: Edges
    // Group[5, 8]: Corners
    int Axis;
    bool bAxisSign;
    bool bClockWise;
    Piece* Group[9];

    void Turn()
    {
        for (int GroupIdx = 0; GroupIdx < 9; GroupIdx++)
        {
            float TurnRadians = fPI / 2.0f;
            m4f TurnWorld = m4f::Identity();
            if (bAxisSign == bClockWise) {
                Group[GroupIdx]->Turns[Axis]++;
                switch (Axis)
                {
                    case 0: TurnWorld = m4f::RotAxisX(TurnRadians); break;
                    case 1: TurnWorld = m4f::RotAxisY(TurnRadians); break;
                    case 2: TurnWorld = m4f::RotAxisZ(TurnRadians); break;
                }
            }
            else {
                Group[GroupIdx]->Turns[Axis]--;
                switch (Axis)
                {
                    case 0: TurnWorld = m4f::RotAxisX(-TurnRadians); break;
                    case 1: TurnWorld = m4f::RotAxisY(-TurnRadians); break;
                    case 2: TurnWorld = m4f::RotAxisZ(-TurnRadians); break;
                }
            }
            Group[GroupIdx]->World = Group[GroupIdx]->World * TurnWorld;
        }
        if (bClockWise)
        {
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
        else
        {
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
    }
};

enum TurnType
{
    Turn_Top,
    Turn_Bot,
    Turn_Left,
    Turn_Right,
    Turn_Front,
    Turn_Back
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

    bool bTurning;
    TurnGroup ActiveTurn;
    int TurnStepIdx;
    float LastTurnStep;
    static constexpr int NumTurnSteps = 32;
    static constexpr float SecondsPerTurn = 0.125f;
    static constexpr float SecondsPerTurnStep = SecondsPerTurn / NumTurnSteps;

    static constexpr int MvHistSize = 20;
    int MoveHistory[MvHistSize];
    int HistIdx;
    bool HistDir;

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
        bTurning = false;
        ActiveTurn = {};
        HistIdx = 0;
        HistDir = true;
    }
    void DebugPrint()
    {
        char PrintBuffer[1024];
        sprintf_s(PrintBuffer, "Top Row: \n\t%s\t%s\t%s\n\t%s\t%s\t%s\n\t%s\t%s\t%s\n",
            GetIDName(Corners[0].ID), GetIDName(Edges[0].ID), GetIDName(Corners[1].ID),
            GetIDName(Edges[1].ID), GetIDName(Centers[0].ID), GetIDName(Edges[2].ID),
            GetIDName(Corners[2].ID), GetIDName(Edges[3].ID), GetIDName(Corners[3].ID)
        );
        OutputDebugStringA(PrintBuffer);
    }
    void Turn_Top(bool bClockWise)
    {
        ASSERT(!bTurning);
        if (bTurning) { return; }
        ActiveTurn =
        {
            1, true, bClockWise,
            &Centers[0],
            &Edges[0], &Edges[1],
            &Edges[3], &Edges[2],
            &Corners[0], &Corners[2],
            &Corners[3], &Corners[1]
        };
        bTurning = true;
        TurnStepIdx = 0;
        LastTurnStep = Clock::Time();
    }
    void Turn_Left(bool bClockWise)
    {
        ASSERT(!bTurning);
        if (bTurning) { return; }
        ActiveTurn =
        {
            0, false, bClockWise,
            &Centers[2],
            &Edges[1], &Edges[4],
            &Edges[9], &Edges[6],
            &Corners[0], &Corners[4],
            &Corners[6], &Corners[2]
        };
        bTurning = true;
        TurnStepIdx = 0;
        LastTurnStep = Clock::Time();
    };
    void Turn_Right(bool bClockWise)
    {
        ASSERT(!bTurning);
        if (bTurning) { return; }
        ActiveTurn =
        {
            0, true, bClockWise,
            &Centers[3],
            &Edges[2], &Edges[7],
            &Edges[10], &Edges[5],
            &Corners[1], &Corners[3],
            &Corners[7], &Corners[5],
        };
        bTurning = true;
        TurnStepIdx = 0;
        LastTurnStep = Clock::Time();
    };
    void Turn_Front(bool bClockWise)
    {
        ASSERT(!bTurning);
        if (bTurning) { return; }
        ActiveTurn =
        {
            2, true, bClockWise,
            &Centers[4],
            &Edges[3], &Edges[6],
            &Edges[11], &Edges[7],
            &Corners[2], &Corners[6],
            &Corners[7], &Corners[3],
        };
        bTurning = true;
        TurnStepIdx = 0;
        LastTurnStep = Clock::Time();
    };
    void Turn_Bot(bool bClockWise)
    {
        ASSERT(!bTurning);
        if (bTurning) { return; }
        ActiveTurn =
        {
            1, false, bClockWise,
            &Centers[5],
            &Edges[8], &Edges[10],
            &Edges[11], &Edges[9],
            &Corners[4], &Corners[5],
            &Corners[7], &Corners[6],
        };
        bTurning = true;
        TurnStepIdx = 0;
        LastTurnStep = Clock::Time();
    };
    void Turn_Back(bool bClockWise)
    {
        ASSERT(!bTurning);
        if (bTurning) { return; }
        ActiveTurn =
        {
            2, false, bClockWise,
            &Centers[1],
            &Edges[0], &Edges[5],
            &Edges[8], &Edges[4],
            &Corners[0], &Corners[1],
            &Corners[5], &Corners[4],
        };
        bTurning = true;
        TurnStepIdx = 0;
        LastTurnStep = Clock::Time();
    }
    void TurnStep()
    {
        float fTime = (float)Clock::Time();
        if ((fTime - LastTurnStep) > SecondsPerTurnStep)
        {
            LastTurnStep = fTime;
            TurnStepIdx++;
            if (TurnStepIdx >= NumTurnSteps)
            {
                ActiveTurn.Turn();
                ActiveTurn = {};
                TurnStepIdx = 0;
                bTurning = false;
            }
        }
    }
    m4f GetActiveTurnRot()
    {
        if (!bTurning)
        {
            ASSERT(false);
            return m4f::Identity();
        }
        else
        {
            float TurnRadians = (float)TurnStepIdx / (float)NumTurnSteps * fPI / 2.0f;
            if (ActiveTurn.bAxisSign != ActiveTurn.bClockWise)
            {
                TurnRadians = -TurnRadians;
            }
            switch (ActiveTurn.Axis)
            {
                case 0: return m4f::RotAxisX(TurnRadians); break;
                case 1: return m4f::RotAxisY(TurnRadians); break;
                case 2: return m4f::RotAxisZ(TurnRadians); break;
            }
        }
        return m4f::Identity();
    }
    bool PieceIsTurning(int PieceIdx)
    {
        if (!bTurning) return false;
        else
        {
            for (int GroupIdx = 0; GroupIdx < 9; GroupIdx++)
            {
                if (PieceIdx == ActiveTurn.Group[GroupIdx]->ID)
                {
                    return true;
                }
            }
        }
        return false;
    }
    void DoTurn(int TurnType, bool bCW)
    {
        switch (TurnType)
        {
            case 0: { Turn_Top(bCW); return; }
            case 1: { Turn_Bot(bCW); return; }
            case 2: { Turn_Left(bCW); return; }
            case 3: { Turn_Right(bCW); return; }
            case 4: { Turn_Front(bCW); return; }
            case 5: { Turn_Back(bCW); return; }
        }
        ASSERT(false);
    };
    void Update()
    {
        static bool bAutoTurn = true;
        if (bTurning)
        {
            TurnStep();
        }
        else if (bAutoTurn)
        {
            static constexpr float WaitTime = 1.0f;
            static float LastWait = Clock::Time();
            static bool bWaiting = true;

            if (bWaiting)
            {
                bWaiting = (Clock::Time() - LastWait) < WaitTime;
                return;
            }

            if (HistDir && HistIdx < MvHistSize)
            {
                constexpr int EncodedMoveMax = 11; // 11 (6 * 2 - 1)
                int EncodedMove;
                do { EncodedMove = GetRandomInRange(0, EncodedMoveMax); } while (HistIdx > 0 && (MoveHistory[HistIdx - 1] / 2) == (EncodedMove / 2));
                int TurnType = EncodedMove / 2;
                bool bCW = EncodedMove % 2 == 0;
                DoTurn(TurnType, bCW);
                MoveHistory[HistIdx++] = EncodedMove;
            }
            else if (!HistDir && HistIdx >= 0)
            {
                int EncodedMove = MoveHistory[HistIdx--];
                int TurnType = EncodedMove / 2;
                bool bCW = EncodedMove % 2 == 0;
                DoTurn(TurnType, !bCW);
            }
            else
            {
                HistIdx = HistDir ? MvHistSize - 1 : 0;
                HistDir = !HistDir;
                bWaiting = HistDir;
                LastWait = Clock::Time();
            }
        }
        else
        {
            bool bCCW = KeyboardState::GetKey(LV_KEY_SHIFT, true);
            if (KeyboardState::GetKey(LV_KEY_W))
            {
                Turn_Top(!bCCW);
            }
            else if (KeyboardState::GetKey(LV_KEY_Q))
            {
                Turn_Left(!bCCW);
            }
            else if (KeyboardState::GetKey(LV_KEY_S))
            {
                Turn_Front(!bCCW);
            }
            else if (KeyboardState::GetKey(LV_KEY_E))
            {
                Turn_Right(!bCCW);
            }
            else if (KeyboardState::GetKey(LV_KEY_A))
            {
                Turn_Bot(!bCCW);
            }
            else if (KeyboardState::GetKey(LV_KEY_D))
            {
                Turn_Back(!bCCW);
            }
        }
    }
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

    VxColor* SpeedCubeVxs = new VxColor[NumPieces * ARRAY_SIZE(Vertices_CubeFacesColor)];
    unsigned int* SpeedCubeIxs = new unsigned int[NumPieces * ARRAY_SIZE(Indices_CubeFaces)];

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
            SpeedCubeVxs[PieceIdx * ARRAY_SIZE(Vertices_CubeFacesColor) + VxIdx] =
            {
                Vertices_CubeFacesColor[VxIdx].Pos + vOffset, // v4f Pos
                Vertices_CubeFacesColor[VxIdx].Col, // v4f Col
            };

            constexpr int VxPerFace = 4;
            int FaceIdx = VxIdx / VxPerFace;
            if (!FaceIsExternal(PieceIdx, FaceIdx))
            {
                SpeedCubeVxs[PieceIdx * ARRAY_SIZE(Vertices_CubeFacesColor) + VxIdx].Col = { 0.0f, 0.0f, 0.0f, 1.0f };
            }
        }

        int PieceFirstIx = ARRAY_SIZE(Vertices_CubeFacesColor) * PieceIdx;
        for (int IxIdx = 0; IxIdx < ARRAY_SIZE(Indices_CubeFaces); IxIdx++)
        {
            SpeedCubeIxs[PieceIdx * ARRAY_SIZE(Indices_CubeFaces) + IxIdx] = Indices_CubeFaces[IxIdx] + PieceFirstIx;
        }
    }

    IxPerPiece = ARRAY_SIZE(Indices_CubeFaces);

    SpeedCubeMesh = CreateMeshState
    (
        Graphics::Device(),
        sizeof(VxColor),
        NumPieces * ARRAY_SIZE(Vertices_CubeFacesColor),
        SpeedCubeVxs,
        NumPieces * ARRAY_SIZE(Indices_CubeFaces),
        SpeedCubeIxs
    );

    delete[] SpeedCubeVxs;
    delete[] SpeedCubeIxs;
}

using namespace SpeedCubeState;

void SpeedCube::Update()
{
    TheCube.Update();
}

void SpeedCube::Draw(LvGFXContext& GFXContext)
{
    GFXContext.ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    GFXContext.SetShaderConstantBuffers_WVP();
    GFXContext.UpdateShaderViewProj(GFXContext.GameCamera);

    m4f CubeWorld = m4f::Identity();

    static bool bEnableCubeAutoRotate = true;
    if (bEnableCubeAutoRotate)
    {
        float CubeRotX = Clock::Time() * 2.0f;
        float CubeRotY = Clock::Time();
        CubeWorld = m4f::RotAxisY(CubeRotY) * m4f::RotAxisX(CubeRotX);
    }

    // Draw each cube piece
    {
        for (int CenterIdx = 0; CenterIdx < 6; CenterIdx++)
        {
            Piece& Curr = TheCube.Centers[CenterIdx];
            m4f PieceWorld = Curr.World;
            if (TheCube.PieceIsTurning(Curr.ID))
            {
                PieceWorld = PieceWorld * TheCube.GetActiveTurnRot();
            }
            PieceWorld = PieceWorld * CubeWorld;

            GFXContext.UpdateShaderWorld(&PieceWorld);
            int StartIx = Curr.ID * IxPerPiece;
            DrawMeshIxRange(GFXContext.ImmContext, *GFXContext.DrawStateColor, SpeedCubeMesh, StartIx, IxPerPiece);
        }
        for (int EdgeIdx = 0; EdgeIdx < 12; EdgeIdx++)
        {
            Piece& Curr = TheCube.Edges[EdgeIdx];
            m4f PieceWorld = Curr.World;
            if (TheCube.PieceIsTurning(Curr.ID))
            {
                PieceWorld = PieceWorld * TheCube.GetActiveTurnRot();
            }
            PieceWorld = PieceWorld * CubeWorld;

            GFXContext.UpdateShaderWorld(&PieceWorld);
            int StartIx = Curr.ID * IxPerPiece;
            DrawMeshIxRange(GFXContext.ImmContext, *GFXContext.DrawStateColor, SpeedCubeMesh, StartIx, IxPerPiece);
        }
        for (int CornerIdx = 0; CornerIdx < 8; CornerIdx++)
        {
            Piece& Curr = TheCube.Corners[CornerIdx];
            m4f PieceWorld = Curr.World;
            if (TheCube.PieceIsTurning(Curr.ID))
            {
                PieceWorld = PieceWorld * TheCube.GetActiveTurnRot();
            }
            PieceWorld = PieceWorld * CubeWorld;
            GFXContext.UpdateShaderWorld(&PieceWorld);
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
    }

    ASSERT(false); // shouldn't get here!
    return false;
};

const char* SpeedCubeState::GetIDName(int ID)
{
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
    }

    ASSERT(false); // shouldn't get here!
    return "ERROR";
}

}

