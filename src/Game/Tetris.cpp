#include "Tetris.h"

namespace Game
{
    namespace Tetris
    {
        using namespace Leviathan;

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

        constexpr int GridWidth = 10;
        constexpr int GridHeight = 20;
        constexpr int GridSize = GridWidth * GridHeight;
        BlockType PlayField[GridSize];

        RGBA BackgroundColor{ Norm8Bit(30, 30, 46) };
        RGBA LineColor{ Norm8Bit(200, 200, 200) };

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

        void DrawGrid(BatchDraw2D& Draw2D)
        {
            const float VisualGridHeight = Leviathan::AppHeight;
            const float VisualGridWidth = VisualGridHeight / 2.0f;
            const v2f VisualGridPos{100.0f, 0.0f};
            const float VisualCellSize = VisualGridHeight / (float)GridHeight;

            // Draw grid background
            Draw2D.AddQuad(QuadF{VisualGridPos.X, VisualGridPos.Y, VisualGridWidth, VisualGridHeight}, BackgroundColor);

            // Draw cells
            for (int CellIdx = 0; CellIdx < GridSize; CellIdx++)
            {
                int CellCol = CellIdx % GridWidth;
                int CellRow = CellIdx / GridWidth;

                float CellX = VisualGridPos.X + CellCol * VisualCellSize;
                float CellY = VisualGridPos.Y + CellRow * VisualCellSize;
                int ColorIdx = CellIdx % BLOCKTYPE_COUNT;
                RGBA CellColor = CellColors[ColorIdx];
                //RGBA CellColor = CellColors[PlayField[CellIdx]];

                Draw2D.AddQuad(QuadF{ CellX, CellY, VisualCellSize, VisualCellSize }, CellColor);
            }

            // Draw grid lines
            constexpr float LineThickness = 2.0f;
            constexpr float HalfLineThickness = LineThickness / 2.0f;
            for (int RowIdx = 0; RowIdx < GridHeight + 1; RowIdx++)
            {
                float LineY = VisualGridPos.Y + RowIdx * VisualCellSize - HalfLineThickness;
                Draw2D.AddQuad(QuadF{VisualGridPos.X, LineY, VisualGridWidth, LineThickness}, LineColor);
            }
            for (int ColIdx = 0; ColIdx < GridWidth + 1; ColIdx++)
            {
                float LineX = VisualGridPos.X + ColIdx * VisualCellSize - HalfLineThickness;
                Draw2D.AddQuad(QuadF{LineX, VisualGridPos.Y, LineThickness, VisualGridHeight}, LineColor);
            }
        }

        void UpdateAndDraw(BatchDraw2D& OutDraw2D)
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
            
            DrawGrid(OutDraw2D);
        }

        void Init()
        {
            for (int CellIdx = 0; CellIdx < GridSize; CellIdx++) { PlayField[CellIdx] = BLOCK_NONE; }
        }

        void Term()
        {

        }
    }
}
