#ifndef LEVIATHAN_INPUTVISUALIZER_H
#define LEVIATHAN_INPUTVISUALIZER_H

#include "Leviathan.h"

namespace Leviathan
{
    struct InputVisualizer
    {
        static constexpr float LineWidth = 2.0f;
        static void DrawKeyboard(BatchDrawCmds& Draw2D);
        static void DrawMouse(BatchDrawCmds& Draw2D);
        static void DrawGamepads(BatchDrawCmds& Draw2D);
    };
}

#endif // LEVIATHAN_INPUTVISUALIZER_H

