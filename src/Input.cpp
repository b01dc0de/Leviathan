#include "Leviathan.h" // TODO: Input.h included in Leviathan.h

namespace Leviathan
{
    // NOTE: Added some blank space here to align the VKCodeMap so that
    // [0] == 0x00 == Line #100 for easy verification
    LvKeyCode VKCodeMap[] =
    {
        LV_KEY_NONE, //                            0x00    Unused
        LV_KEY_NONE, // VK_LBUTTON                 0x01    Left mouse button
        LV_KEY_NONE, // VK_RBUTTON                 0x02    Right mouse button
        LV_KEY_NONE, // VK_CANCEL                  0x03    Control-break processing
        LV_KEY_NONE, // VK_MBUTTON                 0x04    Middle mouse button
        LV_KEY_NONE, // VK_XBUTTON1                0x05    X1 mouse button
        LV_KEY_NONE, // VK_XBUTTON2                0x06    X2 mouse button
        LV_KEY_NONE, // ----                       0x07    Reserved
        LV_KEY_BACKSPACE, // VK_BACK               0x08    BACKSPACE key
        LV_KEY_TAB, // VK_TAB                      0x09    TAB key
        LV_KEY_NONE, // ----                       0x0A    Reserved
        LV_KEY_NONE, // ----                       0x0B    Reserved
        LV_KEY_NONE, // VK_CLEAR                   0x0C    CLEAR key
        LV_KEY_ENTER, // VK_RETURN                 0x0D    ENTER key
        LV_KEY_NONE, // ----                       0x0E    Unassigned
        LV_KEY_NONE, // ----                       0x0F    Unassigned
        LV_KEY_NONE, // VK_SHIFT                   0x10    SHIFT key
        LV_KEY_NONE, // VK_CONTROL                 0x11    CTRL key
        LV_KEY_NONE, // VK_MENU                    0x12    ALT key
        LV_KEY_NONE, // VK_PAUSE                   0x13    PAUSE key
        LV_KEY_CAPSLOCK, // VK_CAPITAL             0x14    CAPS LOCK key
        LV_KEY_NONE, // VK_KANA | VK_HANGUL        0x15    IME Kana mode | IME Hangul mode
        LV_KEY_NONE, // VK_IME_ON                  0x16    IME On
        LV_KEY_NONE, // VK_JUNJA                   0x17    IME Junja mode
        LV_KEY_NONE, // VK_FINAL                   0x18    IME final mode
        LV_KEY_NONE, // VK_HANJA | VK_KANJI        0x19    IME Hanja mode | IME Kanji mode
        LV_KEY_NONE, // VK_IME_OFF                 0x1A    IME Off
        LV_KEY_ESC, // VK_ESCAPE                   0x1B    ESC key
        LV_KEY_NONE, // VK_CONVERT                 0x1C    IME convert
        LV_KEY_NONE, // VK_NONCONVERT              0x1D    IME nonconvert
        LV_KEY_NONE, // VK_ACCEPT                  0x1E    IME accept
        LV_KEY_NONE, // VK_MODECHANGE              0x1F    IME mode change request
        LV_KEY_SPACE, // VK_SPACE                   0x20    SPACEBAR
        LV_KEY_NONE, // VK_PRIOR                   0x21    PAGE UP key
        LV_KEY_NONE, // VK_NEXT                    0x22    PAGE DOWN key
        LV_KEY_NONE, // VK_END                     0x23    END key
        LV_KEY_NONE, // VK_HOME                    0x24    HOME key
        LV_KEY_ARROW_LEFT, // VK_LEFT              0x25    LEFT ARROW key
        LV_KEY_ARROW_UP, // VK_UP                  0x26    UP ARROW key
        LV_KEY_ARROW_RIGHT, // VK_RIGHT            0x27    RIGHT ARROW key
        LV_KEY_ARROW_DOWN, // VK_DOWN              0x28    DOWN ARROW key
        LV_KEY_NONE, // VK_SELECT                  0x29    SELECT key
        LV_KEY_NONE, // VK_PRINT                   0x2A    PRINT key
        LV_KEY_NONE, // VK_EXECUTE                 0x2B    EXECUTE key
        LV_KEY_NONE, // VK_SNAPSHOT                0x2C    PRINT SCREEN key
        LV_KEY_NONE, // VK_INSERT                  0x2D    INS key
        LV_KEY_NONE, // VK_DELETE                  0x2E    DEL key
        LV_KEY_NONE, // VK_HELP                    0x2F    HELP key
        LV_KEY_0, //                               0x30    0 key
        LV_KEY_1, //                               0x31    1 key
        LV_KEY_2, //                               0x32    2 key
        LV_KEY_3, //                               0x33    3 key
        LV_KEY_4, //                               0x34    4 key
        LV_KEY_5, //                               0x35    5 key
        LV_KEY_6, //                               0x36    6 key
        LV_KEY_7, //                               0x37    7 key
        LV_KEY_8, //                               0x38    8 key
        LV_KEY_9, //                               0x39    9 key
        LV_KEY_NONE, // ----                       0x3A    Undefined
        LV_KEY_NONE, // ----                       0x3B    Undefined
        LV_KEY_NONE, // ----                       0x3C    Undefined
        LV_KEY_NONE, // ----                       0x3D    Undefined
        LV_KEY_NONE, // ----                       0x3E    Undefined
        LV_KEY_NONE, // ----                       0x3F    Undefined
        LV_KEY_NONE, // ----                       0x40    Undefined
        LV_KEY_A, //                               0x41    A key
        LV_KEY_B, //                               0x42    B key
        LV_KEY_C, //                               0x43    C key
        LV_KEY_D, //                               0x44    D key
        LV_KEY_E, //                               0x45    E key
        LV_KEY_F, //                               0x46    F key
        LV_KEY_G, //                               0x47    G key
        LV_KEY_H, //                               0x48    H key
        LV_KEY_I, //                               0x49    I key
        LV_KEY_J, //                               0x4A    J key
        LV_KEY_K, //                               0x4B    K key
        LV_KEY_L, //                               0x4C    L key
        LV_KEY_M, //                               0x4D    M key
        LV_KEY_N, //                               0x4E    N key
        LV_KEY_O, //                               0x4F    O key
        LV_KEY_P, //                               0x50    P key
        LV_KEY_Q, //                               0x51    Q key
        LV_KEY_R, //                               0x52    R key
        LV_KEY_S, //                               0x53    S key
        LV_KEY_T, //                               0x54    T key
        LV_KEY_U, //                               0x55    U key
        LV_KEY_V, //                               0x56    V key
        LV_KEY_W, //                               0x57    W key
        LV_KEY_X, //                               0x58    X key
        LV_KEY_Y, //                               0x59    Y key
        LV_KEY_Z, //                               0x5A    Z key
        LV_KEY_LEFT_SUPER, // VK_LWIN              0x5B    Left Windows key
        LV_KEY_RIGHT_SUPER, // VK_RWIN             0x5C    Right Windows key
        LV_KEY_NONE, // VK_APPS                    0x5D    Applications key
        LV_KEY_NONE, // ----                       0x5E    Reserved
        LV_KEY_NONE, // VK_SLEEP                   0x5F    Computer Sleep key
        LV_KEY_NONE, // VK_NUMPAD0                 0x60    Numeric keypad 0 key
        LV_KEY_NONE, // VK_NUMPAD1                 0x61    Numeric keypad 1 key
        LV_KEY_NONE, // VK_NUMPAD2                 0x62    Numeric keypad 2 key
        LV_KEY_NONE, // VK_NUMPAD3                 0x63    Numeric keypad 3 key
        LV_KEY_NONE, // VK_NUMPAD4                 0x64    Numeric keypad 4 key
        LV_KEY_NONE, // VK_NUMPAD5                 0x65    Numeric keypad 5 key
        LV_KEY_NONE, // VK_NUMPAD6                 0x66    Numeric keypad 6 key
        LV_KEY_NONE, // VK_NUMPAD7                 0x67    Numeric keypad 7 key
        LV_KEY_NONE, // VK_NUMPAD8                 0x68    Numeric keypad 8 key
        LV_KEY_NONE, // VK_NUMPAD9                 0x69    Numeric keypad 9 key
        LV_KEY_NONE, // VK_MULTIPLY                0x6A    Multiply key
        LV_KEY_NONE, // VK_ADD                     0x6B    Add key
        LV_KEY_NONE, // VK_SEPARATOR               0x6C    Separator key
        LV_KEY_NONE, // VK_SUBTRACT                0x6D    Subtract key
        LV_KEY_NONE, // VK_DECIMAL                 0x6E    Decimal key
        LV_KEY_NONE, // VK_DIVIDE                  0x6F    Divide key
        LV_KEY_F1, // VK_F1                        0x70    F1 key
        LV_KEY_F2, // VK_F2                        0x71    F2 key
        LV_KEY_F3, // VK_F3                        0x72    F3 key
        LV_KEY_F4, // VK_F4                        0x73    F4 key
        LV_KEY_F5, // VK_F5                        0x74    F5 key
        LV_KEY_F6, // VK_F6                        0x75    F6 key
        LV_KEY_F7, // VK_F7                        0x76    F7 key
        LV_KEY_F8, // VK_F8                        0x77    F8 key
        LV_KEY_F9, // VK_F9                        0x78    F9 key
        LV_KEY_F10, // VK_F10                      0x79    F10 key
        LV_KEY_F11, // VK_F11                      0x7A    F11 key
        LV_KEY_F12, // VK_F12                      0x7B    F12 key
        LV_KEY_NONE, // VK_F13                     0x7C    F13 key
        LV_KEY_NONE, // VK_F14                     0x7D    F14 key
        LV_KEY_NONE, // VK_F15                     0x7E    F15 key
        LV_KEY_NONE, // VK_F16                     0x7F    F16 key
        LV_KEY_NONE, // VK_F17                     0x80    F17 key
        LV_KEY_NONE, // VK_F18                     0x81    F18 key
        LV_KEY_NONE, // VK_F19                     0x82    F19 key
        LV_KEY_NONE, // VK_F20                     0x83    F20 key
        LV_KEY_NONE, // VK_F21                     0x84    F21 key
        LV_KEY_NONE, // VK_F22                     0x85    F22 key
        LV_KEY_NONE, // VK_F23                     0x86    F23 key
        LV_KEY_NONE, // VK_F24                     0x87    F24 key
        LV_KEY_NONE, // ----                       0x88    Reserved
        LV_KEY_NONE, // ----                       0x89    Reserved
        LV_KEY_NONE, // ----                       0x8A    Reserved
        LV_KEY_NONE, // ----                       0x8B    Reserved
        LV_KEY_NONE, // ----                       0x8C    Reserved
        LV_KEY_NONE, // ----                       0x8D    Reserved
        LV_KEY_NONE, // ----                       0x8E    Reserved
        LV_KEY_NONE, // ----                       0x8F    Reserved
        LV_KEY_NONE, // VK_NUMLOCK                 0x90    NUM LOCK key
        LV_KEY_NONE, // VK_SCROLL                  0x91    SCROLL LOCK key
        LV_KEY_NONE, // ----                       0x92    OEM specific
        LV_KEY_NONE, // ----                       0x93    OEM specific
        LV_KEY_NONE, // ----                       0x94    OEM specific
        LV_KEY_NONE, // ----                       0x95    OEM specific
        LV_KEY_NONE, // ----                       0x96    OEM specific
        LV_KEY_NONE, // ----                       0x97    Unassigned
        LV_KEY_NONE, // ----                       0x98    Unassigned
        LV_KEY_NONE, // ----                       0x99    Unassigned
        LV_KEY_NONE, // ----                       0x9A    Unassigned
        LV_KEY_NONE, // ----                       0x9B    Unassigned
        LV_KEY_NONE, // ----                       0x9C    Unassigned
        LV_KEY_NONE, // ----                       0x9D    Unassigned
        LV_KEY_NONE, // ----                       0x9E    Unassigned
        LV_KEY_NONE, // ----                       0x9F    Unassigned
        LV_KEY_LEFT_SHIFT, // VK_LSHIFT                  0xA0    Left SHIFT key
        LV_KEY_RIGHT_SHIFT, // VK_RSHIFT                  0xA1    Right SHIFT key
        LV_KEY_LEFT_CTRL, // VK_LCONTROL                0xA2    Left CONTROL key
        LV_KEY_RIGHT_CTRL, // VK_RCONTROL                0xA3    Right CONTROL key
        LV_KEY_LEFT_ALT, // VK_LMENU                   0xA4    Left ALT key
        LV_KEY_RIGHT_ALT, // VK_RMENU                   0xA5    Right ALT key
        LV_KEY_NONE, // VK_BROWSER_BACK            0xA6    Browser Back key
        LV_KEY_NONE, // VK_BROWSER_FORWARD         0xA7    Browser Forward key
        LV_KEY_NONE, // VK_BROWSER_REFRESH         0xA8    Browser Refresh key
        LV_KEY_NONE, // VK_BROWSER_STOP            0xA9    Browser Stop key
        LV_KEY_NONE, // VK_BROWSER_SEARCH          0xAA    Browser Search key
        LV_KEY_NONE, // VK_BROWSER_FAVORITES       0xAB    Browser Favorites key
        LV_KEY_NONE, // VK_BROWSER_HOME            0xAC    Browser Start and Home key
        LV_KEY_NONE, // VK_VOLUME_MUTE             0xAD    Volume Mute key
        LV_KEY_NONE, // VK_VOLUME_DOWN             0xAE    Volume Down key
        LV_KEY_NONE, // VK_VOLUME_UP               0xAF    Volume Up key
        LV_KEY_NONE, // VK_MEDIA_NEXT_TRACK        0xB0    Next Track key
        LV_KEY_NONE, // VK_MEDIA_PREV_TRACK        0xB1    Previous Track key
        LV_KEY_NONE, // VK_MEDIA_STOP              0xB2    Stop Media key
        LV_KEY_NONE, // VK_MEDIA_PLAY_PAUSE        0xB3    Play/Pause Media key
        LV_KEY_NONE, // VK_LAUNCH_MAIL             0xB4    Start Mail key
        LV_KEY_NONE, // VK_LAUNCH_MEDIA_SELECT     0xB5    Select Media key
        LV_KEY_NONE, // VK_LAUNCH_APP1             0xB6    Start Application 1 key
        LV_KEY_NONE, // VK_LAUNCH_APP2             0xB7    Start Application 2 key
        LV_KEY_NONE, // ----                       0xB8    Reserved
        LV_KEY_NONE, // ----                       0xB9    Reserved
        LV_KEY_SEMICOLON, // VK_OEM_1              0xBA    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ;: key
        LV_KEY_EQUALS, // VK_OEM_PLUS              0xBB    For any country/region, the + key
        LV_KEY_COMMA, // VK_OEM_COMMA              0xBC    For any country/region, the , key
        LV_KEY_MINUS, // VK_OEM_MINUS              0xBD    For any country/region, the - key
        LV_KEY_PERIOD, // VK_OEM_PERIOD            0xBE    For any country/region, the . key
        LV_KEY_SLASH, // VK_OEM_2                  0xBF    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the /? key
        LV_KEY_GRAVE, // VK_OEM_3                  0xC0    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the `~ key
        LV_KEY_NONE, // ----                       0xC1    Reserved
        LV_KEY_NONE, // ----                       0xC2    Reserved
        LV_KEY_NONE, // ----                       0xC3    Reserved
        LV_KEY_NONE, // ----                       0xC4    Reserved
        LV_KEY_NONE, // ----                       0xC5    Reserved
        LV_KEY_NONE, // ----                       0xC6    Reserved
        LV_KEY_NONE, // ----                       0xC7    Reserved
        LV_KEY_NONE, // ----                       0xC8    Reserved
        LV_KEY_NONE, // ----                       0xC9    Reserved
        LV_KEY_NONE, // ----                       0xCA    Reserved
        LV_KEY_NONE, // ----                       0xCB    Reserved
        LV_KEY_NONE, // ----                       0xCC    Reserved
        LV_KEY_NONE, // ----                       0xCD    Reserved
        LV_KEY_NONE, // ----                       0xCE    Reserved
        LV_KEY_NONE, // ----                       0xCF    Reserved
        LV_KEY_NONE, // ----                       0xD0    Reserved
        LV_KEY_NONE, // ----                       0xD1    Reserved
        LV_KEY_NONE, // ----                       0xD2    Reserved
        LV_KEY_NONE, // ----                       0xD3    Reserved
        LV_KEY_NONE, // ----                       0xD4    Reserved
        LV_KEY_NONE, // ----                       0xD5    Reserved
        LV_KEY_NONE, // ----                       0xD6    Reserved
        LV_KEY_NONE, // ----                       0xD7    Reserved
        LV_KEY_NONE, // ----                       0xD8    Reserved
        LV_KEY_NONE, // ----                       0xD9    Reserved
        LV_KEY_NONE, // ----                       0xDA    Reserved
        LV_KEY_LEFT_BRACKET, // VK_OEM_4                   0xDB    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the [{ key
        LV_KEY_BACKSLASH, // VK_OEM_5                   0xDC    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \\| key
        LV_KEY_RIGHT_BRACKET, // VK_OEM_6                   0xDD    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ]} key
        LV_KEY_QUOTE, // VK_OEM_7                   0xDE    Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '" key
        LV_KEY_NONE, // VK_OEM_8                   0xDF    Used for miscellaneous characters; it can vary by keyboard.
        LV_KEY_NONE, // ----                       0xE0    Reserved
        LV_KEY_NONE, // ----                       0xE1    OEM specific
        LV_KEY_NONE, // VK_OEM_102                 0xE2    The <> keys on the US standard keyboard, or the \\| key on the non-US 102-key keyboard
        LV_KEY_NONE, // ----                       0xE3    OEM specific
        LV_KEY_NONE, // ----                       0xE4    OEM specific
        LV_KEY_NONE, // VK_PROCESSLV_KEY              0xE5    IME PROCESS key
        LV_KEY_NONE, // ----                       0xE6    OEM specific
        LV_KEY_NONE, // VK_PACKET                  0xE7    Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in LV_KEYBDINPUT, SendInput, WM_LV_KEYDOWN, and WM_LV_KEYUP
        LV_KEY_NONE, // ----                       0xE8    Unassigned
        LV_KEY_NONE, // ----                       0xE9    OEM specific
        LV_KEY_NONE, // ----                       0xEA    OEM specific
        LV_KEY_NONE, // ----                       0xEB    OEM specific
        LV_KEY_NONE, // ----                       0xEC    OEM specific
        LV_KEY_NONE, // ----                       0xED    OEM specific
        LV_KEY_NONE, // ----                       0xEE    OEM specific
        LV_KEY_NONE, // ----                       0xEF    OEM specific
        LV_KEY_NONE, // ----                       0xF0    OEM specific
        LV_KEY_NONE, // ----                       0xF1    OEM specific
        LV_KEY_NONE, // ----                       0xF2    OEM specific
        LV_KEY_NONE, // ----                       0xF3    OEM specific
        LV_KEY_NONE, // ----                       0xF4    OEM specific
        LV_KEY_NONE, // ----                       0xF5    OEM specific
        LV_KEY_NONE, // VK_ATTN                    0xF6    Attn key
        LV_KEY_NONE, // VK_CRSEL                   0xF7    CrSel key
        LV_KEY_NONE, // VK_EXSEL                   0xF8    ExSel key
        LV_KEY_NONE, // VK_EREOF                   0xF9    Erase EOF key
        LV_KEY_NONE, // VK_PLAY                    0xFA    Play key
        LV_KEY_NONE, // VK_ZOOM                    0xFB    Zoom key
        LV_KEY_NONE, // VK_NONAME                  0xFC    Reserved
        LV_KEY_NONE, // VK_PA1                     0xFD    PA1 key
        LV_KEY_NONE, // VK_OEM_CLEAR               0xFE    Clear key
        LV_KEY_NONE, // -                          0xFF    Unused
    };
    int VKCodeMapSize = ARRAY_SIZE(VKCodeMap);

