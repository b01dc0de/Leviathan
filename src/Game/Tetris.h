#ifndef GAME_TETRIS_H
#define GAME_TETRIS_H

#include "../Leviathan.h"
#include "../Graphics.h"

namespace Game
{
    using Leviathan::InstQuadColorData;

    namespace Tetris
    {
        void UpdateAndDraw(Array<InstQuadColorData>& OutDrawCmds);
        void Init();
        void Term();
    }
}

#endif // GAME_TETRIS_H
