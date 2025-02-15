#ifndef GAME_SNAKE_H
#define GAME_SNAKE_H

#include "../Leviathan.h"

namespace Game
{

    using Leviathan::BatchDraw2D;

    namespace GridSnake
    {
        void Update();
        void Draw(BatchDraw2D& Draw2D);
        void Init();
        void Term();
    }

}

#endif // GAME_TETRIS_H