    LvKeyCode KeyboardState::ActiveKeys[] =
    {
        LV_KEY_NONE, LV_KEY_NONE, LV_KEY_NONE, LV_KEY_NONE,
        LV_KEY_NONE, LV_KEY_NONE, LV_KEY_NONE, LV_KEY_NONE
    };
    int KeyboardState::ActiveCount = 0;

    LvKeyCode KeyboardState::VkToLv(int VkCode)
    {
        if (VkCode <= 0 || VkCode > VKCodeMapSize)
        {
            return LV_KEY_NONE;
        }
        return VKCodeMap[VkCode];
    }

    bool KeyboardState::GetKeyState(LvKeyCode KeyCode)
    {
        for (int KeyIdx = 0; KeyIdx < ActiveCount; KeyIdx++)
        {
            if (ActiveKeys[KeyIdx] == KeyCode)
            {
                return true;
            }
        }
        return false;
    }

    void KeyboardState::SetKeyUp(int VkCode)
    {
        if (ActiveCount == 0) { return; }

        LvKeyCode LvCode = VkToLv(VkCode);
        if (LvCode == LV_KEY_NONE) { return; }

        bool bKeyFound = false;
        int KeyIdx = 0;
        while (KeyIdx < ActiveCount)
        {
            if (ActiveKeys[KeyIdx] == LvCode)
            {
                bKeyFound = true;
                break;
            }
            KeyIdx++;
        }
        while (KeyIdx < ActiveCount)
        {
            if (KeyIdx == ActiveCount - 1)
            {
                ActiveKeys[KeyIdx] = LV_KEY_NONE;
            }
            else
            {
                ActiveKeys[KeyIdx] = ActiveKeys[KeyIdx + 1];
            }
            KeyIdx++;
        }
        if (bKeyFound) { ActiveCount--; }
    }

