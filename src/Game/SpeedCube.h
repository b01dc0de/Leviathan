#ifndef GAME_SPEEDCUBE_H
#define GAME_SPEEDCUBE_H

#include "../Leviathan.h"

namespace Game
{
    namespace SpeedCube
    {
        void Update();
        void Draw(LvGFXContext& GFXContext);
        void Init();
        void Term();
    }
}

#endif // GAME_SPEEDCUBE_H
