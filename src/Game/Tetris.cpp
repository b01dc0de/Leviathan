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

        RGBA BackgroundColor{1.0f, 1.0f, 1.0f, 1.0f};

        void DrawGrid(Array<InstQuadColorData>& OutDrawCmds)
        {
            /*
                - Draw grid BG
                - Draw grid cells
                - Draw grid lines
            */
            const float VisualGridHeight = Leviathan::AppHeight;
            const float VisualGridWidth = VisualGridHeight / 2.0f;
            const v2f VisualGridPos{100.0f, 0.0f};

            OutDrawCmds.Add(InstQuadColorData{ QuadF{VisualGridPos.X, VisualGridPos.Y, VisualGridWidth, VisualGridHeight}, BackgroundColor });
        }

        void UpdateAndDraw(Array<InstQuadColorData>& OutDrawCmds)
        {
            DrawGrid(OutDrawCmds);
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
