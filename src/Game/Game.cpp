#include "Game.h"
#include "Tetris.h"
#include "Snake.h"

namespace Game
{
    enum struct GameType
    {
        Tetris,
        GridSnake,
        Count
    };

    constexpr GameType SelectedGame = GameType::GridSnake;

    using ProcCallback = void (*)(void);
    using UpdateDrawCallback = void (*)(BatchDraw2D&);
    struct GameDefinition
    {
        UpdateDrawCallback UpdateDraw = nullptr;
        ProcCallback Init = nullptr;
        ProcCallback Term = nullptr;
    };
    GameDefinition GameDefines[(size_t)GameType::Count] =
    {
        { // Tetris
            Tetris::UpdateAndDraw,
            Tetris::Init,
            Tetris::Term
        },
        { // Snake
            GridSnake::UpdateAndDraw,
            GridSnake::Init,
            GridSnake::Term
        }
    };

    void Manager::UpdateAndDraw(BatchDraw2D& Draw2D)
    {
        GameDefinition& CurrGame = GameDefines[(size_t)SelectedGame];
        ASSERT(CurrGame.UpdateDraw);
        CurrGame.UpdateDraw(Draw2D);
    }
    void Manager::Init()
    {
        GameDefinition& CurrGame = GameDefines[(size_t)SelectedGame];
        ASSERT(CurrGame.Init);
        CurrGame.Init();
    }
    void Manager::Term()
    {
        GameDefinition& CurrGame = GameDefines[(size_t)SelectedGame];
        ASSERT(CurrGame.Term);
        CurrGame.Term();
    }
}
