#ifndef GAME_LABYRINTH_H
#define GAME_LABYRINTH_H

#include "../Leviathan.h"

namespace Game
{
    namespace Labyrinth
    {
        void Update();
        void Draw(LvGFXContext& GFXContext);
        void Init();
        void Term();
    }
}

#endif // GAME_LABYRINTH_H