    void KeyboardState::SetKeyDown(int VkCode)
    {
        if (ActiveCount == KeyCount) {
            return;
        }

        LvKeyCode LvCode = VkToLv(VkCode);
        if (LvCode == LV_KEY_NONE) { return; }

        for (int KeyIdx = 0; KeyIdx < KeyCount; KeyIdx++)
        {
            if (ActiveKeys[KeyIdx] == LvCode)
            {
                return;
            }
            if (ActiveKeys[KeyIdx] == LV_KEY_NONE)
            {
                ActiveKeys[KeyIdx] = LvCode;
                ActiveCount++;
                return;
            }
        }
    }

    struct VisualKey
    {
        LvKeyCode LvCode = LV_KEY_NONE;
        v2f Size{ 1.0f, 1.0f };
    };

    struct VisualKeyboard
    {
        static constexpr float KeySize = 25.0f;
        static VisualKey Row0[LV_KEY_F12 - LV_KEY_ESC + 1];
        static VisualKey Row1[LV_KEY_BACKSPACE - LV_KEY_GRAVE + 1];
        static VisualKey Row2[LV_KEY_BACKSLASH - LV_KEY_TAB + 1];
        static VisualKey Row3[LV_KEY_ENTER - LV_KEY_CAPSLOCK + 1];
        static VisualKey Row4[LV_KEY_RIGHT_SHIFT - LV_KEY_LEFT_SHIFT + 1];
        static VisualKey Row5[LV_KEY_RIGHT_CTRL - LV_KEY_LEFT_CTRL + 1];
        static void DrawKey(ID2D1RenderTarget* D2_RenderTarget, ID2D1Brush* InBrush, bool bIsDown, float HalfKeySize, const v2f& KeyPos);
        static void DrawRow(ID2D1RenderTarget* InRT, ID2D1Brush* InBrush, const VisualKey* Row, int KeyCount, const v2f& Pos);
        static void DrawKeyboard(ID2D1RenderTarget* InRT, ID2D1Brush* InBrush);
    };

