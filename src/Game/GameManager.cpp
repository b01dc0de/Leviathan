#include "GameManager.h"
#include "BulletLimbo.h"
#include "Tetris.h"
#include "Snake.h"
#include "../Input.h"

using namespace Game;
using namespace Leviathan;

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
    using DrawCallback = void (*)(GameGraphicsContext&);

    ProcCallback Update = nullptr;
    DrawCallback Draw = nullptr;
    ProcCallback Init = nullptr;
    ProcCallback Term = nullptr;
};

GameType GetNextGame(GameType CurrGame)
{
    GameType NextGame = (GameType)(((int)CurrGame + 1) % (int)GameType::Count);
    return NextGame;
}

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

    if (KeyboardState::GetKey(LV_KEY_1))
    {
        CurrGame.Term();

        SelectedGame = GetNextGame(SelectedGame);

        const GameDefinition& NextGame = GameDefines[(size_t)SelectedGame];
        NextGame.Init();
        NextGame.Update();
    }
}


void GameManager::Draw(GameGraphicsContext& GFXContext)
{
    const GameDefinition& CurrGame = GameDefines[(size_t)SelectedGame];
    ASSERT(CurrGame.Draw);
    CurrGame.Draw(GFXContext);
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

