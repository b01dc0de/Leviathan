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



    // NOTE: Added some blank space here to align the VKCodeMap so that [0] == 0x00 == Line #100
    LvKeyCode VKCodeMap[] =
    {
        KEY_NONE, //                            0x00    Unused
        KEY_NONE, // VK_LBUTTON                 0x01    Left mouse button
        KEY_NONE, // VK_RBUTTON                 0x02    Right mouse button
        KEY_NONE, // VK_CANCEL                  0x03    Control-break processing
        KEY_NONE, // VK_MBUTTON                 0x04    Middle mouse button
        KEY_NONE, // VK_XBUTTON1                0x05    X1 mouse button
        KEY_NONE, // VK_XBUTTON2                0x06    X2 mouse button
        KEY_NONE, // ----                       0x07    Reserved
        KEY_NONE, // VK_BACK                    0x08    BACKSPACE key
        KEY_NONE, // VK_TAB                     0x09    TAB key
        KEY_NONE, // ----                       0x0A    Reserved
        KEY_NONE, // ----                       0x0B    Reserved
        KEY_NONE, // VK_CLEAR                   0x0C    CLEAR key
        KEY_NONE, // VK_RETURN                  0x0D    ENTER key
        KEY_NONE, // ----                       0x0E    Unassigned
        KEY_NONE, // ----                       0x0F    Unassigned
        KEY_NONE, // VK_SHIFT                   0x10    SHIFT key
        KEY_NONE, // VK_CONTROL                 0x11    CTRL key
        KEY_NONE, // VK_MENU                    0x12    ALT key
        KEY_NONE, // VK_PAUSE                   0x13    PAUSE key
        KEY_NONE, // VK_CAPITAL                 0x14    CAPS LOCK key
        KEY_NONE, // VK_KANA | VK_HANGUL        0x15    IME Kana mode | IME Hangul mode
        KEY_NONE, // VK_IME_ON                  0x16    IME On
        KEY_NONE, // VK_JUNJA                   0x17    IME Junja mode
        KEY_NONE, // VK_FINAL                   0x18    IME final mode
        KEY_NONE, // VK_HANJA | VK_KANJI        0x19    IME Hanja mode | IME Kanji mode
        KEY_NONE, // VK_IME_OFF                 0x1A    IME Off
        KEY_NONE, // VK_ESCAPE                  0x1B    ESC key
        KEY_NONE, // VK_CONVERT                 0x1C    IME convert
        KEY_NONE, // VK_NONCONVERT              0x1D    IME nonconvert
        KEY_NONE, // VK_ACCEPT                  0x1E    IME accept
        KEY_NONE, // VK_MODECHANGE              0x1F    IME mode change request
        KEY_NONE, // VK_SPACE                   0x20    SPACEBAR
        KEY_NONE, // VK_PRIOR                   0x21    PAGE UP key
        KEY_NONE, // VK_NEXT                    0x22    PAGE DOWN key
        KEY_NONE, // VK_END                     0x23    END key
        KEY_NONE, // VK_HOME                    0x24    HOME key
        KEY_NONE, // VK_LEFT                    0x25    LEFT ARROW key
        KEY_NONE, // VK_UP                      0x26    UP ARROW key
        KEY_NONE, // VK_RIGHT                   0x27    RIGHT ARROW key
        KEY_NONE, // VK_DOWN                    0x28    DOWN ARROW key
        KEY_NONE, // VK_SELECT                  0x29    SELECT key
        KEY_NONE, // VK_PRINT                   0x2A    PRINT key
        KEY_NONE, // VK_EXECUTE                 0x2B    EXECUTE key
        KEY_NONE, // VK_SNAPSHOT                0x2C    PRINT SCREEN key
        KEY_NONE, // VK_INSERT                  0x2D    INS key
        KEY_NONE, // VK_DELETE                  0x2E    DEL key
        KEY_NONE, // VK_HELP                    0x2F    HELP key
        KEY_NONE, //                            0x30    0 key
        KEY_NONE, //                            0x31    1 key
        KEY_NONE, //                            0x32    2 key
        KEY_NONE, //                            0x33    3 key
        KEY_NONE, //                            0x34    4 key
        KEY_NONE, //                            0x35    5 key
        KEY_NONE, //                            0x36    6 key
        KEY_NONE, //                            0x37    7 key
        KEY_NONE, //                            0x38    8 key
        KEY_NONE, //                            0x39    9 key
        KEY_NONE, // ----                       0x3A    Undefined
        KEY_NONE, // ----                       0x3B    Undefined
        KEY_NONE, // ----                       0x3C    Undefined
        KEY_NONE, // ----                       0x3D    Undefined
        KEY_NONE, // ----                       0x3E    Undefined
        KEY_NONE, // ----                       0x3F    Undefined
        KEY_NONE, // ----                       0x40    Undefined
        KEY_NONE, //                            0x41    A key
        KEY_NONE, //                            0x42    B key
        KEY_NONE, //                            0x43    C key
        KEY_NONE, //                            0x44    D key
        KEY_NONE, //                            0x45    E key
        KEY_NONE, //                            0x46    F key
        KEY_NONE, //                            0x47    G key
        KEY_NONE, //                            0x48    H key
        KEY_NONE, //                            0x49    I key
        KEY_NONE, //                            0x4A    J key
        KEY_NONE, //                            0x4B    K key
        KEY_NONE, //                            0x4C    L key
        KEY_NONE, //                            0x4D    M key
        KEY_NONE, //                            0x4E    N key
        KEY_NONE, //                            0x4F    O key
        KEY_NONE, //                            0x50    P key
        KEY_NONE, //                            0x51    Q key
        KEY_NONE, //                            0x52    R key
        KEY_NONE, //                            0x53    S key
        KEY_NONE, //                            0x54    T key
        KEY_NONE, //                            0x55    U key
        KEY_NONE, //                            0x56    V key
        KEY_NONE, //                            0x57    W key
        KEY_NONE, //                            0x58    X key
        KEY_NONE, //                            0x59    Y key
        KEY_NONE, //                            0x5A    Z key
        KEY_NONE, // VK_LWIN                    0x5B    Left Windows key
        KEY_NONE, // VK_RWIN                    0x5C    Right Windows key
        KEY_NONE, // VK_APPS                    0x5D    Applications key
        KEY_NONE, // ----                       0x5E    Reserved
        KEY_NONE, // VK_SLEEP                   0x5F    Computer Sleep key
        KEY_NONE, // VK_NUMPAD0                 0x60    Numeric keypad 0 key
        KEY_NONE, // VK_NUMPAD1                 0x61    Numeric keypad 1 key
        KEY_NONE, // VK_NUMPAD2                 0x62    Numeric keypad 2 key
        KEY_NONE, // VK_NUMPAD3                 0x63    Numeric keypad 3 key
        KEY_NONE, // VK_NUMPAD4                 0x64    Numeric keypad 4 key
        KEY_NONE, // VK_NUMPAD5                 0x65    Numeric keypad 5 key
        KEY_NONE, // VK_NUMPAD6                 0x66    Numeric keypad 6 key
        KEY_NONE, // VK_NUMPAD7                 0x67    Numeric keypad 7 key
        KEY_NONE, // VK_NUMPAD8                 0x68    Numeric keypad 8 key
        KEY_NONE, // VK_NUMPAD9                 0x69    Numeric keypad 9 key
        KEY_NONE, // VK_MULTIPLY                0x6A    Multiply key
        KEY_NONE, // VK_ADD                     0x6B    Add key
        KEY_NONE, // VK_SEPARATOR               0x6C    Separator key
        KEY_NONE, // VK_SUBTRACT                0x6D    Subtract key
        KEY_NONE, // VK_DECIMAL                 0x6E    Decimal key
        KEY_NONE, // VK_DIVIDE                  0x6F    Divide key
        KEY_NONE, // VK_F1                      0x70    F1 key
        KEY_NONE, // VK_F2                      0x71    F2 key
        KEY_NONE, // VK_F3                      0x72    F3 key
        KEY_NONE, // VK_F4                      0x73    F4 key
        KEY_NONE, // VK_F5                      0x74    F5 key
        KEY_NONE, // VK_F6                      0x75    F6 key
        KEY_NONE, // VK_F7                      0x76    F7 key
        KEY_NONE, // VK_F8                      0x77    F8 key
        KEY_NONE, // VK_F9                      0x78    F9 key
        KEY_NONE, // VK_F10                     0x79    F10 key
        KEY_NONE, // VK_F11                     0x7A    F11 key
        KEY_NONE, // VK_F12                     0x7B    F12 key
        KEY_NONE, // VK_F13                     0x7C    F13 key
        KEY_NONE, // VK_F14                     0x7D    F14 key
        KEY_NONE, // VK_F15                     0x7E    F15 key
        KEY_NONE, // VK_F16                     0x7F    F16 key
        KEY_NONE, // VK_F17                     0x80    F17 key
        KEY_NONE, // VK_F18                     0x81    F18 key
        KEY_NONE, // VK_F19                     0x82    F19 key
        KEY_NONE, // VK_F20                     0x83    F20 key
        KEY_NONE, // VK_F21                     0x84    F21 key
        KEY_NONE, // VK_F22                     0x85    F22 key
        KEY_NONE, // VK_F23                     0x86    F23 key
        KEY_NONE, // VK_F24                     0x87    F24 key
        KEY_NONE, // ----                       0x88    Reserved
        KEY_NONE, // ----                       0x89    Reserved
        KEY_NONE, // ----                       0x8A    Reserved
        KEY_NONE, // ----                       0x8B    Reserved
        KEY_NONE, // ----                       0x8C    Reserved
        KEY_NONE, // ----                       0x8D    Reserved
        KEY_NONE, // ----                       0x8E    Reserved
        KEY_NONE, // ----                       0x8F    Reserved
        KEY_NONE, // VK_NUMLOCK                 0x90    NUM LOCK key
        KEY_NONE, // VK_SCROLL                  0x91    SCROLL LOCK key
        KEY_NONE, // ----                       0x92    OEM specific
        KEY_NONE, // ----                       0x93    OEM specific
        KEY_NONE, // ----                       0x94    OEM specific
        KEY_NONE, // ----                       0x95    OEM specific
        KEY_NONE, // ----                       0x96    OEM specific
        KEY_NONE, // ----                       0x97    Unassigned
        KEY_NONE, // ----                       0x98    Unassigned
        KEY_NONE, // ----                       0x99    Unassigned
        KEY_NONE, // ----                       0x9A    Unassigned
        KEY_NONE, // ----                       0x9B    Unassigned
        KEY_NONE, // ----                       0x9C    Unassigned
        KEY_NONE, // ----                       0x9D    Unassigned
        KEY_NONE, // ----                       0x9E    Unassigned
        KEY_NONE, // ----                       0x9F    Unassigned
        KEY_NONE, // VK_LSHIFT                  0xA0    Left SHIFT key
        KEY_NONE, // VK_RSHIFT                  0xA1    Right SHIFT key
        KEY_NONE, // VK_LCONTROL                0xA2    Left CONTROL key
        KEY_NONE, // VK_RCONTROL                0xA3    Right CONTROL key
        KEY_NONE, // VK_LMENU                   0xA4    Left ALT key
        KEY_NONE, // VK_RMENU                   0xA5    Right ALT key
        KEY_NONE, // VK_BROWSER_BACK            0xA6    Browser Back key
        KEY_NONE, // VK_BROWSER_FORWARD         0xA7    Browser Forward key
        KEY_NONE, // VK_BROWSER_REFRESH         0xA8    Browser Refresh key
        KEY_NONE, // VK_BROWSER_STOP            0xA9    Browser Stop key
        KEY_NONE, // VK_BROWSER_SEARCH          0xAA    Browser Search key
        KEY_NONE, // VK_BROWSER_FAVORITES       0xAB    Browser Favorites key
        KEY_NONE, // VK_BROWSER_HOME            0xAC    Browser Start and Home key
        KEY_NONE, // VK_VOLUME_MUTE             0xAD    Volume Mute key
        KEY_NONE, // VK_VOLUME_DOWN             0xAE    Volume Down key
        KEY_NONE, // VK_VOLUME_UP               0xAF    Volume Up key
        KEY_NONE, // VK_MEDIA_NEXT_TRACK        0xB0    Next Track key
        KEY_NONE, // VK_MEDIA_PREV_TRACK        0xB1    Previous Track key
        KEY_NONE, // VK_MEDIA_STOP              0xB2    Stop Media key
        KEY_NONE, // VK_MEDIA_PLAY_PAUSE        0xB3    Play/Pause Media key
        KEY_NONE, // VK_LAUNCH_MAIL             0xB4    Start Mail key
        KEY_NONE, // VK_LAUNCH_MEDIA_SELECT     0xB5    Select Media key
        KEY_NONE, // VK_LAUNCH_APP1             0xB6    Start Application 1 key
        KEY_NONE, // VK_LAUNCH_APP2             0xB7    Start Application 2 key
        KEY_NONE, // ----                       0xB8    Reserved
        KEY_NONE, // ----                       0xB9    Reserved
        KEY_NONE, // VK_OEM_1                   0xBA    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ;: key
        KEY_NONE, // VK_OEM_PLUS                0xBB    For any country/region, the + key
        KEY_NONE, // VK_OEM_COMMA               0xBC    For any country/region, the , key
        KEY_NONE, // VK_OEM_MINUS               0xBD    For any country/region, the - key
        KEY_NONE, // VK_OEM_PERIOD              0xBE    For any country/region, the . key
        KEY_NONE, // VK_OEM_2                   0xBF    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the /? key
        KEY_NONE, // VK_OEM_3                   0xC0    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the `~ key
        KEY_NONE, // ----                       0xC1    Reserved
        KEY_NONE, // ----                       0xC2    Reserved
        KEY_NONE, // ----                       0xC3    Reserved
        KEY_NONE, // ----                       0xC4    Reserved
        KEY_NONE, // ----                       0xC5    Reserved
        KEY_NONE, // ----                       0xC6    Reserved
        KEY_NONE, // ----                       0xC7    Reserved
        KEY_NONE, // ----                       0xC8    Reserved
        KEY_NONE, // ----                       0xC9    Reserved
        KEY_NONE, // ----                       0xCA    Reserved
        KEY_NONE, // ----                       0xCB    Reserved
        KEY_NONE, // ----                       0xCC    Reserved
        KEY_NONE, // ----                       0xCD    Reserved
        KEY_NONE, // ----                       0xCE    Reserved
        KEY_NONE, // ----                       0xCF    Reserved
        KEY_NONE, // ----                       0xD0    Reserved
        KEY_NONE, // ----                       0xD1    Reserved
        KEY_NONE, // ----                       0xD2    Reserved
        KEY_NONE, // ----                       0xD3    Reserved
        KEY_NONE, // ----                       0xD4    Reserved
        KEY_NONE, // ----                       0xD5    Reserved
        KEY_NONE, // ----                       0xD6    Reserved
        KEY_NONE, // ----                       0xD7    Reserved
        KEY_NONE, // ----                       0xD8    Reserved
        KEY_NONE, // ----                       0xD9    Reserved
        KEY_NONE, // ----                       0xDA    Reserved
        KEY_NONE, // VK_OEM_4                   0xDB    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the [{ key
        KEY_NONE, // VK_OEM_5                   0xDC    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \\| key
        KEY_NONE, // VK_OEM_6                   0xDD    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ]} key
        KEY_NONE, // VK_OEM_7                   0xDE    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '" key
        KEY_NONE, // VK_OEM_8                   0xDF    Used for miscellaneous characters; it can vary by keyboard.
        KEY_NONE, // ----                       0xE0    Reserved
        KEY_NONE, // ----                       0xE1    OEM specific
        KEY_NONE, // VK_OEM_102                 0xE2    The <> keys on the US standard keyboard, or the \\| key on the non-US 102-key keyboard
        KEY_NONE, // ----                       0xE3    OEM specific
        KEY_NONE, // ----                       0xE4    OEM specific
        KEY_NONE, // VK_PROCESSKEY              0xE5    IME PROCESS key
        KEY_NONE, // ----                       0xE6    OEM specific
        KEY_NONE, // VK_PACKET                  0xE7    Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
        KEY_NONE, // ----                       0xE8    Unassigned
        KEY_NONE, // ----                       0xE9    OEM specific
        KEY_NONE, // ----                       0xEA    OEM specific
        KEY_NONE, // ----                       0xEB    OEM specific
        KEY_NONE, // ----                       0xEC    OEM specific
        KEY_NONE, // ----                       0xED    OEM specific
        KEY_NONE, // ----                       0xEE    OEM specific
        KEY_NONE, // ----                       0xEF    OEM specific
        KEY_NONE, // ----                       0xF0    OEM specific
        KEY_NONE, // ----                       0xF1    OEM specific
        KEY_NONE, // ----                       0xF2    OEM specific
        KEY_NONE, // ----                       0xF3    OEM specific
        KEY_NONE, // ----                       0xF4    OEM specific
        KEY_NONE, // ----                       0xF5    OEM specific
        KEY_NONE, // VK_ATTN                    0xF6    Attn key
        KEY_NONE, // VK_CRSEL                   0xF7    CrSel key
        KEY_NONE, // VK_EXSEL                   0xF8    ExSel key
        KEY_NONE, // VK_EREOF                   0xF9    Erase EOF key
        KEY_NONE, // VK_PLAY                    0xFA    Play key
        KEY_NONE, // VK_ZOOM                    0xFB    Zoom key
        KEY_NONE, // VK_NONAME                  0xFC    Reserved
        KEY_NONE, // VK_PA1                     0xFD    PA1 key
        KEY_NONE, // VK_OEM_CLEAR               0xFE    Clear key
        KEY_NONE, // -                          0xFF    Unused
    };
    int VKCodeMapSize = ARRAY_SIZE(VKCodeMap);

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