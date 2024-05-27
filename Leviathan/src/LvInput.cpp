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
			LV_KEY_INVALID,
			LV_KEY_INVALID, // VK_LBUTTON 	0x01 	Left mouse button
			LV_KEY_INVALID, // VK_RBUTTON 	0x02 	Right mouse button
			LV_KEY_UNDEFINED, // VK_CANCEL 	0x03 	Control - break processing
			LV_KEY_INVALID, // VK_MBUTTON 	0x04 	Middle mouse button
			LV_KEY_UNDEFINED, // VK_XBUTTON1 	0x05 	X1 mouse button
			LV_KEY_UNDEFINED, // VK_XBUTTON2 	0x06 	X2 mouse button
			LV_KEY_RESERVED, // - 0x07 	Reserved
			LV_KEY_BACKSPACE, // VK_BACK 	0x08 	BACKSPACE key
			LV_KEY_TAB, // VK_TAB 	0x09 	TAB key
			LV_KEY_RESERVED, // 0x0A 	Reserved
			LV_KEY_RESERVED, // 0x0B 	Reserved
			LV_KEY_UNDEFINED, // VK_CLEAR 	0x0C 	CLEAR key
			LV_KEY_ENTER, // VK_RETURN 	0x0D 	ENTER key
			 // - 0x0E - 0F 	Unassigned
			LV_KEY_UNDEFINED, // 0x0E
			LV_KEY_UNDEFINED, // 0x0F
			LV_KEY_SHIFT, // VK_SHIFT 	0x10 	SHIFT key
			LV_KEY_CTRL, // VK_CONTROL 	0x11 	CTRL key
			LV_KEY_ALT, // VK_MENU 	0x12 	ALT key
			LV_KEY_UNDEFINED, // VK_PAUSE 	0x13 	PAUSE key
			LV_KEY_CAPSLOCK, // VK_CAPITAL 	0x14 	CAPS LOCK key
			LV_KEY_UNDEFINED, // VK_KANA VK_HANGUL		0x15 	IME Kana mode / IME Hangul mode
			LV_KEY_UNDEFINED, // VK_IME_ON 	0x16 	IME On
			LV_KEY_UNDEFINED, // VK_JUNJA 	0x17 	IME Junja mode
			LV_KEY_UNDEFINED, // VK_FINAL 	0x18 	IME final mode
			LV_KEY_UNDEFINED, // VK_HANJA VK_KANJI		0x19 	IME Hanja mode / IME Kanji mode
			LV_KEY_UNDEFINED, // VK_IME_OFF 	0x1A 	IME Off
			LV_KEY_ESC, // VK_ESCAPE 	0x1B 	ESC key
			LV_KEY_UNDEFINED, // VK_CONVERT 	0x1C 	IME convert
			LV_KEY_UNDEFINED, // VK_NONCONVERT 	0x1D 	IME nonconvert
			LV_KEY_UNDEFINED, // VK_ACCEPT 	0x1E 	IME accept
			LV_KEY_UNDEFINED, // VK_MODECHANGE 	0x1F 	IME mode change request
			LV_KEY_SPACE, // VK_SPACE 	0x20 	SPACEBAR
			LV_KEY_PAGEUP, // VK_PRIOR 	0x21 	PAGE UP key
			LV_KEY_PAGEDOWN, // VK_NEXT 	0x22 	PAGE DOWN key
			LV_KEY_END, // VK_END 	0x23 	END key
			LV_KEY_HOME, // VK_HOME 	0x24 	HOME key
			LV_KEY_LEFT, // VK_LEFT 	0x25 	LEFT ARROW key
			LV_KEY_UP, // VK_UP 	0x26 	UP ARROW key
			LV_KEY_RIGHT, // VK_RIGHT 	0x27 	RIGHT ARROW key
			LV_KEY_DOWN, // VK_DOWN 	0x28 	DOWN ARROW key
			LV_KEY_UNDEFINED, // VK_SELECT 	0x29 	SELECT key
			LV_KEY_UNDEFINED, // VK_PRINT 	0x2A 	PRINT key
			LV_KEY_UNDEFINED, // VK_EXECUTE 	0x2B 	EXECUTE key
			LV_KEY_PRINTSCREEN, // VK_SNAPSHOT 	0x2C 	PRINT SCREEN key
			LV_KEY_INSERT, // VK_INSERT 	0x2D 	INS key
			LV_KEY_DEL, // VK_DELETE 	0x2E 	DEL key
			LV_KEY_UNDEFINED, // VK_HELP 	0x2F 	HELP key
			LV_KEY_0, // 	0x30 	0 key
			LV_KEY_1, // 	0x31 	1 key
			LV_KEY_2, // 	0x32 	2 key
			LV_KEY_3, // 	0x33 	3 key
			LV_KEY_4, // 	0x34 	4 key
			LV_KEY_5, // 	0x35 	5 key
			LV_KEY_6, // 	0x36 	6 key
			LV_KEY_7, // 	0x37 	7 key
			LV_KEY_8, // 	0x38 	8 key
			LV_KEY_9, // 	0x39 	9 key
			// - 0x3A - 40 	Undefined
			LV_KEY_UNDEFINED, // 0x3A
			LV_KEY_UNDEFINED, // 0x3B
			LV_KEY_UNDEFINED, // 0x3C
			LV_KEY_UNDEFINED, // 0x3D
			LV_KEY_UNDEFINED, // 0x3E
			LV_KEY_UNDEFINED, // 0x3F
			LV_KEY_UNDEFINED, // 0x40
			LV_KEY_A, // 	0x41 	A key
			LV_KEY_B, // 	0x42 	B key
			LV_KEY_C, // 	0x43 	C key
			LV_KEY_D, // 	0x44 	D key
			LV_KEY_E, // 	0x45 	E key
			LV_KEY_F, // 	0x46 	F key
			LV_KEY_G, // 	0x47 	G key
			LV_KEY_H, // 	0x48 	H key
			LV_KEY_I, // 	0x49 	I key
			LV_KEY_J, // 	0x4A 	J key
			LV_KEY_K, // 	0x4B 	K key
			LV_KEY_L, // 	0x4C 	L key
			LV_KEY_M, // 	0x4D 	M key
			LV_KEY_N, // 	0x4E 	N key
			LV_KEY_O, // 	0x4F 	O key
			LV_KEY_P, // 	0x50 	P key
			LV_KEY_Q, // 	0x51 	Q key
			LV_KEY_R, // 	0x52 	R key
			LV_KEY_S, // 	0x53 	S key
			LV_KEY_T, // 	0x54 	T key
			LV_KEY_U, // 	0x55 	U key
			LV_KEY_V, // 	0x56 	V key
			LV_KEY_W, // 	0x57 	W key
			LV_KEY_X, // 	0x58 	X key
			LV_KEY_Y, // 	0x59 	Y key
			LV_KEY_Z, // 	0x5A 	Z key
			LV_KEY_SUPER, // VK_LWIN 	0x5B 	Left Windows key
			LV_KEY_SUPER, // VK_RWIN 	0x5C 	Right Windows key
			LV_KEY_UNDEFINED, // VK_APPS 	0x5D 	Applications key
			LV_KEY_RESERVED, // - 0x5E 	Reserved
			LV_KEY_UNDEFINED, // VK_SLEEP 	0x5F 	Computer Sleep key
			LV_KEY_NUMPAD0, // VK_NUMPAD0 	0x60 	Numeric keypad 0 key
			LV_KEY_NUMPAD1, // VK_NUMPAD1 	0x61 	Numeric keypad 1 key
			LV_KEY_NUMPAD2, // VK_NUMPAD2 	0x62 	Numeric keypad 2 key
			LV_KEY_NUMPAD3, // VK_NUMPAD3 	0x63 	Numeric keypad 3 key
			LV_KEY_NUMPAD4, // VK_NUMPAD4 	0x64 	Numeric keypad 4 key
			LV_KEY_NUMPAD5, // VK_NUMPAD5 	0x65 	Numeric keypad 5 key
			LV_KEY_NUMPAD6, // VK_NUMPAD6 	0x66 	Numeric keypad 6 key
			LV_KEY_NUMPAD7, // VK_NUMPAD7 	0x67 	Numeric keypad 7 key
			LV_KEY_NUMPAD8, // VK_NUMPAD8 	0x68 	Numeric keypad 8 key
			LV_KEY_NUMPAD9, // VK_NUMPAD9 	0x69 	Numeric keypad 9 key
			LV_KEY_MULTIPLY, // VK_MULTIPLY 	0x6A 	Multiply key
			LV_KEY_ADD, // VK_ADD 	0x6B 	Add key
			LV_KEY_UNDEFINED, // VK_SEPARATOR 	0x6C 	Separator key
			LV_KEY_SUBTRACT, // VK_SUBTRACT 	0x6D 	Subtract key
			LV_KEY_DECIMAL, // VK_DECIMAL 	0x6E 	Decimal key
			LV_KEY_DIVIDE, // VK_DIVIDE 	0x6F 	Divide key
			LV_KEY_F1, // VK_F1 	0x70 	F1 key
			LV_KEY_F2, // VK_F2 	0x71 	F2 key
			LV_KEY_F3, // VK_F3 	0x72 	F3 key
			LV_KEY_F4, // VK_F4 	0x73 	F4 key
			LV_KEY_F5, // VK_F5 	0x74 	F5 key
			LV_KEY_F6, // VK_F6 	0x75 	F6 key
			LV_KEY_F7, // VK_F7 	0x76 	F7 key
			LV_KEY_F8, // VK_F8 	0x77 	F8 key
			LV_KEY_F9, // VK_F9 	0x78 	F9 key
			LV_KEY_F10, // VK_F10 	0x79 	F10 key
			LV_KEY_F11, // VK_F11 	0x7A 	F11 key
			LV_KEY_F12, // VK_F12 	0x7B 	F12 key
			LV_KEY_UNDEFINED, // VK_F13 	0x7C 	F13 key
			LV_KEY_UNDEFINED, // VK_F14 	0x7D 	F14 key
			LV_KEY_UNDEFINED, // VK_F15 	0x7E 	F15 key
			LV_KEY_UNDEFINED, // VK_F16 	0x7F 	F16 key
			LV_KEY_UNDEFINED, // VK_F17 	0x80 	F17 key
			LV_KEY_UNDEFINED, // VK_F18 	0x81 	F18 key
			LV_KEY_UNDEFINED, // VK_F19 	0x82 	F19 key
			LV_KEY_UNDEFINED, // VK_F20 	0x83 	F20 key
			LV_KEY_UNDEFINED, // VK_F21 	0x84 	F21 key
			LV_KEY_UNDEFINED, // VK_F22 	0x85 	F22 key
			LV_KEY_UNDEFINED, // VK_F23 	0x86 	F23 key
			LV_KEY_UNDEFINED, // VK_F24 	0x87 	F24 key
			// - 0x88 - 8F 	Reserved
			LV_KEY_RESERVED,  // 0x88
			LV_KEY_RESERVED,  // 0x89
			LV_KEY_RESERVED,  // 0x8A
			LV_KEY_RESERVED,  // 0x8B
			LV_KEY_RESERVED,  // 0x8C
			LV_KEY_RESERVED,  // 0x8D
			LV_KEY_RESERVED,  // 0x8E
			LV_KEY_RESERVED,  // 0x8F
			LV_KEY_UNDEFINED, // VK_NUMLOCK 	0x90 	NUM LOCK key
			LV_KEY_UNDEFINED, // VK_SCROLL 	0x91 	SCROLL LOCK key
			// - 0x92 - 96 	OEM specific
			LV_KEY_UNDEFINED, // 0x92
			LV_KEY_UNDEFINED, // 0x93
			LV_KEY_UNDEFINED, // 0x94
			LV_KEY_UNDEFINED, // 0x95
			LV_KEY_UNDEFINED, // 0x96
			// - 0x97 - 9F 	Unassigned
			LV_KEY_UNDEFINED, // 0x97
			LV_KEY_UNDEFINED, // 0x98
			LV_KEY_UNDEFINED, // 0x99
			LV_KEY_UNDEFINED, // 0x9A
			LV_KEY_UNDEFINED, // 0x9B
			LV_KEY_UNDEFINED, // 0x9C
			LV_KEY_UNDEFINED, // 0x9D
			LV_KEY_UNDEFINED, // 0x9E
			LV_KEY_UNDEFINED, // 0x9F
			LV_KEY_SHIFT, // VK_LSHIFT 	0xA0 	Left SHIFT key
			LV_KEY_SHIFT, // VK_RSHIFT 	0xA1 	Right SHIFT key
			LV_KEY_CTRL, // VK_LCONTROL 	0xA2 	Left CONTROL key
			LV_KEY_CTRL, // VK_RCONTROL 	0xA3 	Right CONTROL key
			LV_KEY_ALT, // VK_LMENU 	0xA4 	Left ALT key
			LV_KEY_ALT, // VK_RMENU 	0xA5 	Right ALT key
			LV_KEY_UNDEFINED, // VK_BROWSER_BACK 	0xA6 	Browser Back key
			LV_KEY_UNDEFINED, // VK_BROWSER_FORWARD 	0xA7 	Browser Forward key
			LV_KEY_UNDEFINED, // VK_BROWSER_REFRESH 	0xA8 	Browser Refresh key
			LV_KEY_UNDEFINED, // VK_BROWSER_STOP 	0xA9 	Browser Stop key
			LV_KEY_UNDEFINED, // VK_BROWSER_SEARCH 	0xAA 	Browser Search key
			LV_KEY_UNDEFINED, // VK_BROWSER_FAVORITES 	0xAB 	Browser Favorites key
			LV_KEY_UNDEFINED, // VK_BROWSER_HOME 	0xAC 	Browser Start and Home key
			LV_KEY_UNDEFINED, // VK_VOLUME_MUTE 	0xAD 	Volume Mute key
			LV_KEY_UNDEFINED, // VK_VOLUME_DOWN 	0xAE 	Volume Down key
			LV_KEY_UNDEFINED, // VK_VOLUME_UP 	0xAF 	Volume Up key
			LV_KEY_UNDEFINED, // VK_MEDIA_NEXT_TRACK 	0xB0 	Next Track key
			LV_KEY_UNDEFINED, // VK_MEDIA_PREV_TRACK 	0xB1 	Previous Track key
			LV_KEY_UNDEFINED, // VK_MEDIA_STOP 	0xB2 	Stop Media key
			LV_KEY_UNDEFINED, // VK_MEDIA_PLAY_PAUSE 	0xB3 	Play / Pause Media key
			LV_KEY_UNDEFINED, // VK_LAUNCH_MAIL 	0xB4 	Start Mail key
			LV_KEY_UNDEFINED, // VK_LAUNCH_MEDIA_SELECT 	0xB5 	Select Media key
			LV_KEY_UNDEFINED, // VK_LAUNCH_APP1 	0xB6 	Start Application 1 key
			LV_KEY_UNDEFINED, // VK_LAUNCH_APP2 	0xB7 	Start Application 2 key
			// - 0xB8 - B9 	Reserved
			LV_KEY_RESERVED, // 0xB8
			LV_KEY_RESERVED, // 0xB9
			LV_KEY_SEMICOLON, // VK_OEM_1 	0xBA 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the; : key
			LV_KEY_EQUAL, // VK_OEM_PLUS 	0xBB 	For any country / region, the + key
			LV_KEY_COMMA, // VK_OEM_COMMA 	0xBC 	For any country / region, the , key
			LV_KEY_MINUS, // VK_OEM_MINUS 	0xBD 	For any country / region, the - key
			LV_KEY_PERIOD, // VK_OEM_PERIOD 	0xBE 	For any country / region, the.key
			LV_KEY_SLASH, // VK_OEM_2 	0xBF 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the / ? key
			LV_KEY_BACKTICK, // VK_OEM_3 	0xC0 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the `~ key
			// - 0xC1 - DA 	Reserved
			LV_KEY_RESERVED, // 0xC1
			LV_KEY_RESERVED, // 0xC2
			LV_KEY_RESERVED, // 0xC3
			LV_KEY_RESERVED, // 0xC4
			LV_KEY_RESERVED, // 0xC5
			LV_KEY_RESERVED, // 0xC6
			LV_KEY_RESERVED, // 0xC7
			LV_KEY_RESERVED, // 0xC8
			LV_KEY_RESERVED, // 0xC9
			LV_KEY_RESERVED, // 0xCA
			LV_KEY_RESERVED, // 0xCB
			LV_KEY_RESERVED, // 0xCC
			LV_KEY_RESERVED, // 0xCD
			LV_KEY_RESERVED, // 0xCE
			LV_KEY_RESERVED, // 0xCF
			LV_KEY_RESERVED, // 0xD0
			LV_KEY_RESERVED, // 0xD1
			LV_KEY_RESERVED, // 0xD2
			LV_KEY_RESERVED, // 0xD3
			LV_KEY_RESERVED, // 0xD4
			LV_KEY_RESERVED, // 0xD5
			LV_KEY_RESERVED, // 0xD6
			LV_KEY_RESERVED, // 0xD7
			LV_KEY_RESERVED, // 0xD8
			LV_KEY_RESERVED, // 0xD9
			LV_KEY_RESERVED, // 0xDA
			LV_KEY_LBRACKET, // VK_OEM_4 	0xDB 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the[{ key
			LV_KEY_BACKSLASH, // VK_OEM_5 	0xDC 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the \\ | key
			LV_KEY_RBRACKET, // VK_OEM_6 	0xDD 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the]} key
			LV_KEY_QUOTE, // VK_OEM_7 	0xDE 	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the '" key
			LV_KEY_UNDEFINED, // VK_OEM_8 	0xDF 	Used for miscellaneous characters; it can vary by keyboard.
			LV_KEY_RESERVED, // - 0xE0 	Reserved
			LV_KEY_UNDEFINED, // - 0xE1 	OEM specific
			LV_KEY_UNDEFINED, // VK_OEM_102 	0xE2 	The <> keys on the US standard keyboard, or the \\ | key on the non - US 102 - key keyboard
			// - 0xE3 - E4 	OEM specific
			LV_KEY_UNDEFINED, // 0xE3
			LV_KEY_UNDEFINED, // 0xE4
			LV_KEY_UNDEFINED, // VK_PROCESSKEY 	0xE5 	IME PROCESS key
			LV_KEY_UNDEFINED, // - 0xE6 	OEM specific
			LV_KEY_UNDEFINED, // VK_PACKET 	0xE7 	Used to pass Unicode characters as if they were keystrokes.The VK_PACKET key is the low word of a 32 - bit Virtual Key value used for non - keyboard input methods.For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
			LV_KEY_UNDEFINED, // - 0xE8 	Unassigned
			// - 0xE9 - F5 	OEM specific
			LV_KEY_UNDEFINED, // 0xE9
			LV_KEY_UNDEFINED, // 0xEA
			LV_KEY_UNDEFINED, // 0xEB
			LV_KEY_UNDEFINED, // 0xEC
			LV_KEY_UNDEFINED, // 0xED
			LV_KEY_UNDEFINED, // 0xEE
			LV_KEY_UNDEFINED, // 0xEF
			LV_KEY_UNDEFINED, // 0xF0
			LV_KEY_UNDEFINED, // 0xF1
			LV_KEY_UNDEFINED, // 0xF2
			LV_KEY_UNDEFINED, // 0xF3
			LV_KEY_UNDEFINED, // 0xF4
			LV_KEY_UNDEFINED, // 0xF5
			LV_KEY_UNDEFINED, // VK_ATTN 	0xF6 	Attn key
			LV_KEY_UNDEFINED, // VK_CRSEL 	0xF7 	CrSel key
			LV_KEY_UNDEFINED, // VK_EXSEL 	0xF8 	ExSel key
			LV_KEY_UNDEFINED, // VK_EREOF 	0xF9 	Erase EOF key
			LV_KEY_UNDEFINED, // VK_PLAY 	0xFA 	Play key
			LV_KEY_UNDEFINED, // VK_ZOOM 	0xFB 	Zoom key
			LV_KEY_RESERVED, // VK_NONAME 	0xFC 	Reserved
			LV_KEY_UNDEFINED, // VK_PA1 	0xFD 	PA1 key
			LV_KEY_UNDEFINED // VK_OEM_CLEAR 	0xFE 	Clear key
		};
		constexpr unsigned int VK2LVINPUT_TABLE_SIZE = LV_ARRAYSIZE(VK_TO_LVINPUT);

		LVINPUT_KEYCODE WindowsVK_To_LvInput(unsigned int VirtualKey)
		{
			LVINPUT_KEYCODE Result = LV_KEY_INVALID;
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

		bool LvKeyboardState::AnyKeysPressed(const LVINPUT_KEYCODE* InLvKeys, u32 NumKeys)
		{
			for (u32 KeyIdx = 0; KeyIdx < NumKeys; KeyIdx++)
			{
				if (IsKeyPressed(InLvKeys[KeyIdx])) { return true; }
			}
			return false;
		}

		bool LvKeyboardState::AnyCombosPressed(const LvKeyInputCombo* InCombos, u32 NumCombos)
		{
			for (u32 ComboIdx = 0; ComboIdx < NumCombos; ComboIdx++)
			{
				if (IsKeyComboPressed(InCombos[ComboIdx])) { return true; }
			}
			return false;
		}

		bool LvKeyboardState::ProcessInput(GameInputReading_T* NewReading)
		{
			Assert(NewReading);
			Assert(NewReading->GetInputKind() == this->Type);
			u64 NewReadingTs = NewReading->GetTimestamp();
			if (NewReading == CurrReadingID || NewReadingTs <= CurrReadingTs)
			{
				//Outf("LvKeyboardState::ProcessInput - Already at latest input state.\n");
				return false;
			}
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
			if (NewReading == CurrReadingID || NewReadingTs <= CurrReadingTs)
			{
				//Outf("LvMouseState::ProcessInput - Already at latest input state.\n");
				return false;
			}
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
			if (NewReading == CurrReadingID || NewReadingTs <= CurrReadingTs)
			{
				//Outf("LvGamepadState::ProcessInput - Already at latest input state.\n");
				return false;
			}
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
			LV_MAKE_CSTR(LV_KEY_UNDEFINED),
			LV_MAKE_CSTR(LV_KEY_SHIFT),
			LV_MAKE_CSTR(LV_KEY_CTRL),
			LV_MAKE_CSTR(LV_KEY_ALT),
			LV_MAKE_CSTR(LV_KEY_SUPER),
			LV_MAKE_CSTR(LV_KEY_ESC),
			LV_MAKE_CSTR(LV_KEY_TAB),
			LV_MAKE_CSTR(LV_KEY_CAPSLOCK),
			LV_MAKE_CSTR(LV_KEY_BACKSPACE),
			LV_MAKE_CSTR(LV_KEY_ENTER),
			LV_MAKE_CSTR(LV_KEY_SPACE),
			LV_MAKE_CSTR(LV_KEY_PRINTSCREEN),
			LV_MAKE_CSTR(LV_KEY_END),
			LV_MAKE_CSTR(LV_KEY_HOME),
			LV_MAKE_CSTR(LV_KEY_INSERT),
			LV_MAKE_CSTR(LV_KEY_DEL),
			LV_MAKE_CSTR(LV_KEY_PAGEUP),
			LV_MAKE_CSTR(LV_KEY_PAGEDOWN),
			LV_MAKE_CSTR(LV_KEY_LEFT),
			LV_MAKE_CSTR(LV_KEY_UP),
			LV_MAKE_CSTR(LV_KEY_RIGHT),
			LV_MAKE_CSTR(LV_KEY_DOWN),
			LV_MAKE_CSTR(LV_KEY_0),
			LV_MAKE_CSTR(LV_KEY_1),
			LV_MAKE_CSTR(LV_KEY_2),
			LV_MAKE_CSTR(LV_KEY_3),
			LV_MAKE_CSTR(LV_KEY_4),
			LV_MAKE_CSTR(LV_KEY_5),
			LV_MAKE_CSTR(LV_KEY_6),
			LV_MAKE_CSTR(LV_KEY_7),
			LV_MAKE_CSTR(LV_KEY_8),
			LV_MAKE_CSTR(LV_KEY_9),
			LV_MAKE_CSTR(LV_KEY_A),
			LV_MAKE_CSTR(LV_KEY_B),
			LV_MAKE_CSTR(LV_KEY_C),
			LV_MAKE_CSTR(LV_KEY_D),
			LV_MAKE_CSTR(LV_KEY_E),
			LV_MAKE_CSTR(LV_KEY_F),
			LV_MAKE_CSTR(LV_KEY_G),
			LV_MAKE_CSTR(LV_KEY_H),
			LV_MAKE_CSTR(LV_KEY_I),
			LV_MAKE_CSTR(LV_KEY_J),
			LV_MAKE_CSTR(LV_KEY_K),
			LV_MAKE_CSTR(LV_KEY_L),
			LV_MAKE_CSTR(LV_KEY_M),
			LV_MAKE_CSTR(LV_KEY_N),
			LV_MAKE_CSTR(LV_KEY_O),
			LV_MAKE_CSTR(LV_KEY_P),
			LV_MAKE_CSTR(LV_KEY_Q),
			LV_MAKE_CSTR(LV_KEY_R),
			LV_MAKE_CSTR(LV_KEY_S),
			LV_MAKE_CSTR(LV_KEY_T),
			LV_MAKE_CSTR(LV_KEY_U),
			LV_MAKE_CSTR(LV_KEY_V),
			LV_MAKE_CSTR(LV_KEY_W),
			LV_MAKE_CSTR(LV_KEY_X),
			LV_MAKE_CSTR(LV_KEY_Y),
			LV_MAKE_CSTR(LV_KEY_Z),
			LV_MAKE_CSTR(LV_KEY_BACKTICK),
			LV_MAKE_CSTR(LV_KEY_MINUS),
			LV_MAKE_CSTR(LV_KEY_EQUAL),
			LV_MAKE_CSTR(LV_KEY_LBRACKET),
			LV_MAKE_CSTR(LV_KEY_RBRACKET),
			LV_MAKE_CSTR(LV_KEY_BACKSLASH),
			LV_MAKE_CSTR(LV_KEY_SEMICOLON),
			LV_MAKE_CSTR(LV_KEY_QUOTE),
			LV_MAKE_CSTR(LV_KEY_COMMA),
			LV_MAKE_CSTR(LV_KEY_PERIOD),
			LV_MAKE_CSTR(LV_KEY_SLASH),
			LV_MAKE_CSTR(LV_KEY_F1),
			LV_MAKE_CSTR(LV_KEY_F2),
			LV_MAKE_CSTR(LV_KEY_F3),
			LV_MAKE_CSTR(LV_KEY_F4),
			LV_MAKE_CSTR(LV_KEY_F5),
			LV_MAKE_CSTR(LV_KEY_F6),
			LV_MAKE_CSTR(LV_KEY_F7),
			LV_MAKE_CSTR(LV_KEY_F8),
			LV_MAKE_CSTR(LV_KEY_F9),
			LV_MAKE_CSTR(LV_KEY_F10),
			LV_MAKE_CSTR(LV_KEY_F11),
			LV_MAKE_CSTR(LV_KEY_F12),
			LV_MAKE_CSTR(LV_KEY_NUMPAD0),
			LV_MAKE_CSTR(LV_KEY_NUMPAD1),
			LV_MAKE_CSTR(LV_KEY_NUMPAD2),
			LV_MAKE_CSTR(LV_KEY_NUMPAD3),
			LV_MAKE_CSTR(LV_KEY_NUMPAD4),
			LV_MAKE_CSTR(LV_KEY_NUMPAD5),
			LV_MAKE_CSTR(LV_KEY_NUMPAD6),
			LV_MAKE_CSTR(LV_KEY_NUMPAD7),
			LV_MAKE_CSTR(LV_KEY_NUMPAD8),
			LV_MAKE_CSTR(LV_KEY_NUMPAD9),
			LV_MAKE_CSTR(LV_KEY_MULTIPLY),
			LV_MAKE_CSTR(LV_KEY_ADD),
			LV_MAKE_CSTR(LV_KEY_SUBTRACT),
			LV_MAKE_CSTR(LV_KEY_DIVIDE),
			LV_MAKE_CSTR(LV_KEY_DECIMAL),
			LV_MAKE_CSTR(LV_KEY_MAX)
		};

		void LvDebug__TestVK2LVINPUT()
		{
			constexpr int VK_MaxValue = 0xFE;
			const char* Invalid_LvInput_Val_CStr = LV_MAKE_CSTR(LV_KEY_INVALID);
			int LvInputKey_NumAssigned[LV_KEY_MAX];
			for (int NumAssgnIdx = 0; NumAssgnIdx < LV_KEY_MAX; NumAssgnIdx++)
			{
				LvInputKey_NumAssigned[NumAssgnIdx] = 0;
			}
			Outf("LVINPUT DEBUG -- VK code to LvInput Keycode table:\n");
			for (uint Curr_VK = 0; Curr_VK <= VK_MaxValue; Curr_VK++)
			{
				LVINPUT_KEYCODE LvInput_ConversionResult = WindowsVK_To_LvInput(Curr_VK);
				if (0 <= LvInput_ConversionResult && LvInput_ConversionResult < LV_KEY_MAX)
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
			for (int NumAssgnIdx = LV_KEY_SHIFT; NumAssgnIdx < LV_KEY_MAX; NumAssgnIdx++)
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
				for (int NumAssgnIdx = LV_KEY_SHIFT; NumAssgnIdx < LV_KEY_MAX; NumAssgnIdx++)
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