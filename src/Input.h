#ifndef LEVIATHAN_INPUT_H
#define LEVIATHAN_INPUT_H

#include "Math.h"

namespace Leviathan
{
    enum LvKeyCode
    {
        LV_KEY_NONE,
        // Function row
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
        // Number row
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
        // QWERTY row
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
        // ASDF row
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
        // ZXCV row
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
        // Modifiers
        LV_KEY_SHIFT,
        LV_KEY_CTRL,
        LV_KEY_SUPER,
        LV_KEY_ALT,
        LV_KEY_SPACE,
        // Arrow keys
        LV_KEY_ARROW_UP,
        LV_KEY_ARROW_LEFT,
        LV_KEY_ARROW_DOWN,
        LV_KEY_ARROW_RIGHT,
        LV_KEY_COUNT,
    };

    struct KeyboardState
    {
        static constexpr int KeyCount = 16;
        static LvKeyCode ActiveKeys[KeyCount];
        static int ActiveCount;
        static bool bRawInput;

        static LvKeyCode VkToLv(int VkCode);
        static bool GetKeyState(LvKeyCode LvCode);
        static void SetKeyUp(int VkCode);
        static void SetKeyDown(int VkCode);
        static void Win32_KeyMsg(UINT Msg, WPARAM wParam, LPARAM lParam);
        static void Win32_RawInput(RAWINPUT* pRawInput);
    };

    struct MouseState
    {
        static bool bInit;
        static bool bRawInput;

        static int MouseX;
        static int MouseY;
        static bool bLeftKey;
        static bool bRightKey;
        static bool bMiddleKey;

        static void Init();
        static void SetMousePosRel(int dX, int dY);
        static void SetMousePosAbs(int X, int Y);
        static void Win32_MouseMsg(UINT Msg, WPARAM wParam, LPARAM lParam);
        static void Win32_RawInput(RAWINPUT* pRawInput);
    };

    enum LvGamepadButton
    {
        LV_GAMEPAD_DPAD_UP,
        LV_GAMEPAD_DPAD_DOWN,
        LV_GAMEPAD_DPAD_LEFT,
        LV_GAMEPAD_DPAD_RIGHT,
        LV_GAMEPAD_START,
        LV_GAMEPAD_SELECT,
        LV_GAMEPAD_LEFT_THUMB,
        LV_GAMEPAD_RIGHT_THUMB,
        LV_GAMEPAD_LEFT_SHOULDER,
        LV_GAMEPAD_RIGHT_SHOULDER,
        LV_GAMEPAD_FACE_UP,
        LV_GAMEPAD_FACE_DOWN,
        LV_GAMEPAD_FACE_LEFT,
        LV_GAMEPAD_FACE_RIGHT,
        LV_GAMEPAD_BUTTON_COUNT,
    };

    struct GamepadState
    {
        static bool Buttons[LV_GAMEPAD_BUTTON_COUNT];
        static float LeftTrigger;
        static float RightTrigger;
        static float LeftStickX;
        static float LeftStickY;
        static float RightStickX;
        static float RightStickY;
        static unsigned int LastReading;

        static float GetLeftTrigger();
        static float GetRightTrigger();
        static v2f GetLeftStick();
        static v2f GetRightStick();
        static bool GetButton(LvGamepadButton LvButton);
        static void Win32_UpdateXInput();
    };

    struct RawInputHandler
    {
        static void Init();
        static void Win32_ProcessInput(WPARAM wParam, LPARAM lParam);
        static void Win32_DeviceChange(WPARAM wParam, LPARAM lParam);
    };

    struct InputVisualizer
    {
        static void DrawKeyboard(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush);
        static void DrawMouse(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2);
        static void DrawGamepad(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush);
    };
}

#endif // LEVIATHAN_INPUT_H