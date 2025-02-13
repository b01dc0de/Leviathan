#ifndef GAME_TETRIS_H
#define GAME_TETRIS_H

#include "../Leviathan.h"
#include "../Graphics.h"

namespace Game
{
    using Leviathan::BatchDraw2D;

    namespace Tetris
    {
        void UpdateAndDraw(BatchDraw2D& Draw2D);
        void Init();
        void Term();
    }
}

#endif // GAME_TETRIS_H