    VisualKey VisualKeyboard::Row0[] = { {LV_KEY_ESC},
        {LV_KEY_F1}, {LV_KEY_F2}, {LV_KEY_F3}, {LV_KEY_F4},
        {LV_KEY_F5}, {LV_KEY_F6}, {LV_KEY_F7}, {LV_KEY_F8},
        {LV_KEY_F9}, {LV_KEY_F10}, {LV_KEY_F11}, {LV_KEY_F12},
    };
    VisualKey VisualKeyboard::Row1[] = { {LV_KEY_GRAVE},
        {LV_KEY_1}, {LV_KEY_2}, {LV_KEY_3}, {LV_KEY_4}, {LV_KEY_5},
        {LV_KEY_6}, {LV_KEY_7}, {LV_KEY_8}, {LV_KEY_9}, {LV_KEY_0},
        {LV_KEY_MINUS}, {LV_KEY_EQUALS}, {LV_KEY_BACKSPACE}
    };
    VisualKey VisualKeyboard::Row2[] = { {LV_KEY_TAB},
        {LV_KEY_Q}, {LV_KEY_W}, {LV_KEY_E}, {LV_KEY_R}, {LV_KEY_T},
        {LV_KEY_Y}, {LV_KEY_U}, {LV_KEY_I}, {LV_KEY_O}, {LV_KEY_P},
        {LV_KEY_LEFT_BRACKET}, {LV_KEY_RIGHT_BRACKET}, {LV_KEY_BACKSLASH}
    };
    VisualKey VisualKeyboard::Row3[] = { {LV_KEY_CAPSLOCK},
        {LV_KEY_A}, {LV_KEY_S}, {LV_KEY_D}, {LV_KEY_F}, {LV_KEY_G},
        {LV_KEY_H}, {LV_KEY_J}, {LV_KEY_K}, {LV_KEY_L},
        {LV_KEY_SEMICOLON}, {LV_KEY_QUOTE}, {LV_KEY_ENTER},
    };
    VisualKey VisualKeyboard::Row4[] = { {LV_KEY_LEFT_SHIFT},
        {LV_KEY_Z}, {LV_KEY_X}, {LV_KEY_C}, {LV_KEY_V}, {LV_KEY_B}, {LV_KEY_N}, {LV_KEY_M},
        {LV_KEY_COMMA}, {LV_KEY_PERIOD}, {LV_KEY_SLASH}, {LV_KEY_RIGHT_SHIFT},
    };
    VisualKey VisualKeyboard::Row5[] = {
        {LV_KEY_LEFT_CTRL}, {LV_KEY_LEFT_SUPER}, {LV_KEY_LEFT_ALT},
        {LV_KEY_SPACE},
        {LV_KEY_RIGHT_ALT}, {LV_KEY_RIGHT_SUPER}, {LV_KEY_RIGHT_CTRL}
    };

