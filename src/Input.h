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

struct KeyState
{
    LvKeyCode Code = LV_KEY_NONE;
    int FramesHeld = 0;
};

struct KeyboardState
{
    static constexpr int KeyCount = 16;
    static KeyState ActiveKeys[KeyCount];
    static int ActiveCount;
    static bool bRawInput;

    static LvKeyCode VkToLv(int VkCode);
    static bool GetKey(LvKeyCode LvCode, bool bHeld = false);
    static void SetKeyUp(int VkCode);
    static void SetKeyDown(int VkCode);
    static void Win32_KeyMsg(UINT Msg, WPARAM wParam, LPARAM lParam);
    static void Win32_RawInput(RAWINPUT* pRawInput);
    static void Tick();
};

struct MouseState
{
    static bool bInit;
    static bool bRawInput;

    static int MouseX;
    static int MouseY;
    static float MouseWheel;
    static bool bLeftKey;
    static bool bRightKey;
    static bool bMiddleKey;

    static void Init();
    static void SetMousePosRel(int dX, int dY);
    static void SetMousePosAbs(int X, int Y);
    static void Win32_MouseMsg(UINT Msg, WPARAM wParam, LPARAM lParam);
    static void Win32_RawInput(RAWINPUT* pRawInput);
    static void Tick();
};

enum LvGamepadButton
{
    LV_GAMEPAD_DPAD_UP,
    LV_GAMEPAD_DPAD_DOWN,
    LV_GAMEPAD_DPAD_LEFT,
    LV_GAMEPAD_DPAD_RIGHT,
    LV_GAMEPAD_FACE_UP,
    LV_GAMEPAD_FACE_DOWN,
    LV_GAMEPAD_FACE_LEFT,
    LV_GAMEPAD_FACE_RIGHT,
    LV_GAMEPAD_START,
    LV_GAMEPAD_SELECT,
    LV_GAMEPAD_LEFT_SHOULDER,
    LV_GAMEPAD_RIGHT_SHOULDER,
    LV_GAMEPAD_LEFT_THUMB,
    LV_GAMEPAD_RIGHT_THUMB,
    LV_GAMEPAD_BUTTON_COUNT,
};

struct GamepadState
{
    static constexpr int NumGamepads = 4;
    static constexpr float StickDeadzone = 0.025f;
    static constexpr double SecondsPerConnectCheck = 5.0;
    static bool bActive[NumGamepads];
    static double LastConnectCheck[NumGamepads];
    static bool Buttons[NumGamepads][LV_GAMEPAD_BUTTON_COUNT];
    static float LeftTrigger[NumGamepads];
    static float RightTrigger[NumGamepads];
    static float LeftStickX[NumGamepads];
    static float LeftStickY[NumGamepads];
    static float RightStickX[NumGamepads];
    static float RightStickY[NumGamepads];
    static unsigned int LastReading;

    static bool IsValidIdx(int GamepadIdx);
    static bool IsConnected(int GamepadIdx = 0);
    static float GetLeftTrigger(int GamepadIdx = 0);
    static float GetRightTrigger(int GamepadIdx = 0);
    static v2f GetLeftStick(int GamepadIdx = 0);
    static v2f GetRightStick(int GamepadIdx = 0);
    static bool GetButton(LvGamepadButton LvButton, int GamepadIdx = 0);
    static void Win32_UpdateXInput();
    static void Tick();
};

struct RawInputHandler
{
    static void Init();
    static void Win32_ProcessInput(WPARAM wParam, LPARAM lParam);
    static void Win32_DeviceChange(WPARAM wParam, LPARAM lParam);
};

}

#endif // LEVIATHAN_INPUT_H

