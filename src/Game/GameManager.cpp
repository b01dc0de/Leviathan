#include "GameManager.h"
#include "Tetris.h"
#include "Snake.h"

using namespace Game;

enum struct GameType
{
    Tetris,
    GridSnake,
    Count
};

static GameType SelectedGame = GameType::GridSnake;

struct GameDefinition
{
    using ProcCallback = void (*)(void);
    using UpdateDrawCallback = void (*)(BatchDraw2D&);

    UpdateDrawCallback UpdateDraw = nullptr;
    ProcCallback Init = nullptr;
    ProcCallback Term = nullptr;
};

const GameDefinition GameDefines[(size_t)GameType::Count] =
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

void GameManager::UpdateAndDraw(BatchDraw2D& Draw2D)
{
    const GameDefinition& CurrGame = GameDefines[(size_t)SelectedGame];
    ASSERT(CurrGame.UpdateDraw);
    CurrGame.UpdateDraw(Draw2D);
}
void GameManager::Init()
{
    const GameDefinition& CurrGame = GameDefines[(size_t)SelectedGame];
    ASSERT(CurrGame.Init);
    CurrGame.Init();
}
void GameManager::Term()
{
    const GameDefinition& CurrGame = GameDefines[(size_t)SelectedGame];
    ASSERT(CurrGame.Term);
    CurrGame.Term();
}