    void VisualKeyboard::DrawKey(ID2D1RenderTarget* D2_RenderTarget, ID2D1Brush* InBrush, bool bIsDown, float HalfKeySize, const v2f& KeyPos)
    {
        D2D1_RECT_F KeyRect
        {
            KeyPos.X - HalfKeySize,
            KeyPos.Y - HalfKeySize,
            KeyPos.X + HalfKeySize,
            KeyPos.Y + HalfKeySize
        };
        if (bIsDown)
        {
            D2_RenderTarget->FillRectangle(&KeyRect, InBrush);
        }
        else
        {
            D2_RenderTarget->DrawRectangle(&KeyRect, InBrush, 1.0f, nullptr);
        }
    }

    void VisualKeyboard::DrawRow(ID2D1RenderTarget* InRT, ID2D1Brush* InBrush, const VisualKey* Row, int KeyCount, const v2f& Pos)
    {
        float HalfKeySize = KeySize * 0.5f;
        float KeyX = Pos.X;
        for (int KeyIdx = 0; KeyIdx < KeyCount; KeyIdx++)
        {
            const VisualKey& CurrKey = Row[KeyIdx];
            v2f CurrKeyPos{ KeyX, Pos.Y };
            DrawKey(InRT, InBrush, KeyboardState::GetKeyState(CurrKey.LvCode), HalfKeySize, CurrKeyPos);
            KeyX += KeySize;
        }
    }

