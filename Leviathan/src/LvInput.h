#ifndef LVINPUT_H
#define LVINPUT_H

namespace Leviathan
{

#define LVINPUT_ENABLE_DEBUG_LOG() (1)

	inline namespace LvInput
	{
		enum LVINPUT_KEYCODE
		{
			LV_KEY_INVALID = -1,
			LV_KEY_UNDEFINED = 0,
			LV_KEY_UNUSED = 0,
			LV_KEY_RESERVED = 0,
			// Modifiers
			LV_KEY_SHIFT,
			LV_KEY_CTRL,
			LV_KEY_ALT,
			LV_KEY_SUPER,
			// Control characters
			LV_KEY_ESC,
			LV_KEY_TAB,
			LV_KEY_CAPSLOCK,
			LV_KEY_BACKSPACE,
			LV_KEY_ENTER,
			LV_KEY_SPACE,
			LV_KEY_PRINTSCREEN,
			// Page navigation
			LV_KEY_END,
			LV_KEY_HOME,
			LV_KEY_INSERT,
			LV_KEY_DEL,
			LV_KEY_PAGEUP,
			LV_KEY_PAGEDOWN,
			// Arrow keys
			LV_KEY_LEFT,
			LV_KEY_UP,
			LV_KEY_RIGHT,
			LV_KEY_DOWN,
			// Number row:
			LV_KEY_0, // 	0 key
			LV_KEY_1, // 	1 key
			LV_KEY_2, // 	2 key
			LV_KEY_3, // 	3 key
			LV_KEY_4, // 	4 key
			LV_KEY_5, // 	5 key
			LV_KEY_6, // 	6 key
			LV_KEY_7, // 	7 key
			LV_KEY_8, // 	8 key
			LV_KEY_9, // 	9 key
			// A-Z character keys:
			LV_KEY_A,
			LV_KEY_B,
			LV_KEY_C,
			LV_KEY_D,
			LV_KEY_E,
			LV_KEY_F,
			LV_KEY_G,
			LV_KEY_H,
			LV_KEY_I,
			LV_KEY_J,
			LV_KEY_K,
			LV_KEY_L,
			LV_KEY_M,
			LV_KEY_N,
			LV_KEY_O,
			LV_KEY_P,
			LV_KEY_Q,
			LV_KEY_R,
			LV_KEY_S,
			LV_KEY_T,
			LV_KEY_U,
			LV_KEY_V,
			LV_KEY_W,
			LV_KEY_X,
			LV_KEY_Y,
			LV_KEY_Z,
			// Punctuation
			LV_KEY_BACKTICK,
			LV_KEY_MINUS,
			LV_KEY_EQUAL,
			LV_KEY_LBRACKET,
			LV_KEY_RBRACKET,
			LV_KEY_BACKSLASH,
			LV_KEY_SEMICOLON,
			LV_KEY_QUOTE,
			LV_KEY_COMMA,
			LV_KEY_PERIOD,
			LV_KEY_SLASH,
			// Function keys
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
			// Numpad
			LV_KEY_NUMPAD0,
			LV_KEY_NUMPAD1,
			LV_KEY_NUMPAD2,
			LV_KEY_NUMPAD3,
			LV_KEY_NUMPAD4,
			LV_KEY_NUMPAD5,
			LV_KEY_NUMPAD6,
			LV_KEY_NUMPAD7,
			LV_KEY_NUMPAD8,
			LV_KEY_NUMPAD9,
			LV_KEY_MULTIPLY,
			LV_KEY_ADD,
			LV_KEY_SUBTRACT,
			LV_KEY_DIVIDE,
			LV_KEY_DECIMAL,
			// Enum max/count/num
			LV_KEY_MAX
		};

		LVINPUT_KEYCODE WindowsVK_To_LvInput(unsigned int VirtualKey);

		using ButtonState_T = bool;
		using KeyState_T = GameInputKeyState;
		using MouseState_T = GameInputMouseState;
		using GamepadState_T = GameInputGamepadState;
		using GameInputReading_T = IGameInputReading;

		struct LvKeyInputCombo
		{
			LVINPUT_KEYCODE LKC0 = LV_KEY_UNUSED;
			LVINPUT_KEYCODE LKC1 = LV_KEY_UNUSED;
			LVINPUT_KEYCODE LKC2 = LV_KEY_UNUSED;
		};

		// Do not support more than 16 keys pressed at same time
		static constexpr int LvInput_MaxKeysPressedCount = 16;
		struct LvKeyboardState
		{
			static constexpr GameInputKind Type = GameInputKindKeyboard;
			GameInputReading_T* CurrReadingID = nullptr;
			u64 CurrReadingTs = 0;
			KeyState_T CurrInput[LvInput_MaxKeysPressedCount];
			u32 CurrPressedCount = 0;
		#if LVINPUT_ENABLE_AGNOSTIC_STATE()
			LVINPUT_KEYCODE CurrState_Agn[LvInput_MaxKeysPressed];
		#endif // LVINPUT_ENABLE_AGNOSTIC_STATE
			bool IsKeyPressed(u32 InVK);
			bool IsKeyPressed(LVINPUT_KEYCODE InLKC);
			bool IsKeyComboPressed(LvKeyInputCombo InKeys);
			bool AnyKeysPressed(const LVINPUT_KEYCODE* InLvKeys, u32 NumKeys);
			bool AnyCombosPressed(const LvKeyInputCombo* InCombos, u32 NumCombos);

			bool ProcessInput(GameInputReading_T* NewReading);
		};

		enum LVINPUT_MOUSE_BUTTON
		{
			LVINPUT_MOUSE_INVALID = -1,
			LVINPUT_MOUSE_LEFT = 0,
			LVINPUT_MOUSE_RIGHT,
			LVINPUT_MOUSE_MIDDLE,
			LVINPUT_MOUSE_WHEEL,
			//LVINPUT_MOUSE_BACK,
			//LVINPUT_MOUSE_NEXT
			LVINPUT_MOUSE_BUTTON_MAX
		};

		struct LvMouseState
		{
			static constexpr GameInputKind Type = GameInputKindMouse;
			GameInputReading_T* CurrReadingID = nullptr;
			u64 CurrReadingTs = 0;
			MouseState_T CurrInput;
		#if LVINPUT_ENABLE_AGNOSTIC_STATE()
			struct
			{
				ButtonState_T Buttons[LVINPUT_MOUSE_BUTTON_MAX];

				int VScroll = 0;
				int HScroll = 0;

				i2D CurrPos;
				i2D PrevPos;
				i2D PosDelta; // Curr - Prev
			} CurrState_Agn;
		#endif // LVINPUT_ENABLE_AGNOSTIC_STATE

			bool ProcessInput(GameInputReading_T* NewReading);
		};

		enum LVINPUT_GAMEPAD_BUTTON
		{
			LVINPUT_GAMEPAD_INVALID = -1,
			// Dpad
			LVINPUT_GAMEPAD_DPAD_LEFT = 0,
			LVINPUT_GAMEPAD_DPAD_RIGHT,
			LVINPUT_GAMEPAD_DPAD_UP,
			LVINPUT_GAMEPAD_DPAD_DOWN,
			// Face buttons: use direction here b/c Playstation, Xbox, Nintendo, etc.
			LVINPUT_GAMEPAD_FACE_LEFT,
			LVINPUT_GAMEPAD_FACE_RIGHT,
			LVINPUT_GAMEPAD_FACE_UP,
			LVINPUT_GAMEPAD_FACE_DOWN,
			// Shoulder buttons
			LVINPUT_GAMEPAD_LSHOULDER,
			LVINPUT_GAMEPAD_RSHOULDER,
			LVINPUT_GAMEPAD_LTRIGGER,
			LVINPUT_GAMEPAD_RTRIGGER,
			// Stick buttons
			LVINPUT_GAMEPAD_LSTICK,
			LVINPUT_GAMEPAD_RSTICK,
			// General interface (legacy: start/select)
			LVINPUT_GAMEPAD_MENU, // Main 'Start'/'Options'/etc button
			LVINPUT_GAMEPAD_MORE, // The 'Share' button in PS5/XSX
			LVINPUT_GAMEPAD_HOME, // General platform 'Logo' button
			//
			LVINPUT_GAMEPAD_MAX
		};

		struct LvGamepadState
		{
			static constexpr GameInputKind Type = GameInputKindGamepad;
			GameInputReading_T* CurrReadingID = nullptr;
			u64 CurrReadingTs = 0;
			GamepadState_T CurrInput{};

		#if LVINPUT_ENABLE_AGNOSTIC_STATE()
			struct
			{
				ButtonState_T Buttons[LVINPUT_GAMEPAD_MAX];
				f2D LeftStick;
				f2D RightSick;
				float LeftTrigger = 0.0f;
				float RightTrigger = 0.0f;
			} CurrState_Agn;
		#endif // ENABLE_LVINPUT_AGNOSTIC_STATE

			bool ProcessInput(GameInputReading_T* NewReading);
		};

		void LvDebug__TestVK2LVINPUT();
	}
}

#endif // LVINPUT_H