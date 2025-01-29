#ifndef LEVIATHAN_INPUTVISUALIZER_H
#define LEVIATHAN_INPUTVISUALIZER_H

#include "Leviathan.h"

namespace Leviathan
{
    struct InputVisualizer
    {
        static constexpr float LineWidth = 2.0f;
        static void DrawKeyboard(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2);
        static void DrawMouse(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2);
        static void DrawGamepad(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2);
    };
}

#endif // LEVIATHAN_INPUTVISUALIZER_H