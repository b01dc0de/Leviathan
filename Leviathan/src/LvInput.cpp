#include "LvCommon.h"

#include "LvEngine.h"
#include "LvInput.h"

namespace Leviathan
{
	inline namespace LvInput
	{
		LVINPUT_KEYCODE const VK_TO_LVINPUT[] =
		{
			// Constant 	Value 	Description
			LVINPUT_INVALID,
			LVINPUT_INVALID, // VK_LBUTTON 	0x01 	Left mouse button
			LVINPUT_INVALID, // VK_RBUTTON 	0x02 	Right mouse button
			LVINPUT_UNDEFINED, // VK_CANCEL 	0x03 	Control - break processing
			LVINPUT_INVALID, // VK_MBUTTON 	0x04 	Middle mouse button
			LVINPUT_UNDEFINED, // VK_XBUTTON1 	0x05 	X1 mouse button
			LVINPUT_UNDEFINED, // VK_XBUTTON2 	0x06 	X2 mouse button
			LVINPUT_RESERVED, // - 0x07 	Reserved
			LVINPUT_BACKSPACE, // VK_BACK 	0x08 	BACKSPACE key
			LVINPUT_TAB, // VK_TAB 	0x09 	TAB key
			LVINPUT_RESERVED, // 0x0A 	Reserved
			LVINPUT_RESERVED, // 0x0B 	Reserved
			LVINPUT_UNDEFINED, // VK_CLEAR 	0x0C 	CLEAR key
			LVINPUT_ENTER, // VK_RETURN 	0x0D 	ENTER key
			 // - 0x0E - 0F 	Unassigned
			LVINPUT_UNDEFINED, // 0x0E
			LVINPUT_UNDEFINED, // 0x0F
			LVINPUT_SHIFT, // VK_SHIFT 	0x10 	SHIFT key
			LVINPUT_CTRL, // VK_CONTROL 	0x11 	CTRL key
			LVINPUT_ALT, // VK_MENU 	0x12 	ALT key
			LVINPUT_UNDEFINED, // VK_PAUSE 	0x13 	PAUSE key
			LVINPUT_CAPSLOCK, // VK_CAPITAL 	0x14 	CAPS LOCK key
			LVINPUT_UNDEFINED, // VK_KANA VK_HANGUL		0x15 	IME Kana mode / IME Hangul mode
			LVINPUT_UNDEFINED, // VK_IME_ON 	0x16 	IME On
			LVINPUT_UNDEFINED, // VK_JUNJA 	0x17 	IME Junja mode
			LVINPUT_UNDEFINED, // VK_FINAL 	0x18 	IME final mode
			LVINPUT_UNDEFINED, // VK_HANJA VK_KANJI		0x19 	IME Hanja mode / IME Kanji mode
			LVINPUT_UNDEFINED, // VK_IME_OFF 	0x1A 	IME Off
			LVINPUT_ESC, // VK_ESCAPE 	0x1B 	ESC key
			LVINPUT_UNDEFINED, // VK_CONVERT 	0x1C 	IME convert
			LVINPUT_UNDEFINED, // VK_NONCONVERT 	0x1D 	IME nonconvert
			LVINPUT_UNDEFINED, // VK_ACCEPT 	0x1E 	IME accept
			LVINPUT_UNDEFINED, // VK_MODECHANGE 	0x1F 	IME mode change request
			LVINPUT_SPACE, // VK_SPACE 	0x20 	SPACEBAR
			LVINPUT_PAGEUP, // VK_PRIOR 	0x21 	PAGE UP key
			LVINPUT_PAGEDOWN, // VK_NEXT 	0x22 	PAGE DOWN key
			LVINPUT_END, // VK_END 	0x23 	END key
			LVINPUT_HOME, // VK_HOME 	0x24 	HOME key
			LVINPUT_LEFT, // VK_LEFT 	0x25 	LEFT ARROW key
			LVINPUT_UP, // VK_UP 	0x26 	UP ARROW key
			LVINPUT_RIGHT, // VK_RIGHT 	0x27 	RIGHT ARROW key
			LVINPUT_DOWN, // VK_DOWN 	0x28 	DOWN ARROW key
			LVINPUT_UNDEFINED, // VK_SELECT 	0x29 	SELECT key
			LVINPUT_UNDEFINED, // VK_PRINT 	0x2A 	PRINT key
			LVINPUT_UNDEFINED, // VK_EXECUTE 	0x2B 	EXECUTE key
			LVINPUT_PRINTSCREEN, // VK_SNAPSHOT 	0x2C 	PRINT SCREEN key
			LVINPUT_INSERT, // VK_INSERT 	0x2D 	INS key
			LVINPUT_DEL, // VK_DELETE 	0x2E 	DEL key
			LVINPUT_UNDEFINED, // VK_HELP 	0x2F 	HELP key
			LVINPUT_KEY_0, // 	0x30 	0 key
			LVINPUT_KEY_1, // 	0x31 	1 key
			LVINPUT_KEY_2, // 	0x32 	2 key
			LVINPUT_KEY_3, // 	0x33 	3 key
			LVINPUT_KEY_4, // 	0x34 	4 key
			LVINPUT_KEY_5, // 	0x35 	5 key
			LVINPUT_KEY_6, // 	0x36 	6 key
			LVINPUT_KEY_7, // 	0x37 	7 key
			LVINPUT_KEY_8, // 	0x38 	8 key
			LVINPUT_KEY_9, // 	0x39 	9 key
			// - 0x3A - 40 	Undefined
			LVINPUT_UNDEFINED, // 0x3A
			LVINPUT_UNDEFINED, // 0x3B
			LVINPUT_UNDEFINED, // 0x3C
			LVINPUT_UNDEFINED, // 0x3D
			LVINPUT_UNDEFINED, // 0x3E
			LVINPUT_UNDEFINED, // 0x3F
			LVINPUT_UNDEFINED, // 0x40
			LVINPUT_KEY_A, // 	0x41 	A key
			LVINPUT_KEY_B, // 	0x42 	B key
			LVINPUT_KEY_C, // 	0x43 	C key
			LVINPUT_KEY_D, // 	0x44 	D key
			LVINPUT_KEY_E, // 	0x45 	E key
			LVINPUT_KEY_F, // 	0x46 	F key
			LVINPUT_KEY_G, // 	0x47 	G key
			LVINPUT_KEY_H, // 	0x48 	H key
			LVINPUT_KEY_I, // 	0x49 	I key
			LVINPUT_KEY_J, // 	0x4A 	J key
			LVINPUT_KEY_K, // 	0x4B 	K key
			LVINPUT_KEY_L, // 	0x4C 	L key
			LVINPUT_KEY_M, // 	0x4D 	M key
			LVINPUT_KEY_N, // 	0x4E 	N key
			LVINPUT_KEY_O, // 	0x4F 	O key
			LVINPUT_KEY_P, // 	0x50 	P key
			LVINPUT_KEY_Q, // 	0x51 	Q key
			LVINPUT_KEY_R, // 	0x52 	R key
			LVINPUT_KEY_S, // 	0x53 	S key
			LVINPUT_KEY_T, // 	0x54 	T key
			LVINPUT_KEY_U, // 	0x55 	U key
			LVINPUT_KEY_V, // 	0x56 	V key
			LVINPUT_KEY_W, // 	0x57 	W key
			LVINPUT_KEY_X, // 	0x58 	X key
			LVINPUT_KEY_Y, // 	0x59 	Y key
			LVINPUT_KEY_Z, // 	0x5A 	Z key
			LVINPUT_SUPER, // VK_LWIN 	0x5B 	Left Windows key
			LVINPUT_SUPER, // VK_RWIN 	0x5C 	Right Windows key
			LVINPUT_UNDEFINED, // VK_APPS 	0x5D 	Applications key
			LVINPUT_RESERVED, // - 0x5E 	Reserved
			LVINPUT_UNDEFINED, // VK_SLEEP 	0x5F 	Computer Sleep key
			LVINPUT_NUMPAD0, // VK_NUMPAD0 	0x60 	Numeric keypad 0 key
			LVINPUT_NUMPAD1, // VK_NUMPAD1 	0x61 	Numeric keypad 1 key
			LVINPUT_NUMPAD2, // VK_NUMPAD2 	0x62 	Numeric keypad 2 key
			LVINPUT_NUMPAD3, // VK_NUMPAD3 	0x63 	Numeric keypad 3 key
			LVINPUT_NUMPAD4, // VK_NUMPAD4 	0x64 	Numeric keypad 4 key
			LVINPUT_NUMPAD5, // VK_NUMPAD5 	0x65 	Numeric keypad 5 key
			LVINPUT_NUMPAD6, // VK_NUMPAD6 	0x66 	Numeric keypad 6 key
			LVINPUT_NUMPAD7, // VK_NUMPAD7 	0x67 	Numeric keypad 7 key
			LVINPUT_NUMPAD8, // VK_NUMPAD8 	0x68 	Numeric keypad 8 key
			LVINPUT_NUMPAD9, // VK_NUMPAD9 	0x69 	Numeric keypad 9 key
			LVINPUT_MULTIPLY, // VK_MULTIPLY 	0x6A 	Multiply key
			LVINPUT_ADD, // VK_ADD 	0x6B 	Add key
			LVINPUT_UNDEFINED, // VK_SEPARATOR 	0x6C 	Separator key
			LVINPUT_SUBTRACT, // VK_SUBTRACT 	0x6D 	Subtract key
			LVINPUT_DECIMAL, // VK_DECIMAL 	0x6E 	Decimal key
			LVINPUT_DIVIDE, // VK_DIVIDE 	0x6F 	Divide key
			LVINPUT_KEY_F1, // VK_F1 	0x70 	F1 key
			LVINPUT_KEY_F2, // VK_F2 	0x71 	F2 key
			LVINPUT_KEY_F3, // VK_F3 	0x72 	F3 key
			LVINPUT_KEY_F4, // VK_F4 	0x73 	F4 key
			LVINPUT_KEY_F5, // VK_F5 	0x74 	F5 key
			LVINPUT_KEY_F6, // VK_F6 	0x75 	F6 key
			LVINPUT_KEY_F7, // VK_F7 	0x76 	F7 key
			LVINPUT_KEY_F8, // VK_F8 	0x77 	F8 key
			LVINPUT_KEY_F9, // VK_F9 	0x78 	F9 key
			LVINPUT_KEY_F10, // VK_F10 	0x79 	F10 key
			LVINPUT_KEY_F11, // VK_F11 	0x7A 	F11 key
			LVINPUT_KEY_F12, // VK_F12 	0x7B 	F12 key
			LVINPUT_UNDEFINED, // VK_F13 	0x7C 	F13 key
			LVINPUT_UNDEFINED, // VK_F14 	0x7D 	F14 key
			LVINPUT_UNDEFINED, // VK_F15 	0x7E 	F15 key
			LVINPUT_UNDEFINED, // VK_F16 	0x7F 	F16 key
			LVINPUT_UNDEFINED, // VK_F17 	0x80 	F17 key
			LVINPUT_UNDEFINED, // VK_F18 	0x81 	F18 key
			LVINPUT_UNDEFINED, // VK_F19 	0x82 	F19 key
			LVINPUT_UNDEFINED, // VK_F20 	0x83 	F20 key
			LVINPUT_UNDEFINED, // VK_F21 	0x84 	F21 key
			LVINPUT_UNDEFINED, // VK_F22 	0x85 	F22 key
			LVINPUT_UNDEFINED, // VK_F23 	0x86 	F23 key
			LVINPUT_UNDEFINED, // VK_F24 	0x87 	F24 key
			// - 0x88 - 8F 	Reserved
			LVINPUT_RESERVED,  // 0x88
			LVINPUT_RESERVED,  // 0x89
			LVINPUT_RESERVED,  // 0x8A
			LVINPUT_RESERVED,  // 0x8B
			LVINPUT_RESERVED,  // 0x8C
			LVINPUT_RESERVED,  // 0x8D
			LVINPUT_RESERVED,  // 0x8E
			LVINPUT_RESERVED,  // 0x8F
			LVINPUT_UNDEFINED, // VK_NUMLOCK 	0x90 	NUM LOCK key
			LVINPUT_UNDEFINED, // VK_SCROLL 	0x91 	SCROLL LOCK key
			// - 0x92 - 96 	OEM specific
			LVINPUT_UNDEFINED, // 0x92
			LVINPUT_UNDEFINED, // 0x93
			LVINPUT_UNDEFINED, // 0x94
			LVINPUT_UNDEFINED, // 0x95
			LVINPUT_UNDEFINED, // 0x96
			// - 0x97 - 9F 	Unassigned
			LVINPUT_UNDEFINED, // 0x97
			LVINPUT_UNDEFINED, // 0x98
			LVINPUT_UNDEFINED, // 0x99
			LVINPUT_UNDEFINED, // 0x9A
			LVINPUT_UNDEFINED, // 0x9B
			LVINPUT_UNDEFINED, // 0x9C
			LVINPUT_UNDEFINED, // 0x9D
			LVINPUT_UNDEFINED, // 0x9E
			LVINPUT_UNDEFINED, // 0x9F
			LVINPUT_SHIFT, // VK_LSHIFT 	0xA0 	Left SHIFT key
			LVINPUT_SHIFT, // VK_RSHIFT 	0xA1 	Right SHIFT key
			LVINPUT_CTRL, // VK_LCONTROL 	0xA2 	Left CONTROL key
			LVINPUT_CTRL, // VK_RCONTROL 	0xA3 	Right CONTROL key
			LVINPUT_ALT, // VK_LMENU 	0xA4 	Left ALT key
			LVINPUT_ALT, // VK_RMENU 	0xA5 	Right ALT key
			LVINPUT_UNDEFINED, // VK_BROWSER_BACK 	0xA6 	Browser Back key
			LVINPUT_UNDEFINED, // VK_BROWSER_FORWARD 	0xA7 	Browser Forward key
			LVINPUT_UNDEFINED, // VK_BROWSER_REFRESH 	0xA8 	Browser Refresh key
			LVINPUT_UNDEFINED, // VK_BROWSER_STOP 	0xA9 	Browser Stop key
			LVINPUT_UNDEFINED, // VK_BROWSER_SEARCH 	0xAA 	Browser Search key
			LVINPUT_UNDEFINED, // VK_BROWSER_FAVORITES 	0xAB 	Browser Favorites key
			LVINPUT_UNDEFINED, // VK_BROWSER_HOME 	0xAC 	Browser Start and Home key
			LVINPUT_UNDEFINED, // VK_VOLUME_MUTE 	0xAD 	Volume Mute key
			LVINPUT_UNDEFINED, // VK_VOLUME_DOWN 	0xAE 	Volume Down key
			LVINPUT_UNDEFINED, // VK_VOLUME_UP 	0xAF 	Volume Up key
			LVINPUT_UNDEFINED, // VK_MEDIA_NEXT_TRACK 	0xB0 	Next Track key
			LVINPUT_UNDEFINED, // VK_MEDIA_PREV_TRACK 	0xB1 	Previous Track key
			LVINPUT_UNDEFINED, // VK_MEDIA_STOP 	0xB2 	Stop Media key
			LVINPUT_UNDEFINED, // VK_MEDIA_PLAY_PAUSE 	0xB3 	Play / Pause Media key
			LVINPUT_UNDEFINED, // VK_LAUNCH_MAIL 	0xB4 	Start Mail key
			LVINPUT_UNDEFINED, // VK_LAUNCH_MEDIA_SELECT 	0xB5 	Select Media key
			LVINPUT_UNDEFINED, // VK_LAUNCH_APP1 	0xB6 	Start Application 1 key
			LVINPUT_UNDEFINED, // VK_LAUNCH_APP2 	0xB7 	Start Application 2 key
			// - 0xB8 - B9 	Reserved
			LVINPUT_RESERVED, // 0xB8
			LVINPUT_RESERVED, // 0xB9
			LVINPUT_KEY_SEMICOLON, // VK_OEM_1 	0xBA 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the; : key
			LVINPUT_KEY_EQUAL, // VK_OEM_PLUS 	0xBB 	For any country / region, the + key
			LVINPUT_KEY_COMMA, // VK_OEM_COMMA 	0xBC 	For any country / region, the , key
			LVINPUT_KEY_MINUS, // VK_OEM_MINUS 	0xBD 	For any country / region, the - key
			LVINPUT_KEY_PERIOD, // VK_OEM_PERIOD 	0xBE 	For any country / region, the.key
			LVINPUT_KEY_SLASH, // VK_OEM_2 	0xBF 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the / ? key
			LVINPUT_KEY_BACKTICK, // VK_OEM_3 	0xC0 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the `~ key
			// - 0xC1 - DA 	Reserved
			LVINPUT_RESERVED, // 0xC1
			LVINPUT_RESERVED, // 0xC2
			LVINPUT_RESERVED, // 0xC3
			LVINPUT_RESERVED, // 0xC4
			LVINPUT_RESERVED, // 0xC5
			LVINPUT_RESERVED, // 0xC6
			LVINPUT_RESERVED, // 0xC7
			LVINPUT_RESERVED, // 0xC8
			LVINPUT_RESERVED, // 0xC9
			LVINPUT_RESERVED, // 0xCA
			LVINPUT_RESERVED, // 0xCB
			LVINPUT_RESERVED, // 0xCC
			LVINPUT_RESERVED, // 0xCD
			LVINPUT_RESERVED, // 0xCE
			LVINPUT_RESERVED, // 0xCF
			LVINPUT_RESERVED, // 0xD0
			LVINPUT_RESERVED, // 0xD1
			LVINPUT_RESERVED, // 0xD2
			LVINPUT_RESERVED, // 0xD3
			LVINPUT_RESERVED, // 0xD4
			LVINPUT_RESERVED, // 0xD5
			LVINPUT_RESERVED, // 0xD6
			LVINPUT_RESERVED, // 0xD7
			LVINPUT_RESERVED, // 0xD8
			LVINPUT_RESERVED, // 0xD9
			LVINPUT_RESERVED, // 0xDA
			LVINPUT_KEY_LBRACKET, // VK_OEM_4 	0xDB 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the[{ key
			LVINPUT_KEY_BACKSLASH, // VK_OEM_5 	0xDC 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the \\ | key
			LVINPUT_KEY_RBRACKET, // VK_OEM_6 	0xDD 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the]} key
			LVINPUT_KEY_QUOTE, // VK_OEM_7 	0xDE 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the '" key
			LVINPUT_UNDEFINED, // VK_OEM_8 	0xDF 	Used for miscellaneous characters; it can vary by keyboard.
			LVINPUT_RESERVED, // - 0xE0 	Reserved
			LVINPUT_UNDEFINED, // - 0xE1 	OEM specific
			LVINPUT_UNDEFINED, // VK_OEM_102 	0xE2 	The <> keys on the US standard keyboard, or the \\ | key on the non - US 102 - key keyboard
			// - 0xE3 - E4 	OEM specific
			LVINPUT_UNDEFINED, // 0xE3
			LVINPUT_UNDEFINED, // 0xE4
			LVINPUT_UNDEFINED, // VK_PROCESSKEY 	0xE5 	IME PROCESS key
			LVINPUT_UNDEFINED, // - 0xE6 	OEM specific
			LVINPUT_UNDEFINED, // VK_PACKET 	0xE7 	Used to pass Unicode characters as if they were keystrokes.The VK_PACKET key is the low word of a 32 - bit Virtual Key value used for non - keyboard input methods.For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
			LVINPUT_UNDEFINED, // - 0xE8 	Unassigned
			// - 0xE9 - F5 	OEM specific
			LVINPUT_UNDEFINED, // 0xE9
			LVINPUT_UNDEFINED, // 0xEA
			LVINPUT_UNDEFINED, // 0xEB
			LVINPUT_UNDEFINED, // 0xEC
			LVINPUT_UNDEFINED, // 0xED
			LVINPUT_UNDEFINED, // 0xEE
			LVINPUT_UNDEFINED, // 0xEF
			LVINPUT_UNDEFINED, // 0xF0
			LVINPUT_UNDEFINED, // 0xF1
			LVINPUT_UNDEFINED, // 0xF2
			LVINPUT_UNDEFINED, // 0xF3
			LVINPUT_UNDEFINED, // 0xF4
			LVINPUT_UNDEFINED, // 0xF5
			LVINPUT_UNDEFINED, // VK_ATTN 	0xF6 	Attn key
			LVINPUT_UNDEFINED, // VK_CRSEL 	0xF7 	CrSel key
			LVINPUT_UNDEFINED, // VK_EXSEL 	0xF8 	ExSel key
			LVINPUT_UNDEFINED, // VK_EREOF 	0xF9 	Erase EOF key
			LVINPUT_UNDEFINED, // VK_PLAY 	0xFA 	Play key
			LVINPUT_UNDEFINED, // VK_ZOOM 	0xFB 	Zoom key
			LVINPUT_RESERVED, // VK_NONAME 	0xFC 	Reserved
			LVINPUT_UNDEFINED, // VK_PA1 	0xFD 	PA1 key
			LVINPUT_UNDEFINED // VK_OEM_CLEAR 	0xFE 	Clear key
		};
		constexpr unsigned int VK2LVINPUT_TABLE_SIZE = LV_ARRAYSIZE(VK_TO_LVINPUT);

		LVINPUT_KEYCODE WindowsVK_To_LvInput(unsigned int VirtualKey)
		{
			LVINPUT_KEYCODE Result = LVINPUT_INVALID;
			if (VirtualKey < VK2LVINPUT_TABLE_SIZE)
			{
				Result = VK_TO_LVINPUT[VirtualKey];
			}
			return Result;
		}

		bool LvKeyboardState::IsKeyPressed(u32 InVK)
		{
			for (u32 KeyIdx = 0; KeyIdx < CurrPressedCount; KeyIdx++)
			{
				if (InVK == CurrInput[KeyIdx].virtualKey)
				{
					return true;
				}
			}
			return false;
		}

		bool LvKeyboardState::IsKeyPressed(LVINPUT_KEYCODE InLKC)
		{
			for (u32 KeyIdx = 0; KeyIdx < CurrPressedCount; KeyIdx++)
			{
				if (InLKC == WindowsVK_To_LvInput(CurrInput[KeyIdx].virtualKey))
				{
					return true;
				}
			}
			return false;
		}

		bool LvKeyboardState::IsKeyComboPressed(LvKeyInputCombo InKeys)
		{
			return (InKeys.LKC0 == 0 ? true : IsKeyPressed(InKeys.LKC0)) &&
				(InKeys.LKC1 == 0 ? true : IsKeyPressed(InKeys.LKC1)) &&
				(InKeys.LKC2 == 0 ? true : IsKeyPressed(InKeys.LKC2));
		}

		bool LvKeyboardState::ProcessInput(GameInputReading_T* NewReading)
		{
			Assert(NewReading);
			Assert(NewReading->GetInputKind() == this->Type);
			u64 NewReadingTs = NewReading->GetTimestamp();
			if (NewReading == CurrReadingID || NewReadingTs == CurrReadingTs)
			{
				//Outf("LvKeyboardState::ProcessInput - Already at latest input state.\n");
				return false;
			}
			Assert(NewReadingTs > CurrReadingTs);
			bool bResult;
			u32 NewPressedCount = NewReading->GetKeyCount();
			if (NewPressedCount <= LvInput_MaxKeysPressedCount)
			{
				u32 NumActive = NewReading->GetKeyState(LvInput_MaxKeysPressedCount, CurrInput);
				// Manually zero-out non-active key states
				u32 NumKeysZeroed = 0; // For debugging purposes
				for (u32 KeyIdx = NumActive; KeyIdx < CurrPressedCount; KeyIdx++)
				{
					CurrInput[KeyIdx] = {};
					NumKeysZeroed++;
				}
			#if LVINPUT_ENABLE_DEBUG_LOG()
				if (NumActive > 0)
				{
					Outf("LvKeyboardState::ProcessInput - Currently active keys:\n\tVKs: ");
					for (u32 KeyIdx = 0; KeyIdx < NumActive; KeyIdx++)
					{
						Outf("0x%02X ", CurrInput[KeyIdx].virtualKey);
					}
					Outf("\n");
				}
			#endif // LVINPUT_ENABLE_DEBUG_LOG
				CurrReadingID = NewReading;
				CurrReadingTs = NewReadingTs;
				CurrPressedCount = NewPressedCount;
				bResult = true;
			} 
			else
			{
				Outf("LvInput::WARNING - Unable to process new keyboard state.\n");
				bResult = false;
			}
			return bResult;
			
		#if LVINPUT_ENABLE_AGNOSTIC_STATE()
			/*
			LVINPUT_KEYCODE CurrState_Agn[LvInput_MaxKeysPressed];
			*/
		#endif // LVINPUT_ENABLE_AGNOSTIC_STATE
		}

		bool LvMouseState::ProcessInput(GameInputReading_T* NewReading)
		{
			Assert(NewReading);
			Assert(NewReading->GetInputKind() == this->Type);
			u64 NewReadingTs = NewReading->GetTimestamp();
			if (NewReading == CurrReadingID || NewReadingTs == CurrReadingTs)
			{
				//Outf("LvMouseState::ProcessInput - Already at latest input state.\n");
				return false;
			}
			Assert(NewReadingTs > CurrReadingTs);
			bool bResult;
			MouseState_T NewInput{};
			if (NewReading->GetMouseState(&NewInput))
			{
			#if LVINPUT_ENABLE_DEBUG_LOG() && 0
				Outf("LvMouseState::ProcessInput - Current mouse state:\n\t");
				auto GetMBState = [&](GameInputMouseButtons InButton) -> char
				{
					return (NewInput.buttons & InButton) ? '1' : '0';
				};
				Outf("Buttons: L: %c, R: %c, M: %c\n",
					GetMBState(GameInputMouseLeftButton),
					GetMBState(GameInputMouseRightButton),
					GetMBState(GameInputMouseMiddleButton));
				Outf("\tPos: (%d, %d)\n", NewInput.positionX, NewInput.positionY);
				Outf("\tWheel: (%d, %d)\n", NewInput.wheelX, NewInput.wheelY);
			#endif // LVINPUT_ENABLE_DEBUG_LOG
				CurrReadingID = NewReading;
				CurrReadingTs = NewReadingTs;
				CurrInput = NewInput;
				bResult = true;
			}
			else
			{
				Outf("LvInput::WARNING - Unable to process new mouse state.\n");
				bResult = false;
			}
			return bResult;
		}

		bool LvGamepadState::ProcessInput(GameInputReading_T* NewReading)
		{
			Assert(NewReading);
			Assert(NewReading->GetInputKind() == this->Type);
			u64 NewReadingTs = NewReading->GetTimestamp();
			if (NewReading == CurrReadingID || NewReadingTs == CurrReadingTs)
			{
				//Outf("LvGamepadState::ProcessInput - Already at latest input state.\n");
				return false;
			}
			Assert(NewReadingTs > CurrReadingTs);
			bool bResult;
			GamepadState_T NewInput{};
			if (NewReading->GetGamepadState(&NewInput))
			{
			#if LVINPUT_ENABLE_DEBUG_LOG()
				auto GetButtonState = [&](GameInputGamepadButtons InButton) -> char
				{
					return (NewInput.buttons & InButton) ? '1' : '0';
				};
				Outf("LvGamepadState::ProcessInput - Gamepad state:\n");
				Outf("\tMenu: %c\tView: %c\n", GetButtonState(GameInputGamepadMenu), GetButtonState(GameInputGamepadView));
				Outf("\tA: %c, B: %c, X: %c, Y: %c\n", GetButtonState(GameInputGamepadA), GetButtonState(GameInputGamepadB), GetButtonState(GameInputGamepadX), GetButtonState(GameInputGamepadY));
				Outf("\tDpad - U %c, D %c, L %c, R %c\n", GetButtonState(GameInputGamepadDPadUp), GetButtonState(GameInputGamepadDPadDown), GetButtonState(GameInputGamepadDPadLeft), GetButtonState(GameInputGamepadDPadRight));
				Outf("\tLB: %c, RB: %c\n", GetButtonState(GameInputGamepadLeftShoulder), GetButtonState(GameInputGamepadRightShoulder));
				Outf("\tLeftStick: (%.02f, %.02f)    ", NewInput.leftThumbstickX, NewInput.leftThumbstickY);
				Outf("RightStick: (%.02f, %.02f)\n", NewInput.rightThumbstickX, NewInput.rightThumbstickY);
			#endif // LVINPUT_ENABLE_DEBUG_LOG
				CurrReadingID = NewReading;
				CurrReadingTs = NewReadingTs;
				CurrInput = NewInput;
				bResult = true;
			}
			else
			{
				Outf("LvInput::WARNING - Unable to process new gamepad state.\n");
				bResult = false;
			}
			return bResult;
		#if LVINPUT_ENABLE_AGNOSTIC_STATE()
			/*
			CurrState_Agn.Buttons;
			CurrState_Agn.LeftStick;
			CurrState_Agn.RightStick;
			CurrState_Agn.LeftTrigger;
			CurrState_Agn.RightTrigger;
			*/
		#endif // ENABLE_LVINPUT_AGNOSTIC_STATE
		}

		const char* const LVINPUTKEY_ENUMSTR_TABLE[] =
		{
			LV_MAKE_CSTR(LVINPUT_UNDEFINED),
			LV_MAKE_CSTR(LVINPUT_SHIFT),
			LV_MAKE_CSTR(LVINPUT_CTRL),
			LV_MAKE_CSTR(LVINPUT_ALT),
			LV_MAKE_CSTR(LVINPUT_SUPER),
			LV_MAKE_CSTR(LVINPUT_ESC),
			LV_MAKE_CSTR(LVINPUT_TAB),
			LV_MAKE_CSTR(LVINPUT_CAPSLOCK),
			LV_MAKE_CSTR(LVINPUT_BACKSPACE),
			LV_MAKE_CSTR(LVINPUT_ENTER),
			LV_MAKE_CSTR(LVINPUT_SPACE),
			LV_MAKE_CSTR(LVINPUT_PRINTSCREEN),
			LV_MAKE_CSTR(LVINPUT_END),
			LV_MAKE_CSTR(LVINPUT_HOME),
			LV_MAKE_CSTR(LVINPUT_INSERT),
			LV_MAKE_CSTR(LVINPUT_DEL),
			LV_MAKE_CSTR(LVINPUT_PAGEUP),
			LV_MAKE_CSTR(LVINPUT_PAGEDOWN),
			LV_MAKE_CSTR(LVINPUT_LEFT),
			LV_MAKE_CSTR(LVINPUT_UP),
			LV_MAKE_CSTR(LVINPUT_RIGHT),
			LV_MAKE_CSTR(LVINPUT_DOWN),
			LV_MAKE_CSTR(LVINPUT_KEY_0),
			LV_MAKE_CSTR(LVINPUT_KEY_1),
			LV_MAKE_CSTR(LVINPUT_KEY_2),
			LV_MAKE_CSTR(LVINPUT_KEY_3),
			LV_MAKE_CSTR(LVINPUT_KEY_4),
			LV_MAKE_CSTR(LVINPUT_KEY_5),
			LV_MAKE_CSTR(LVINPUT_KEY_6),
			LV_MAKE_CSTR(LVINPUT_KEY_7),
			LV_MAKE_CSTR(LVINPUT_KEY_8),
			LV_MAKE_CSTR(LVINPUT_KEY_9),
			LV_MAKE_CSTR(LVINPUT_KEY_A),
			LV_MAKE_CSTR(LVINPUT_KEY_B),
			LV_MAKE_CSTR(LVINPUT_KEY_C),
			LV_MAKE_CSTR(LVINPUT_KEY_D),
			LV_MAKE_CSTR(LVINPUT_KEY_E),
			LV_MAKE_CSTR(LVINPUT_KEY_F),
			LV_MAKE_CSTR(LVINPUT_KEY_G),
			LV_MAKE_CSTR(LVINPUT_KEY_H),
			LV_MAKE_CSTR(LVINPUT_KEY_I),
			LV_MAKE_CSTR(LVINPUT_KEY_J),
			LV_MAKE_CSTR(LVINPUT_KEY_K),
			LV_MAKE_CSTR(LVINPUT_KEY_L),
			LV_MAKE_CSTR(LVINPUT_KEY_M),
			LV_MAKE_CSTR(LVINPUT_KEY_N),
			LV_MAKE_CSTR(LVINPUT_KEY_O),
			LV_MAKE_CSTR(LVINPUT_KEY_P),
			LV_MAKE_CSTR(LVINPUT_KEY_Q),
			LV_MAKE_CSTR(LVINPUT_KEY_R),
			LV_MAKE_CSTR(LVINPUT_KEY_S),
			LV_MAKE_CSTR(LVINPUT_KEY_T),
			LV_MAKE_CSTR(LVINPUT_KEY_U),
			LV_MAKE_CSTR(LVINPUT_KEY_V),
			LV_MAKE_CSTR(LVINPUT_KEY_W),
			LV_MAKE_CSTR(LVINPUT_KEY_X),
			LV_MAKE_CSTR(LVINPUT_KEY_Y),
			LV_MAKE_CSTR(LVINPUT_KEY_Z),
			LV_MAKE_CSTR(LVINPUT_KEY_BACKTICK),
			LV_MAKE_CSTR(LVINPUT_KEY_MINUS),
			LV_MAKE_CSTR(LVINPUT_KEY_EQUAL),
			LV_MAKE_CSTR(LVINPUT_KEY_LBRACKET),
			LV_MAKE_CSTR(LVINPUT_KEY_RBRACKET),
			LV_MAKE_CSTR(LVINPUT_KEY_BACKSLASH),
			LV_MAKE_CSTR(LVINPUT_KEY_SEMICOLON),
			LV_MAKE_CSTR(LVINPUT_KEY_QUOTE),
			LV_MAKE_CSTR(LVINPUT_KEY_COMMA),
			LV_MAKE_CSTR(LVINPUT_KEY_PERIOD),
			LV_MAKE_CSTR(LVINPUT_KEY_SLASH),
			LV_MAKE_CSTR(LVINPUT_KEY_F1),
			LV_MAKE_CSTR(LVINPUT_KEY_F2),
			LV_MAKE_CSTR(LVINPUT_KEY_F3),
			LV_MAKE_CSTR(LVINPUT_KEY_F4),
			LV_MAKE_CSTR(LVINPUT_KEY_F5),
			LV_MAKE_CSTR(LVINPUT_KEY_F6),
			LV_MAKE_CSTR(LVINPUT_KEY_F7),
			LV_MAKE_CSTR(LVINPUT_KEY_F8),
			LV_MAKE_CSTR(LVINPUT_KEY_F9),
			LV_MAKE_CSTR(LVINPUT_KEY_F10),
			LV_MAKE_CSTR(LVINPUT_KEY_F11),
			LV_MAKE_CSTR(LVINPUT_KEY_F12),
			LV_MAKE_CSTR(LVINPUT_NUMPAD0),
			LV_MAKE_CSTR(LVINPUT_NUMPAD1),
			LV_MAKE_CSTR(LVINPUT_NUMPAD2),
			LV_MAKE_CSTR(LVINPUT_NUMPAD3),
			LV_MAKE_CSTR(LVINPUT_NUMPAD4),
			LV_MAKE_CSTR(LVINPUT_NUMPAD5),
			LV_MAKE_CSTR(LVINPUT_NUMPAD6),
			LV_MAKE_CSTR(LVINPUT_NUMPAD7),
			LV_MAKE_CSTR(LVINPUT_NUMPAD8),
			LV_MAKE_CSTR(LVINPUT_NUMPAD9),
			LV_MAKE_CSTR(LVINPUT_MULTIPLY),
			LV_MAKE_CSTR(LVINPUT_ADD),
			LV_MAKE_CSTR(LVINPUT_SUBTRACT),
			LV_MAKE_CSTR(LVINPUT_DIVIDE),
			LV_MAKE_CSTR(LVINPUT_DECIMAL),
			LV_MAKE_CSTR(LVINPUT_MAX)
		};

		void LvDebug__TestVK2LVINPUT()
		{
			constexpr int VK_MaxValue = 0xFE;
			const char* Invalid_LvInput_Val_CStr = LV_MAKE_CSTR(LVINPUT_INVALID);
			int LvInputKey_NumAssigned[LVINPUT_MAX];
			for (int NumAssgnIdx = 0; NumAssgnIdx < LVINPUT_MAX; NumAssgnIdx++)
			{
				LvInputKey_NumAssigned[NumAssgnIdx] = 0;
			}
			Outf("LVINPUT DEBUG -- VK code to LvInput Keycode table:\n");
			for (uint Curr_VK = 0; Curr_VK <= VK_MaxValue; Curr_VK++)
			{
				LVINPUT_KEYCODE LvInput_ConversionResult = WindowsVK_To_LvInput(Curr_VK);
				if (0 <= LvInput_ConversionResult && LvInput_ConversionResult < LVINPUT_MAX)
				{
					const char* LvInputKey_Cstr = LVINPUTKEY_ENUMSTR_TABLE[LvInput_ConversionResult];
					Outf("\tVK: 0x%02X    ->    %i:%s\n", Curr_VK, (int)LvInput_ConversionResult, LvInputKey_Cstr);
					LvInputKey_NumAssigned[(int)LvInput_ConversionResult]++;
				}
				else
				{
					Outf("\tVK: 0x%02X    ->    ERROR:%i:%s\n", Curr_VK, LvInput_ConversionResult, Invalid_LvInput_Val_CStr);
				}
			}
			bool bUnassignedValuesMsgPrint = false;
			bool bMultipleAssignments = false;
			for (int NumAssgnIdx = LVINPUT_SHIFT; NumAssgnIdx < LVINPUT_MAX; NumAssgnIdx++)
			{
				if (LvInputKey_NumAssigned[NumAssgnIdx] == 0)
				{
					if (!bUnassignedValuesMsgPrint)
					{
						Outf("LVINPUT WARNING -- The following LVINPUT keycode values are unassigned:\n");
						bUnassignedValuesMsgPrint = true;
					}
					Outf("\t%s\n", LVINPUTKEY_ENUMSTR_TABLE[NumAssgnIdx]);
				}
				if (LvInputKey_NumAssigned[NumAssgnIdx] > 1)
				{
					bMultipleAssignments = true;
				}
			}
			if (bMultipleAssignments)
			{
				Outf("LVINPUT WARNING -- The following LVINPUT keycode values have multiple VK codes assigned to them:\n");
				for (int NumAssgnIdx = LVINPUT_SHIFT; NumAssgnIdx < LVINPUT_MAX; NumAssgnIdx++)
				{
					if (LvInputKey_NumAssigned[NumAssgnIdx] > 1)
					{
						Outf("\t%s\n", LVINPUTKEY_ENUMSTR_TABLE[NumAssgnIdx]);
					}
				}
			}
		}
	}
}