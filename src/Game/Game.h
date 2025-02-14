#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "../Leviathan.h"

namespace Game
{
    struct Manager
    {
        static void UpdateAndDraw(BatchDraw2D& Draw2D);
        static void Init();
        static void Term();
    };
}

#endif // GAME_GAME_H