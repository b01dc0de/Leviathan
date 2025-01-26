#ifndef LEVIATHAN_INPUT_H
#define LEVIATHAN_INPUT_H

namespace Leviathan
{
    enum LvKeyCode
    {
        LV_KEY_NONE,
        // Row 1
        LV_KEY_ESC,
        LV_KEY_F1,
        LV_KEY_F2,
        LV_KEY_F3,
        LV_KEY_F4,
        LV_KEY_F5,
        LV_KEY_F6,
        LV_KEY_F7,
        LV_KEY_F8,
        LV_KEY_F9,
        LV_KEY_F10,
        LV_KEY_F11,
        LV_KEY_F12,
        // Row 2
        LV_KEY_GRAVE,
        LV_KEY_1,
        LV_KEY_2,
        LV_KEY_3,
        LV_KEY_4,
        LV_KEY_5,
        LV_KEY_6,
        LV_KEY_7,
        LV_KEY_8,
        LV_KEY_9,
        LV_KEY_0,
        LV_KEY_MINUS,
        LV_KEY_EQUALS,
        LV_KEY_BACKSPACE,
        // Row 3
        LV_KEY_TAB,
        LV_KEY_Q,
        LV_KEY_W,
        LV_KEY_E,
        LV_KEY_R,
        LV_KEY_T,
        LV_KEY_Y,
        LV_KEY_U,
        LV_KEY_I,
        LV_KEY_O,
        LV_KEY_P,
        LV_KEY_LEFT_BRACKET,
        LV_KEY_RIGHT_BRACKET,
        LV_KEY_BACKSLASH,
        // Row 4
        LV_KEY_CAPSLOCK,
        LV_KEY_A,
        LV_KEY_S,
        LV_KEY_D,
        LV_KEY_F,
        LV_KEY_G,
        LV_KEY_H,
        LV_KEY_J,
        LV_KEY_K,
        LV_KEY_L,
        LV_KEY_SEMICOLON,
        LV_KEY_QUOTE,
        LV_KEY_ENTER,
        // Row 5
        LV_KEY_LEFT_SHIFT,
        LV_KEY_Z,
        LV_KEY_X,
        LV_KEY_C,
        LV_KEY_V,
        LV_KEY_B,
        LV_KEY_N,
        LV_KEY_M,
        LV_KEY_COMMA,
        LV_KEY_PERIOD,
        LV_KEY_SLASH,
        LV_KEY_RIGHT_SHIFT,
        // Row 6
        LV_KEY_LEFT_CTRL,
        LV_KEY_LEFT_SUPER,
        LV_KEY_LEFT_ALT,
        LV_KEY_SPACE,
        LV_KEY_RIGHT_ALT,
        LV_KEY_RIGHT_SUPER,
        LV_KEY_RIGHT_CTRL,
        // Arrow LV_KEYs
        LV_KEY_ARROW_UP,
        LV_KEY_ARROW_LEFT,
        LV_KEY_ARROW_DOWN,
        LV_KEY_ARROW_RIGHT
    };
    // TODO: Do we want to distinguish between left/right modifier keys?

    struct KeyboardState
    {
        static constexpr int KeyCount = 8;
        static LvKeyCode ActiveKeys[KeyCount];
        static int ActiveCount;

        static LvKeyCode VkToLv(int VkCode);
        static bool GetKeyState(LvKeyCode LvCode);
        static void SetKeyUp(int VkCode);
        static void SetKeyDown(int VkCode);
    };

    struct InputVisualizer
    {
        static void Draw(ID2D1RenderTarget* D2_RenderTarget, ID2D1Brush* InBrush);
    };
}

#endif // LEVIATHAN_INPUT_H