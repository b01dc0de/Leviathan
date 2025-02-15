#ifndef GAME_GAMEMANAGER_H
#define GAME_GAMEMANAGER_H

#include "../Leviathan.h"

struct GameManager
{
    static void Update();
    static void Draw(BatchDraw2D& Draw2D);
    static void Init();
    static void Term();
};

#endif // GAME_GAMEMANAGER_H
