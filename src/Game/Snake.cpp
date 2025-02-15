#include "Snake.h"

namespace Game
{
    using namespace Leviathan;

    inline namespace GridSnakeState
    {
        enum MoveDirection
        {
            DIR_NORTH,
            DIR_EAST,
            DIR_SOUTH,
            DIR_WEST,
            DIR_COUNT
        };

        enum struct PlayState
        {
            Attract,
            Play,
            Count
        };

        enum GridCellState
        {
            CELL_EMPTY,
            CELL_SNAKE,
            CELL_FRUIT,
            CELLSTATE_COUNT
        };

        constexpr int GridLength = 32;
        constexpr int GridSize = GridLength * GridLength;

        float VisualGridSize = AppWidth / 3.0f;
        float VisualGridOffset = VisualGridSize / 3.0f;
        v2f VisualGridPos{ VisualGridOffset / 3.0f, VisualGridOffset };
        float VisualCellSize = VisualGridSize / GridLength;

        GridCellState PlayField[GridSize];
        PlayState GameState;

        int HeadX = 0;
        int HeadY = 0;
        MoveDirection HeadDir = DIR_NORTH;
        double LastMoveTime = 0.0f;
        double SecondsPerMove = 0.25f;
        Array<v2i> Tail;

        v2i FruitPos;

        RGBA GridColor{ Norm8Bit(127u, 186u, 0u) };
        RGBA HeadColor{ GridColor };
        RGBA FruitColor{ Norm8Bit(255u, 136u, 142u) };

        bool CheckIdx(int Row, int Col);
        bool CheckCollision(int Row, int Col);
        bool CheckCollision(const v2i& Pos);
        GridCellState& GetCell(int Row, int Col);
        GridCellState& GetCell(const v2i& Pos);
        v2i GetNextPos(MoveDirection Dir);
        bool CanMove(MoveDirection Dir);
        v2i GetNewFruitPos();
        void EatFruit();
        void ClearGrid();
        void Update();
    }

    bool GridSnakeState::CheckIdx(int Row, int Col)
    {
        return (0 <= Row && Row < GridLength) && (0 <= Col && Col < GridLength);
    }

    bool GridSnakeState::CheckCollision(int Row, int Col)
    {
        return !CheckIdx(Row, Col) || GetCell(Row, Col) == CELL_SNAKE;
    }

    bool GridSnakeState::CheckCollision(const v2i& Pos)
    {
        return CheckCollision(Pos.Y, Pos.X);
    }

    GridCellState& GridSnakeState::GetCell(int Row, int Col)
    {
        ASSERT(CheckIdx(Row, Col));
        return PlayField[Row * GridLength + Col];
    }

    GridCellState& GridSnakeState::GetCell(const v2i& Pos)
    {
        return GetCell(Pos.Y, Pos.X);
    }

    v2i GridSnakeState::GetNextPos(MoveDirection Dir)
    {
        v2i NextPos{ HeadX, HeadY };
        switch (Dir)
        {
            case DIR_NORTH: { NextPos.Y--; } break;
            case DIR_EAST: { NextPos.X++; } break;
            case DIR_SOUTH: { NextPos.Y++; } break;
            case DIR_WEST: { NextPos.X--; } break;
        }
        return NextPos;
    }

    bool GridSnakeState::CanMove(MoveDirection Dir)
    {
        v2i NextPos = GetNextPos(Dir);
        return !CheckCollision(NextPos.Y, NextPos.X);
    }

    v2i GridSnakeState::GetNewFruitPos()
    {
        int Min = 1;
        int Max = GridLength - 1;
        v2i NewFruitPos;
        do
        {
            NewFruitPos = { GetRandomInRange(Min, Max), GetRandomInRange(Min, Max) };
        } while (CheckCollision(NewFruitPos));
        return NewFruitPos;
    }

    void GridSnakeState::EatFruit()
    {
        Tail.Add(FruitPos);
        FruitPos = GetNewFruitPos();
    }

    void GridSnakeState::ClearGrid()
    {
        for (int CellIdx = 0; CellIdx < GridSize; CellIdx++) { PlayField[CellIdx] = CELL_EMPTY; }
    }

    void GridSnakeState::Update()
    {
        bool bInputNorth = KeyboardState::GetKeyState(LV_KEY_W);
        bool bInputSouth = KeyboardState::GetKeyState(LV_KEY_S);
        bool bInputEast = KeyboardState::GetKeyState(LV_KEY_D);
        bool bInputWest = KeyboardState::GetKeyState(LV_KEY_A);

        switch (GameState)
        {
            case PlayState::Attract:
            {
                if (bInputNorth || bInputEast || bInputSouth || bInputWest) {
                    if (bInputNorth) { HeadDir = DIR_NORTH; }
                    if (bInputSouth) { HeadDir = DIR_SOUTH; }
                    if (bInputEast) { HeadDir = DIR_EAST; }
                    if (bInputWest) { HeadDir = DIR_WEST; }
                    GameState = PlayState::Play;
                }
                return;
            } break;
            case PlayState::Play:
            {
            } break;
        }

        switch (HeadDir)
        {
            case DIR_NORTH:
            case DIR_SOUTH:
            {
                if (bInputEast != bInputWest)
                {
                    HeadDir = bInputEast ?  DIR_EAST : DIR_WEST;
                }
            } break;
            case DIR_EAST:
            case DIR_WEST:
            {
                if (bInputNorth != bInputSouth)
                {
                    HeadDir = bInputNorth ? DIR_NORTH : DIR_SOUTH;
                }
            } break;
            default: { ASSERT(false); }
        }

        double CurrTime = Clock::Time();
        if (CurrTime - LastMoveTime > SecondsPerMove)
        {
            v2i NextPos = GetNextPos(HeadDir);
            if (!CheckCollision(NextPos))
            {
                HeadX = NextPos.X;
                HeadY = NextPos.Y;
                if (GetCell(NextPos) == CELL_FRUIT) { EatFruit(); }
            }
            else
            {
                GameState = PlayState::Attract;
                GridSnake::Init();
            }
            LastMoveTime = CurrTime;
        }
    }

    void GridSnake::UpdateAndDraw(BatchDraw2D& Draw2D)
    {
        GridSnakeState::Update();

        QuadF GridQuad{ VisualGridPos.X, VisualGridPos.Y, VisualGridSize, VisualGridSize };
        Draw2D.AddBox(GridQuad, GridColor);

        QuadF HeadQuad
        {
            VisualGridPos.X + HeadX * VisualCellSize,
            VisualGridPos.Y + VisualGridSize - ((HeadY+1) * VisualCellSize),
            VisualCellSize, VisualCellSize
        };
        Draw2D.AddQuad(HeadQuad, HeadColor);

        QuadF FruitQuad
        {
            VisualGridPos.X + FruitPos.X * VisualCellSize,
            VisualGridPos.Y + VisualGridSize - ((FruitPos.Y+1) * VisualCellSize),
            VisualCellSize, VisualCellSize
        };
        Draw2D.AddQuad(FruitQuad, FruitColor);
    }

    void GridSnake::Init()
    {
        GridSnakeState::ClearGrid();
        GameState = PlayState::Attract;
        HeadX = GridLength / 2;
        HeadY = GridLength / 2;
        FruitPos = { 0, 0 };
        LastMoveTime = Clock::Time();
    }

    void GridSnake::Term()
    {
    }
}