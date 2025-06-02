#include "Tetris.h"

namespace Game
{

namespace Tetris
{

using namespace Leviathan;

inline namespace TetrisState
{

enum BlockType
{
    BLOCK_NONE,
    BLOCK_I,
    BLOCK_O,
    BLOCK_T,
    BLOCK_S,
    BLOCK_Z,
    BLOCK_J,
    BLOCK_L,
    BLOCKTYPE_COUNT
};

constexpr int NumPieces = 7;
constexpr int NumBlocks = 4;

constexpr int OrientationI_Length = 4;
constexpr int OrientationO_Length = 2;
constexpr int Orientation_Length = 3;

constexpr int OrientationI_Size = OrientationI_Length * OrientationI_Length;
constexpr int OrientationO_Size = OrientationO_Length * OrientationO_Length;
constexpr int Orientation_Size = Orientation_Length * Orientation_Length;

BlockType Orientations_I[][OrientationI_Size] =
{
    {
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,
           BLOCK_I,    BLOCK_I,    BLOCK_I,    BLOCK_I,
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,
    },
    {
        BLOCK_NONE, BLOCK_NONE,    BLOCK_I, BLOCK_NONE,
        BLOCK_NONE, BLOCK_NONE,    BLOCK_I, BLOCK_NONE,
        BLOCK_NONE, BLOCK_NONE,    BLOCK_I, BLOCK_NONE,
        BLOCK_NONE, BLOCK_NONE,    BLOCK_I, BLOCK_NONE,
    },
    {
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,
           BLOCK_I,    BLOCK_I,    BLOCK_I,    BLOCK_I,
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,
    },
    {
        BLOCK_NONE,    BLOCK_I, BLOCK_NONE, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_I, BLOCK_NONE, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_I, BLOCK_NONE, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_I, BLOCK_NONE, BLOCK_NONE,
    },
};
BlockType Orientations_O[][OrientationO_Size] =
{
    {
        BLOCK_O, BLOCK_O,
        BLOCK_O, BLOCK_O,
    }
};
BlockType Orientations_T[][Orientation_Size] =
{
    {
        BLOCK_NONE,    BLOCK_T, BLOCK_NONE,
           BLOCK_T,    BLOCK_T,    BLOCK_T,
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE
    },
    {
        BLOCK_NONE,    BLOCK_T, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_T,    BLOCK_T,
        BLOCK_NONE,    BLOCK_T, BLOCK_NONE
    },
    {
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,
           BLOCK_T,    BLOCK_T,    BLOCK_T,
        BLOCK_NONE,    BLOCK_T, BLOCK_NONE
    },
    {
        BLOCK_NONE,    BLOCK_T, BLOCK_NONE,
           BLOCK_T,    BLOCK_T, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_T, BLOCK_NONE
    }
};
BlockType Orientations_S[][Orientation_Size] =
{
    {
        BLOCK_NONE,    BLOCK_S,    BLOCK_S,
           BLOCK_S,    BLOCK_S, BLOCK_NONE,
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE
    },
    {
        BLOCK_NONE,    BLOCK_S, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_S,    BLOCK_S,
        BLOCK_NONE, BLOCK_NONE,    BLOCK_S
    },
    {
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_S,    BLOCK_S,
           BLOCK_S,    BLOCK_S, BLOCK_NONE
    },
    {
           BLOCK_S, BLOCK_NONE, BLOCK_NONE,
           BLOCK_S,    BLOCK_S, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_S, BLOCK_NONE
    }
};
BlockType Orientations_Z[][Orientation_Size] =
{
    {
           BLOCK_Z,    BLOCK_Z, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_Z,    BLOCK_Z,
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE
    },
    {
        BLOCK_NONE, BLOCK_NONE,    BLOCK_Z,
        BLOCK_NONE,    BLOCK_Z,    BLOCK_Z,
        BLOCK_NONE,    BLOCK_Z, BLOCK_NONE
    },
    {
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,
           BLOCK_Z,    BLOCK_Z, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_Z,    BLOCK_Z
    },
    {
        BLOCK_NONE,    BLOCK_Z, BLOCK_NONE,
           BLOCK_Z,    BLOCK_Z, BLOCK_NONE,
           BLOCK_Z, BLOCK_NONE, BLOCK_NONE
    }
};
BlockType Orientations_J[][Orientation_Size] =
{
    {
           BLOCK_J, BLOCK_NONE, BLOCK_NONE,
           BLOCK_J,    BLOCK_J,    BLOCK_J,
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE
    },
    {
        BLOCK_NONE,    BLOCK_J,    BLOCK_J,
        BLOCK_NONE,    BLOCK_J, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_J, BLOCK_NONE
    },
    {
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,
           BLOCK_J,    BLOCK_J,    BLOCK_J,
        BLOCK_NONE, BLOCK_NONE,    BLOCK_J
    },
    {
        BLOCK_NONE,    BLOCK_J, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_J, BLOCK_NONE,
           BLOCK_J,    BLOCK_J, BLOCK_NONE
    }
};
BlockType Orientations_L[][Orientation_Size] =
{
    {
        BLOCK_NONE, BLOCK_NONE,    BLOCK_L,
           BLOCK_L,    BLOCK_L,    BLOCK_L,
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE
    },
    {
        BLOCK_NONE,    BLOCK_L, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_L, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_L,    BLOCK_L
    },
    {
        BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,
           BLOCK_L,    BLOCK_L,    BLOCK_L,
           BLOCK_L, BLOCK_NONE, BLOCK_NONE
    },
    {
           BLOCK_L,    BLOCK_L, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_L, BLOCK_NONE,
        BLOCK_NONE,    BLOCK_L, BLOCK_NONE
    }
};

inline namespace Colors
{

const fColor BackgroundColor{ Norm8Bit(30, 30, 46) };
const fColor LineColor{ Norm8Bit(200, 200, 200) };
fColor CellColors[BLOCKTYPE_COUNT] =
{
    BackgroundColor, // BLOCK_NONE
    Norm8Bit(156, 220, 254), // BLOCK_I / LIGHT BLUE
    Norm8Bit(220, 220, 170), // BLOCK_O / YELLOW
    Norm8Bit(203, 166, 247), // BLOCK_T / PURPLE
    Norm8Bit(78, 201, 176), // BLOCK_S / GREEN
    Norm8Bit(237, 104, 98), // BLOCK_Z / RED
    Norm8Bit(30, 30, 145), // BLOCK_J / BLUE
    Norm8Bit(250, 163, 100), // BLOCK_L / ORANGE
};

} // inline namespace Colors

static constexpr BlockType PieceList[NumPieces] = { BLOCK_I, BLOCK_O, BLOCK_T, BLOCK_S, BLOCK_Z, BLOCK_J, BLOCK_L };
struct PieceBag
{
    BlockType List[NumPieces] = {};
    int CurrIdx = NumPieces;

    void Shuffle()
    {
        BlockType PieceList[NumPieces] = { BLOCK_I, BLOCK_O, BLOCK_T, BLOCK_S, BLOCK_Z, BLOCK_J, BLOCK_L };

        int NewListIdx = 0;
        while (NewListIdx < NumPieces)
        {
            int RandomIdx = GetRandomInRange(0, NumPieces - 1);
            if (PieceList[RandomIdx] != BLOCK_NONE)
            {
                List[NewListIdx++] = PieceList[RandomIdx];
                PieceList[RandomIdx] = BLOCK_NONE;
            }
        }

        CurrIdx = 0;
    }
    BlockType GetNextPiece()
    {
        if (CurrIdx == NumPieces)
        {
            Shuffle();
        }
        return List[CurrIdx++];
    }
};

BlockType* GetOrientation(BlockType Type, int Rotation, int& OutLength, int& OutSize)
{
    int OrientationLength = Orientation_Length;
    int OrientationSize = Orientation_Size;
    if (Type == BLOCK_I)
    {
        OrientationLength = OrientationI_Length;
        OrientationSize = OrientationI_Size;
    }
    else if (Type == BLOCK_O)
    {
        OrientationLength = OrientationO_Length;
        OrientationSize = OrientationO_Size;
    }
    BlockType* Orientation = nullptr;
    switch (Type)
    {
        case BLOCK_I:
        {
            ASSERT(Rotation < ARRAY_SIZE(Orientations_I));
            if (Rotation < ARRAY_SIZE(Orientations_I))
            {
                Orientation = Orientations_I[Rotation];
            }
        } break;
        case BLOCK_O:
        {
            ASSERT(Rotation < ARRAY_SIZE(Orientations_O));
            if (Rotation < ARRAY_SIZE(Orientations_O))
            {
                Orientation = Orientations_O[Rotation];
            }
        } break;
        case BLOCK_T:
        {
            ASSERT(Rotation < ARRAY_SIZE(Orientations_T));
            if (Rotation < ARRAY_SIZE(Orientations_T))
            {
                Orientation = Orientations_T[Rotation];
            }
        } break;
        case BLOCK_S:
        {
            ASSERT(Rotation < ARRAY_SIZE(Orientations_S));
            if (Rotation < ARRAY_SIZE(Orientations_S))
            {
                Orientation = Orientations_S[Rotation];
            }
        } break;
        case BLOCK_Z:
        {
            ASSERT(Rotation < ARRAY_SIZE(Orientations_Z));
            if (Rotation < ARRAY_SIZE(Orientations_Z))
            {
                Orientation = Orientations_Z[Rotation];
            }
        } break;
        case BLOCK_J:
        {
            ASSERT(Rotation < ARRAY_SIZE(Orientations_J));
            if (Rotation < ARRAY_SIZE(Orientations_J))
            {
                Orientation = Orientations_J[Rotation];
            }
        } break;
        case BLOCK_L:
        {
            ASSERT(Rotation < ARRAY_SIZE(Orientations_L));
            if (Rotation < ARRAY_SIZE(Orientations_L))
            {
                Orientation = Orientations_L[Rotation];
            }
        } break;
        default: { ASSERT(false); } break;
    }

    OutLength = OrientationLength;
    OutSize = OrientationSize;
    return Orientation;
}

int GetNumOrientations(BlockType Type)
{
    int NumOrientations = 0;
    switch (Type)
    {
        case BLOCK_I: { NumOrientations = ARRAY_SIZE(Orientations_I); } break;
        case BLOCK_O: { NumOrientations = ARRAY_SIZE(Orientations_O); } break;
        case BLOCK_T: { NumOrientations = ARRAY_SIZE(Orientations_T); } break;
        case BLOCK_S: { NumOrientations = ARRAY_SIZE(Orientations_S); } break;
        case BLOCK_Z: { NumOrientations = ARRAY_SIZE(Orientations_Z); } break;
        case BLOCK_J: { NumOrientations = ARRAY_SIZE(Orientations_J); } break;
        case BLOCK_L: { NumOrientations = ARRAY_SIZE(Orientations_L); } break;
        default: { ASSERT(false); } break;
    }
    ASSERT(NumOrientations > 0);
    return NumOrientations;
}

int GetNextOrientationIdx(BlockType Type, int Rotation, int Offset)
{
    int NumOrientations = GetNumOrientations(Type);
    int NextOrientationIdx = (Rotation + Offset + NumOrientations) % NumOrientations;
    return NextOrientationIdx;
}

BlockType* GetNextOrientation(BlockType Type, int Rotation, int& OutLength, int& OutSize, int Offset)
{
    return GetOrientation(Type, GetNextOrientationIdx(Type, Rotation, Offset), OutLength, OutSize);
}

struct GridPos
{
    int Row = -1;
    int Col = -1;
};

struct ActivePiece
{
    BlockType Type = BLOCK_NONE;
    bool bActive = false;
    int Rotation = 0;
    int Row = 0;
    int Col = 0;
    GridPos BlockPos[NumBlocks];

    bool IsPiece(GridPos Pos)
    {
        for (int BlockIdx = 0; BlockIdx < NumBlocks; BlockIdx++)
        {
            int Row = BlockPos[BlockIdx].Row;
            int Col = BlockPos[BlockIdx].Col;
            if (Row == Pos.Row && Col == Pos.Col) { return true; }
        }
        return false;
    }
    void Init(BlockType InType, int InRow, int InCol, int InRotation)
    {
        Type = InType;
        bActive = true;
        Rotation = InRotation;
        Row = InRow;
        Col = InCol;
        { // Init BlockPos
            int OrientationLength = 0;
            int OrientationSize = 0;
            BlockType* Orientation = GetOrientation(Type, Rotation, OrientationLength, OrientationSize);
            ASSERT(OrientationLength > 0);
            ASSERT(OrientationSize > 0);
            ASSERT(Orientation);

            int BlockIdx = 0;
            for (int OriIdx = 0; OriIdx < OrientationSize && BlockIdx < NumBlocks; OriIdx++)
            {
                BlockType Value = Orientation[OriIdx];
                if (Value != BLOCK_NONE)
                {
                    int OriRow = OriIdx / OrientationLength;
                    int OriCol = OriIdx % OrientationLength;
                    BlockPos[BlockIdx].Row = Row + OriRow;
                    BlockPos[BlockIdx].Col = Col + OriCol;
                    BlockIdx++;
                }
            }
        }
    }
    void Move(int RowOffset, int ColOffset)
    {
        Row += RowOffset;
        Col += ColOffset;
        for (int BlockIdx = 0; BlockIdx < NumBlocks; BlockIdx++)
        {
            BlockPos[BlockIdx].Row += RowOffset;
            BlockPos[BlockIdx].Col += ColOffset;
        }
    }
};

constexpr int GridWidth = 10;
constexpr int GridHeight = 20;
constexpr int GridSize = GridWidth * GridHeight;
//constexpr double SecondsPerFall = 1.0f;
constexpr double SecondsPerFall = 0.25f;
constexpr double SecondsPerFastFall = SecondsPerFall / 3.0f;
constexpr double SecondsPerMove = SecondsPerFall / 4.0f;
constexpr double SecondsPerRotate = SecondsPerFall / 2.0f;

enum TetrisGameState : int
{
    ATTRACT,
    PAUSED,
    PLAY,
    STATE_COUNT,
};

struct TetrisInput
{
    bool bMoveLeft = false;
    bool bMoveRight = false;
    bool bRotateLeft = false;
    bool bRotateRight = false;

    double LastMoveTime = 0.0;
    double LastRotateTime = 0.0;

    void Process();
};

void TetrisInput::Process()
{
    bMoveLeft = false;
    bMoveRight = false;
    bRotateLeft = false;
    bRotateRight = false;
    double CurrTime = Clock::Time();
    bool bKeyMoveLeftPressed = KeyboardState::GetKey(LV_KEY_A);
    bool bKeyMoveRightPressed = KeyboardState::GetKey(LV_KEY_D);
    bool bKeyMoveLeftHeld = KeyboardState::GetKey(LV_KEY_A, true);
    bool bKeyMoveRightHeld = KeyboardState::GetKey(LV_KEY_D, true);
    bool bKeyRotateLeftPressed = KeyboardState::GetKey(LV_KEY_Q);
    bool bKeyRotateRightPressed = KeyboardState::GetKey(LV_KEY_E);
    if ((bKeyMoveLeftPressed || bKeyMoveRightPressed) ||
        (bKeyMoveLeftHeld != bKeyMoveRightHeld && (CurrTime - LastMoveTime) > SecondsPerMove))
    {
        bMoveLeft = bKeyMoveLeftPressed || bKeyMoveLeftHeld;
        bMoveRight = bKeyMoveRightPressed || bKeyMoveRightHeld;
        if (bMoveLeft) { LastMoveTime = bKeyMoveLeftPressed ? (CurrTime + SecondsPerMove) : CurrTime; }
        else if (bMoveRight) { LastMoveTime = bKeyMoveRightPressed ? (CurrTime + SecondsPerMove) : CurrTime; }
    }
    if (bKeyRotateLeftPressed != bKeyRotateRightPressed)
    {
        bRotateLeft = bKeyRotateLeftPressed;
        bRotateRight = bKeyRotateRightPressed;
        LastRotateTime = CurrTime;
    }
}

struct Tetrion
{
    static TetrisGameState State;
    static TetrisInput Input;

    static PieceBag Bag;
    static BlockType PlayField[GridSize];
    static ActivePiece Piece;
    static double LastFallTime;

    static bool CheckIdx(int Row, int Col);
    static bool CheckIdx(GridPos Pos);
    static bool CheckCollision(int Row, int Col, BlockType Type, int Rotation);
    static BlockType& GetGridCell(int Row, int Col);
    static BlockType& GetGridCell(GridPos Pos);
    static bool IsLineFull(int Row);
    static void ClearRow(int Row);
    static void ClearFullLines();
    static bool CanFall();
    static void Fall();
    static void RotateLeft();
    static void RotateRight();
    static void Settle();
    static void InitNextPiece();
    static void Init();
    static void ClearGrid();
    static void Update();
    static void ProcessInput();
    static void DrawGrid(BatchDrawCmds& Draw2D);
};

TetrisGameState Tetrion::State = TetrisGameState::ATTRACT;
TetrisInput Tetrion::Input{};
PieceBag Tetrion::Bag{};
BlockType Tetrion::PlayField[GridSize]{};
ActivePiece Tetrion::Piece{};
double Tetrion::LastFallTime = 0.0f;

bool Tetrion::CheckIdx(int Row, int Col)
{
    return (0 <= Row && Row < GridHeight) && (0 <= Col && Col < GridWidth);
}

bool Tetrion::CheckIdx(GridPos Pos)
{
    return CheckIdx(Pos.Row, Pos.Col);
}

bool Tetrion::CheckCollision(int Row, int Col, BlockType Type, int Rotation)
{
    int OrientationLength = 0;
    int OrientationSize = 0;
    BlockType* Orientation = GetOrientation(Type, Rotation, OrientationLength, OrientationSize);
    ASSERT(OrientationLength > 0);
    ASSERT(OrientationSize > 0);
    ASSERT(Orientation);

    for (int OriIdx = 0; OriIdx < OrientationSize; OriIdx++)
    {
        BlockType Value = Orientation[OriIdx];
        if (Value != BLOCK_NONE)
        {
            int CurrRow = Row + (OriIdx / OrientationLength);
            int CurrCol = Col + (OriIdx % OrientationLength);
            if (!CheckIdx(CurrRow, CurrCol))
            {
                return true;
            }
            if (GetGridCell(CurrRow, CurrCol) != BLOCK_NONE)
            {
                return true;
            }
        }
    }

    return false;
}

BlockType& Tetrion::GetGridCell(int Row, int Col)
{
    ASSERT(0 <= Row && Row < GridHeight);
    ASSERT(0 <= Col && Col < GridWidth);
    int Idx = Row * GridWidth + Col;
    return PlayField[Idx];
}

BlockType& Tetrion::GetGridCell(GridPos Pos)
{
    return GetGridCell(Pos.Row, Pos.Col);
}

void Tetrion::InitNextPiece()
{
    BlockType NextType = Bag.GetNextPiece();

    int StartRow = 0;
    int StartCol = 5;
    Piece.Init(NextType, StartRow, StartCol, 0);

    for (int BlockIdx = 0; BlockIdx < NumBlocks; BlockIdx++)
    {
        GridPos Pos = Piece.BlockPos[BlockIdx];
        BlockType& Cell = GetGridCell(Pos);
        // Game over condition
        if (GetGridCell(Pos) != BLOCK_NONE)
        {
            State = TetrisGameState::ATTRACT;
            Init();
            return;
        }
    }
}

void Tetrion::Init()
{
    switch (State)
    {
        case TetrisGameState::ATTRACT:
        {
            ClearGrid();
        } break;
        case TetrisGameState::PAUSED:
        {
            return;
        } break;
        case TetrisGameState::PLAY:
        {
            ClearGrid();
            LastFallTime = Clock::Time();
            InitNextPiece();
        } break;
    }
}

void Tetrion::ClearGrid()
{
    for (int CellIdx = 0; CellIdx < GridSize; CellIdx++) { PlayField[CellIdx] = BLOCK_NONE; }
    Piece.bActive = false;
}

bool Tetrion::IsLineFull(int Row)
{
    ASSERT(CheckIdx(Row, 0));
    for (int ColIdx = 0; ColIdx < GridWidth; ColIdx++)
    {
        if (GetGridCell(Row, ColIdx) == BLOCK_NONE) { return false; }
    }
    return true;
}

void Tetrion::ClearRow(int Row)
{
    ASSERT(CheckIdx(Row, 0));
    for (int RowIdx = Row; RowIdx >= 1; RowIdx--)
    {
        for (int ColIdx = 0; ColIdx < GridWidth; ColIdx++)
        {
            BlockType& Cell = GetGridCell(RowIdx, ColIdx);
            BlockType& AboveCell = GetGridCell(RowIdx - 1, ColIdx);
            Cell = AboveCell;
        }
    }
}

void Tetrion::ClearFullLines()
{
    int NumRowsCleared = 0;
    for (int RowIdx = GridHeight - 1; RowIdx >= 0 && RowIdx >= NumRowsCleared; RowIdx--)
    {
        if (IsLineFull(RowIdx))
        {
            ClearRow(RowIdx++);
            NumRowsCleared++;
        }
    }
}

bool Tetrion::CanFall()
{
    return !CheckCollision(Piece.Row + 1, Piece.Col, Piece.Type, Piece.Rotation);
}

void Tetrion::Fall()
{
    Piece.Move(+1, 0);
}

void Tetrion::RotateLeft()
{
    int NextOrientationIdx = GetNextOrientationIdx(Piece.Type, Piece.Rotation, -1);
    ASSERT(!CheckCollision(Piece.Row, Piece.Col, Piece.Type, NextOrientationIdx));
    Piece.Init(Piece.Type, Piece.Row, Piece.Col, NextOrientationIdx);
}

void Tetrion::RotateRight()
{
    int NextOrientationIdx = GetNextOrientationIdx(Piece.Type, Piece.Rotation, +1);
    ASSERT(!CheckCollision(Piece.Row, Piece.Col, Piece.Type, NextOrientationIdx));
    Piece.Init(Piece.Type, Piece.Row, Piece.Col, NextOrientationIdx);
}

void Tetrion::Settle()
{
    for (int BlockIdx = 0; BlockIdx < NumBlocks; BlockIdx++)
    {
        GridPos Pos = Piece.BlockPos[BlockIdx];
        BlockType& Cell = GetGridCell(Pos);
        ASSERT(GetGridCell(Pos) == BLOCK_NONE);
        Cell = Piece.Type;
    }

    ClearFullLines();

    InitNextPiece();
}

/* NOTE:
    - For the 'current piece'
        - Check if piece is timed to fall
            - If can fall... Fall
            - If blocked... Stick
        - Check for player input
            - Move input: Check for collision -> move if valid
            - Orientation input: Check for collision w/ next orientation -> turn if valid
            - FastFall input: Same as fall
            - Lock input: Drop piece to lowest possible point, then Stick
    - If need next piece:
        - If more pieces in current bag, select next random piece
        - If no more pieces, shuffle new bag
*/
void Tetrion::Update()
{
    switch (State)
    {
        case TetrisGameState::ATTRACT:
        {
            if (KeyboardState::GetKey(LV_KEY_ENTER))
            {
                State = TetrisGameState::PLAY;
                Init();
            }
            return;
        } break;
        case TetrisGameState::PAUSED:
        {
            if (KeyboardState::GetKey(LV_KEY_BACKSPACE))
            {
                State = TetrisGameState::ATTRACT;
                Init();
            }
            if (KeyboardState::GetKey(LV_KEY_ENTER))
            {
                State = TetrisGameState::PLAY;
            }
            return;
        } break;
        case TetrisGameState::PLAY:
        {
            if (KeyboardState::GetKey(LV_KEY_BACKSPACE))
            {
                State = TetrisGameState::ATTRACT;
                Init();
            }
            if (KeyboardState::GetKey(LV_KEY_ENTER))
            {
                State = TetrisGameState::PAUSED;
                Init();
            }
        } break;
    }

    Input.Process();

    if (Input.bMoveLeft)
    {
        bool bCanMoveLeft = !CheckCollision(Piece.Row, Piece.Col - 1, Piece.Type, Piece.Rotation);
        if (bCanMoveLeft) { Piece.Move(0, -1); }
    }
    else if (Input.bMoveRight)
    {
        bool bCanMoveRight = !CheckCollision(Piece.Row, Piece.Col + 1, Piece.Type, Piece.Rotation);
        if (bCanMoveRight) { Piece.Move(0, +1); }
    }

    if (Input.bRotateLeft || Input.bRotateRight)
    {
        int LeftOrientationIdx = GetNextOrientationIdx(Piece.Type, Piece.Rotation, -1);
        bool bCanRotateLeft = !CheckCollision(Piece.Row, Piece.Col, Piece.Type, LeftOrientationIdx);
        int RightOrientationIdx = GetNextOrientationIdx(Piece.Type, Piece.Rotation, +1);
        bool bCanRotateRight = !CheckCollision(Piece.Row, Piece.Col, Piece.Type, RightOrientationIdx);
        if (Input.bRotateLeft && bCanRotateLeft) { RotateLeft(); }
        else if (Input.bRotateRight && bCanRotateRight) { RotateRight(); }
        else { if (bCanRotateLeft) { RotateLeft(); } else if (bCanRotateRight) { RotateRight(); } }
    }
    bool bSink = KeyboardState::GetKey(LV_KEY_W);
    bool bFastFall = KeyboardState::GetKey(LV_KEY_S, true);
    if (bSink)
    {
        while (CanFall()) { Fall(); }
    }

    double CurrTime = Clock::Time();
    double CurrSecondsPerFall = bFastFall ? SecondsPerFastFall : SecondsPerFall;
    if ((CurrTime - LastFallTime) > CurrSecondsPerFall)
    {
        LastFallTime = CurrTime;
        if (CanFall()) { Fall(); }
        else { Settle(); }
    }
}

} // inline namespace TetrisState

void Tetrion::DrawGrid(BatchDrawCmds& Draw2D)
{
    const float VisualGridHeight = Leviathan::AppHeight;
    const float VisualGridWidth = VisualGridHeight / 2.0f;
    const v2f VisualGridPos{ 100.0f, 0.0f };
    const float VisualCellSize = VisualGridHeight / (float)GridHeight;

    // Draw grid background
    Draw2D.AddRect(RectF{ VisualGridPos.X, VisualGridPos.Y, VisualGridWidth, VisualGridHeight }, BackgroundColor);

    // Draw cells
    for (int CellIdx = 0; CellIdx < GridSize; CellIdx++)
    {
        int CellCol = CellIdx % GridWidth;
        int CellRow = CellIdx / GridWidth;

        float CellX = VisualGridPos.X + CellCol * VisualCellSize;
        float CellY = AppHeight - (VisualGridPos.Y + CellRow * VisualCellSize) - VisualCellSize;
        fColor CellColor = CellColors[Tetrion::PlayField[CellIdx]];

        Draw2D.AddRect(RectF{ CellX, CellY, VisualCellSize, VisualCellSize }, CellColor);
    }

    // Draw Player Piece
    if (Piece.bActive)
    {
        fColor PlayerColor = CellColors[Piece.Type];
        for (int BlockIdx = 0; BlockIdx < NumBlocks; BlockIdx++)
        {
            GridPos Pos = Piece.BlockPos[BlockIdx];
            if (CheckIdx(Pos))
            {
                float CellX = VisualGridPos.X + (Pos.Col * VisualCellSize);
                float CellY = AppHeight - (VisualGridPos.Y + (Pos.Row * VisualCellSize) + VisualCellSize);
                Draw2D.AddRect(RectF{ CellX, CellY, VisualCellSize, VisualCellSize }, PlayerColor);
            }
        }
    }

    // Draw grid lines
    constexpr float LineThickness = 2.0f;
    constexpr float HalfLineThickness = LineThickness / 2.0f;
    for (int RowIdx = 0; RowIdx < GridHeight + 1; RowIdx++)
    {
        float LineY = VisualGridPos.Y + RowIdx * VisualCellSize - HalfLineThickness;
        Draw2D.AddRect(RectF{ VisualGridPos.X, LineY, VisualGridWidth, LineThickness }, LineColor);
    }
    for (int ColIdx = 0; ColIdx < GridWidth + 1; ColIdx++)
    {
        float LineX = VisualGridPos.X + ColIdx * VisualCellSize - HalfLineThickness;
        Draw2D.AddRect(RectF{ LineX, VisualGridPos.Y, LineThickness, VisualGridHeight }, LineColor);
    }
}

void Update()
{
    Tetrion::Update();
}

void Draw(LvGFXContext& GFXContext)
{
    BatchDrawCmds& Draw2D = *GFXContext.DrawBatch;
    Tetrion::DrawGrid(Draw2D);
}

void Init()
{
    Tetrion::Init();
}

void Term()
{

}

} // namespace Tetris

} // namespace Game

