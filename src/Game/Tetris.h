#ifndef GAME_TETRIS_H
#define GAME_TETRIS_H

#include "../Leviathan.h"

namespace Game
{
    namespace Tetris
    {
        void Update();
        void Draw(LvGFXContext& GFXContext);
        void Init();
        void Term();
    }
}

#endif // GAME_TETRIS_H

