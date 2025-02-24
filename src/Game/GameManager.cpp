#include "GameManager.h"
#include "BulletLimbo.h"
#include "Tetris.h"
#include "Snake.h"

using namespace Game;

enum struct GameType
{
    BulletLimbo,
    Tetris,
    GridSnake,
    Count
};

static GameType SelectedGame = GameType::BulletLimbo;

struct GameDefinition
{
    using ProcCallback = void (*)(void);
    using DrawCallback = void (*)(BatchDraw2D&);

    ProcCallback Update = nullptr;
    DrawCallback Draw = nullptr;
    ProcCallback Init = nullptr;
    ProcCallback Term = nullptr;
};

const GameDefinition GameDefines[(size_t)GameType::Count] =
{
    // BulletLimbo
    { BulletLimbo::Update, BulletLimbo::Draw, BulletLimbo::Init, BulletLimbo::Term },
    // Tetris
    { Tetris::Update, Tetris::Draw, Tetris::Init, Tetris::Term },
    // Snake
    { GridSnake::Update, GridSnake::Draw, GridSnake::Init, GridSnake::Term },
};

void GameManager::Update()
{
    const GameDefinition& CurrGame = GameDefines[(size_t)SelectedGame];
    ASSERT(CurrGame.Update);
    CurrGame.Update();
}

void GameManager::Draw(BatchDraw2D& Draw2D)
{
    const GameDefinition& CurrGame = GameDefines[(size_t)SelectedGame];
    ASSERT(CurrGame.Draw);
    CurrGame.Draw(Draw2D);
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

