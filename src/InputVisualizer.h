#ifndef LEVIATHAN_INPUTVISUALIZER_H
#define LEVIATHAN_INPUTVISUALIZER_H

#include "Leviathan.h"

namespace Leviathan
{
    struct InputVisualizer
    {
        static constexpr float LineWidth = 2.0f;
        static void DrawKeyboard(BatchDraw2D& Draw2D);
        static void DrawMouse(BatchDraw2D& Draw2D);
        static void DrawGamepads(BatchDraw2D& Draw2D);
    };
}

#endif // LEVIATHAN_INPUTVISUALIZER_H