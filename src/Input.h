#ifndef LEVIATHAN_INPUT_H
#define LEVIATHAN_INPUT_H

namespace Leviathan
{
    struct KeyboardState
    {
        static constexpr int KeyCount = 8;
        static int ActiveKeys[KeyCount];
        static int ActiveCount;

        static bool GetKeyState(int VKCode);
        static void SetKeyUp(int VKCode);
        static void SetKeyDown(int VKCode);
    };

    struct InputVisualizer
    {
        static void Draw(ID2D1RenderTarget* D2_RenderTarget);
    };
}

#endif // LEVIATHAN_INPUT_H