    void VisualKeyboard::DrawKeyboard(ID2D1RenderTarget* InRT, ID2D1Brush* InBrush)
    {
        float StartX = 800.0f;
        v2f Row0_Pos{ StartX, KeySize };
        v2f Row1_Pos{ StartX, KeySize * 2.0f };
        v2f Row2_Pos{ StartX, KeySize * 3.0f };
        v2f Row3_Pos{ StartX, KeySize * 4.0f };
        v2f Row4_Pos{ StartX, KeySize * 5.0f };
        v2f Row5_Pos{ StartX, KeySize * 6.0f };
        DrawRow(InRT, InBrush, Row0, ARRAY_SIZE(Row0), Row0_Pos);
        DrawRow(InRT, InBrush, Row1, ARRAY_SIZE(Row1), Row1_Pos);
        DrawRow(InRT, InBrush, Row2, ARRAY_SIZE(Row2), Row2_Pos);
        DrawRow(InRT, InBrush, Row3, ARRAY_SIZE(Row3), Row3_Pos);
        DrawRow(InRT, InBrush, Row4, ARRAY_SIZE(Row4), Row4_Pos);
        DrawRow(InRT, InBrush, Row5, ARRAY_SIZE(Row5), Row5_Pos);
    }

    void InputVisualizer::Draw(ID2D1RenderTarget* D2_RenderTarget, ID2D1Brush* InBrush)
    {
        float KeySize = 25.0f;
        float HalfKeySize = KeySize * 0.5f;
        v2f BaseKeyPos{AppWidth - 100.0f, 100.0f};
        v2f ArrowUpPos = BaseKeyPos;
        v2f ArrowLeftPos{ BaseKeyPos.X - KeySize, BaseKeyPos.Y + KeySize };
        v2f ArrowDownPos{ ArrowLeftPos.X + KeySize, ArrowLeftPos.Y };
        v2f ArrowRightPos{ ArrowDownPos.X + KeySize, ArrowLeftPos.Y };
        VisualKeyboard::DrawKey(D2_RenderTarget, InBrush, KeyboardState::GetKeyState(LV_KEY_ARROW_UP), HalfKeySize, ArrowUpPos);
        VisualKeyboard::DrawKey(D2_RenderTarget, InBrush, KeyboardState::GetKeyState(LV_KEY_ARROW_LEFT), HalfKeySize, ArrowLeftPos);
        VisualKeyboard::DrawKey(D2_RenderTarget, InBrush, KeyboardState::GetKeyState(LV_KEY_ARROW_DOWN), HalfKeySize, ArrowDownPos);
        VisualKeyboard::DrawKey(D2_RenderTarget, InBrush, KeyboardState::GetKeyState(LV_KEY_ARROW_RIGHT), HalfKeySize, ArrowRightPos);

        VisualKeyboard::DrawKeyboard(D2_RenderTarget, InBrush);
    }
}