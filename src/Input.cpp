#include "Leviathan.h" // TODO: Input.h included in Leviathan.h

namespace Leviathan
{
    enum LvKeyCode
    {
        KEY_NONE,
        // Row 1
        KEY_ESC,
        KEY_F1,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,
        // Row 2
        KEY_GRAVE,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_0,
        KEY_MINUS,
        KEY_EQUALS,
        KEY_BACKSPACE,
        // Row 3
        KEY_TAB,
        KEY_Q,
        KEY_W,
        KEY_E,
        KEY_R,
        KEY_T,
        KEY_Y,
        KEY_U,
        KEY_I,
        KEY_O,
        KEY_P,
        KEY_LEFT_BRACKET,
        KEY_RIGHT_BACKET,
        KEY_BACKSLASH,
        // Row 4
        KEY_CAPSLOCK,
        KEY_A,
        KEY_S,
        KEY_D,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_SEMICOLON,
        KEY_QUOTE,
        KEY_ENTER,
        // Row 5
        KEY_LEFT_SHIFT,
        KEY_Z,
        KEY_X,
        KEY_C,
        KEY_V,
        KEY_B,
        KEY_N,
        KEY_M,
        KEY_COMMA,
        KEY_PERIOD,
        KEY_SLASH,
        KEY_RIGHT_SHIFT,
        // Row 6
        KEY_LEFT_CTRL,
        KEY_LEFT_SUPER,
        KEY_LEFT_ALT,
        KEY_SPACE,
        KEY_RIGHT_ALT,
        KEY_RIGHT_SUPER,
        KEY_RIGHT_CTRL,
        // Arrow keys
        KEY_ARROW_UP,
        KEY_ARROW_LEFT,
        KEY_ARROW_DOWN,
        KEY_ARROW_RIGHT
    };
    // TODO: Do we want to distinguish between left/right modifier keys?

    /*
    LvKeyCode VKCodeMap[] =
    {
    };
    int VKCodeMapSize = ARRAY_SIZE(VKCodeMap);
    */

    int KeyboardState::ActiveKeys[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int KeyboardState::ActiveCount = 0;

    bool KeyboardState::GetKeyState(int VKCode)
    {
        for (int KeyIdx = 0; KeyIdx < ActiveCount; KeyIdx++)
        {
            if (ActiveKeys[KeyIdx] == VKCode)
            {
                return true;
            }
        }
        return false;
    }

    void KeyboardState::SetKeyUp(int VKCode)
    {
        if (ActiveCount == 0) { return; }

        bool bKeyFound = false;
        for (int KeyIdx = 0; KeyIdx < ActiveCount; KeyIdx++)
        {
            if (ActiveKeys[KeyIdx] == VKCode)
            {
                bKeyFound = true;
            }
            if (bKeyFound && KeyIdx < (ActiveCount - 1))
            {
                ActiveKeys[KeyIdx] = ActiveKeys[KeyIdx + 1];
            }
        }
        if (bKeyFound) { ActiveCount--; }
    }

    void KeyboardState::SetKeyDown(int VKCode)
    {
        if (ActiveCount == KeyCount) { return; }

        for (int KeyIdx = 0; KeyIdx < KeyCount; KeyIdx++)
        {
            if (ActiveKeys[KeyIdx] == VKCode)
            {
                return;
            }
            if (ActiveKeys[KeyIdx] == 0)
            {
                ActiveKeys[KeyIdx] = VKCode;
                ActiveCount++;
                return;
            }
        }
    }

    void InputVisualizer::Draw(ID2D1RenderTarget* D2_RenderTarget)
    {
        // TODO: Implement (draw the owl)
    }
}