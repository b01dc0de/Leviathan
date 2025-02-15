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

        constexpr int GridLength = 32;
        constexpr int GridSize = GridLength * GridLength;

        float VisualGridSize = AppWidth / 3.0f;
        float VisualGridOffset = VisualGridSize / 3.0f;
        v2f VisualGridPos{ VisualGridOffset / 3.0f, VisualGridOffset };
        float VisualCellSize = VisualGridSize / GridLength;

        RGBA GridColor{ Norm8Bit(127u, 186u, 0u) };
        RGBA HeadColor{ GridColor };
        RGBA TailColor{ GridColor };
        RGBA FruitColor{ Norm8Bit(255u, 136u, 142u) };

        PlayState GameState;

        MoveDirection HeadDir = DIR_NORTH;
        v2i HeadPos;
        Array<v2i> Tail;
        int LastTailIdx = 0;
        v2i FruitPos;
        double LastMoveTime = 0.0f;
        double SecondsPerMove = 0.25f;

        bool CheckIdx(int Row, int Col);
        bool CheckTailCollision(const v2i& Pos);
        bool CanMove(MoveDirection Dir);
        v2i GetNextPos(MoveDirection Dir);
        v2i GetNewFruitPos();
        void GrowTail(v2i FruitPos);
        void MoveTail(v2i NewTail);
        void EatFruit();
        void Update();
        void DrawCell(BatchDraw2D& Draw2D, v2i Cell, RGBA Color);
    }

    void GridSnakeState::DrawCell(BatchDraw2D& Draw2D, v2i Cell, RGBA Color)
    {
        QuadF CellQuad
        {
            VisualGridPos.X + Cell.X * VisualCellSize,
            VisualGridPos.Y + VisualGridSize - ((Cell.Y+1) * VisualCellSize),
            VisualCellSize, VisualCellSize
        };
        Draw2D.AddQuad(CellQuad, Color);
    }

    bool GridSnakeState::CheckIdx(int Row, int Col)
    {
        return (0 <= Row && Row < GridLength) && (0 <= Col && Col < GridLength);
    }

    bool GridSnakeState::CheckTailCollision(const v2i& Pos)
    {
        for (int TailIdx = 0; TailIdx < Tail.Num; TailIdx++)
        {
            if (Pos == Tail[TailIdx]) { return true; }
        }
        return false;
    }

    bool GridSnakeState::CanMove(MoveDirection Dir)
    {
        v2i NextPos = GetNextPos(Dir);
        return CheckIdx(NextPos.Y, NextPos.X) && !CheckTailCollision(NextPos);
    }

    v2i GridSnakeState::GetNextPos(MoveDirection Dir)
    {
        v2i NextPos{ HeadPos.X, HeadPos.Y };
        switch (Dir)
        {
            case DIR_NORTH: { NextPos.Y--; } break;
            case DIR_EAST: { NextPos.X++; } break;
            case DIR_SOUTH: { NextPos.Y++; } break;
            case DIR_WEST: { NextPos.X--; } break;
        }
        return NextPos;
    }

    v2i GridSnakeState::GetNewFruitPos()
    {
        int Min = 1;
        int Max = GridLength - 1;
        v2i NewFruitPos{ -1, -1 };
        do { NewFruitPos = { GetRandomInRange(Min, Max), GetRandomInRange(Min, Max) }; }
        while (NewFruitPos == HeadPos || CheckTailCollision(NewFruitPos));
        return NewFruitPos;
    }

    void GridSnakeState::GrowTail(v2i FruitPos)
    {
        Tail.Add(FruitPos);
    }

    void GridSnakeState::MoveTail(v2i NewTail)
    {
        if (Tail.Num == 0)
        {
            return;
        }
        else
        {
            Tail[LastTailIdx] = NewTail;
            LastTailIdx = (LastTailIdx + 1) % Tail.Num;
        }
    }

    void GridSnakeState::EatFruit()
    {
        GrowTail(FruitPos);
        FruitPos = GetNewFruitPos();
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
            if (CanMove(HeadDir))
            {
                v2i NewTailPos = HeadPos;
                HeadPos = GetNextPos(HeadDir);
                if (HeadPos == FruitPos) { EatFruit(); }
                MoveTail(NewTailPos);
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

        // Draw Grid
        Draw2D.AddBox(QuadF{ VisualGridPos.X, VisualGridPos.Y, VisualGridSize, VisualGridSize }, GridColor);

        // Draw Head
        DrawCell(Draw2D, HeadPos, HeadColor);
        // Draw Tail
        for (int TailIdx = 0; TailIdx < Tail.Num; TailIdx++)
        {
            DrawCell(Draw2D, Tail[TailIdx], TailColor);
        }

        DrawCell(Draw2D, FruitPos, FruitColor);
    }

    void GridSnake::Init()
    {
        Tail.Empty();
        GameState = PlayState::Attract;
        HeadPos = { GridLength / 2, GridLength / 2 };
        FruitPos = GetNewFruitPos();
        LastMoveTime = Clock::Time();
    }

    void GridSnake::Term()
    {
    }
}