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
                const RGBA BackgroundColor{ Norm8Bit(30, 30, 46) };
                const RGBA LineColor{ Norm8Bit(200, 200, 200) };
                RGBA CellColors[BLOCKTYPE_COUNT] =
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
            }

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
                        // TODO: Actually make this random
                        int RandomIdx = NewListIdx;
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

            struct GridPos
            {
                int Row = -1;
                int Col = -1;
            };

            struct ActivePiece
            {
                BlockType Type = BLOCK_NONE;
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
                void Init(BlockType InType, int StartRow, int StartCol)
                {
                    Type = InType;
                    Rotation = 0;
                    Row = StartRow;
                    Col = StartCol;
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
            };

            constexpr int GridWidth = 10;
            constexpr int GridHeight = 20;
            constexpr int GridSize = GridWidth * GridHeight;
            constexpr double SecondsPerFall = 1.0f;
            constexpr double SecondsPerFastFall = SecondsPerFall / 3.0f;
            struct Tetrion
            {
                static PieceBag Bag;
                static BlockType PlayField[GridSize];
                static ActivePiece Piece;
                static double LastFallTime;

                static bool CheckIdx(int Row, int Col);
                static bool CheckIdx(GridPos Pos);
                static BlockType& GetGridCell(int Row, int Col);
                static BlockType& GetGridCell(GridPos Pos);
                static bool CanFall();
                static void Fall();
                static void InitNextPiece();
                static void Init();
                static void Update();
            };
            PieceBag Tetrion::Bag{};
            BlockType Tetrion::PlayField[GridSize]{};
            ActivePiece Tetrion::Piece{};
            double Tetrion::LastFallTime =  0.0f;

            bool Tetrion::CheckIdx(int Row, int Col)
            {
                return (0 <= Row && Row < GridHeight) && (0 <= Col && Col < GridWidth);
            }

            bool Tetrion::CheckIdx(GridPos Pos)
            {
                return CheckIdx(Pos.Row, Pos.Col);
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
                Piece.Init(NextType, StartRow, StartCol);

                // Write piece to PlayField
                for (int BlockIdx = 0; BlockIdx < NumBlocks; BlockIdx++)
                {
                    GridPos Pos = Piece.BlockPos[BlockIdx];
                    BlockType& Cell = GetGridCell(Pos);
                    if (GetGridCell(Pos) != BLOCK_NONE) { /* Fail condition */ }
                    else { Cell = Piece.Type; }
                }
            }

            void Tetrion::Init()
            {
                for (int CellIdx = 0; CellIdx < GridSize; CellIdx++) { PlayField[CellIdx] = BLOCK_NONE; }

                LastFallTime = Clock::Time();

                InitNextPiece();
            }

            bool Tetrion::CanFall()
            {
                for (int BlockIdx = 0; BlockIdx < NumBlocks; BlockIdx++)
                {
                    GridPos CurrPos = Piece.BlockPos[BlockIdx];
                    GridPos NextPos{ CurrPos.Row + 1, CurrPos.Col };
                    if (CheckIdx(NextPos))
                    {
                        bool bNextEmpty = GetGridCell(NextPos) == BLOCK_NONE;
                        bool bNextSelf = Piece.IsPiece(NextPos);
                        if (!bNextSelf && !bNextEmpty) { return false; }
                    }
                    else
                    {
                        return false;
                    }
                }
                return true;
            }

            void Tetrion::Fall()
            {
                for (int BlockIdx = 0; BlockIdx < NumBlocks; BlockIdx++)
                {
                    GridPos CurrPos = Piece.BlockPos[BlockIdx];
                    GridPos NextPos{ CurrPos.Row + 1, CurrPos.Col };
                    ASSERT(CheckIdx(NextPos));
                    BlockType& CurrCell = GetGridCell(CurrPos);
                    ASSERT(CurrCell == Piece.Type);
                    CurrCell = BLOCK_NONE;
                    Piece.BlockPos[BlockIdx] = NextPos;
                }

                for (int BlockIdx = 0; BlockIdx < NumBlocks; BlockIdx++)
                {
                    GridPos NextPos = Piece.BlockPos[BlockIdx];
                    BlockType& NextCell = GetGridCell(NextPos);
                    NextCell = Piece.Type;
                    Piece.BlockPos[BlockIdx] = NextPos;
                }
            }

            void Tetrion::Update()
            {
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
                static bool bInit = false;
                if (!bInit) { Init(); bInit = true; }

                bool bInputLeft = KeyboardState::GetKeyState(LV_KEY_A);
                bool bInputRight = KeyboardState::GetKeyState(LV_KEY_D);
                if (bInputLeft != bInputRight)
                {
                    // TODO
                }

                bool bRotateLeft = KeyboardState::GetKeyState(LV_KEY_Q);
                bool bRotateRight = KeyboardState::GetKeyState(LV_KEY_E);
                if (bInputLeft != bRotateRight)
                {
                    // TODO
                }
                bool bSink = KeyboardState::GetKeyState(LV_KEY_W);
                bool bFastFall = KeyboardState::GetKeyState(LV_KEY_S);
                if (bSink)
                {
                    // TODO
                }
                else if (bFastFall)
                {
                    // TODO
                }

                double CurrTime = Clock::Time();
                if ((CurrTime - LastFallTime) > SecondsPerFall)
                {
                    LastFallTime = CurrTime;
                    if (CanFall()) { Fall(); }
                    else { InitNextPiece(); }
                }
            }
        }

        void DrawGrid(BatchDraw2D& Draw2D)
        {
            const float VisualGridHeight = Leviathan::AppHeight;
            const float VisualGridWidth = VisualGridHeight / 2.0f;
            const v2f VisualGridPos{ 100.0f, 0.0f };
            const float VisualCellSize = VisualGridHeight / (float)GridHeight;

            // Draw grid background
            Draw2D.AddQuad(QuadF{ VisualGridPos.X, VisualGridPos.Y, VisualGridWidth, VisualGridHeight }, BackgroundColor);

            // Draw cells
            for (int CellIdx = 0; CellIdx < GridSize; CellIdx++)
            {
                int CellCol = CellIdx % GridWidth;
                int CellRow = CellIdx / GridWidth;

                float CellX = VisualGridPos.X + CellCol * VisualCellSize;
                float CellY = AppHeight - (VisualGridPos.Y + CellRow * VisualCellSize) - VisualCellSize;
                //int ColorIdx = CellIdx % BLOCKTYPE_COUNT;
                //RGBA CellColor = CellColors[ColorIdx];
                RGBA CellColor = CellColors[Tetrion::PlayField[CellIdx]];

                Draw2D.AddQuad(QuadF{ CellX, CellY, VisualCellSize, VisualCellSize }, CellColor);
            }

            // Draw grid lines
            constexpr float LineThickness = 2.0f;
            constexpr float HalfLineThickness = LineThickness / 2.0f;
            for (int RowIdx = 0; RowIdx < GridHeight + 1; RowIdx++)
            {
                float LineY = VisualGridPos.Y + RowIdx * VisualCellSize - HalfLineThickness;
                Draw2D.AddQuad(QuadF{ VisualGridPos.X, LineY, VisualGridWidth, LineThickness }, LineColor);
            }
            for (int ColIdx = 0; ColIdx < GridWidth + 1; ColIdx++)
            {
                float LineX = VisualGridPos.X + ColIdx * VisualCellSize - HalfLineThickness;
                Draw2D.AddQuad(QuadF{ LineX, VisualGridPos.Y, LineThickness, VisualGridHeight }, LineColor);
            }
        }


        void UpdateAndDraw(BatchDraw2D& OutDraw2D)
        {
            Tetrion::Update();

            DrawGrid(OutDraw2D);
        }

        void Init()
        {
            Tetrion::Init();
        }

        void Term()
        {

        }
    }
}
