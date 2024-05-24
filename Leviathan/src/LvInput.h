#ifndef LVINPUT_H
#define LVINPUT_H

namespace Leviathan
{

#define LVINPUT_ENABLE_DEBUG_LOG() (1)

	inline namespace LvInput
	{
		enum LVINPUT_KEYCODE
		{
			LVINPUT_INVALID = -1,
			LVINPUT_UNDEFINED = 0,
			LVINPUT_UNUSED = 0,
			LVINPUT_RESERVED = 0,
			// Modifiers
			LVINPUT_SHIFT,
			LVINPUT_CTRL,
			LVINPUT_ALT,
			LVINPUT_SUPER,
			// Control characters
			LVINPUT_ESC,
			LVINPUT_TAB,
			LVINPUT_CAPSLOCK,
			LVINPUT_BACKSPACE,
			LVINPUT_ENTER,
			LVINPUT_SPACE,
			LVINPUT_PRINTSCREEN,
			// Page navigation
			LVINPUT_END,
			LVINPUT_HOME,
			LVINPUT_INSERT,
			LVINPUT_DEL,
			LVINPUT_PAGEUP,
			LVINPUT_PAGEDOWN,
			// Arrow keys
			LVINPUT_LEFT,
			LVINPUT_UP,
			LVINPUT_RIGHT,
			LVINPUT_DOWN,
			// Number row:
			LVINPUT_KEY_0, // 	0 key
			LVINPUT_KEY_1, // 	1 key
			LVINPUT_KEY_2, // 	2 key
			LVINPUT_KEY_3, // 	3 key
			LVINPUT_KEY_4, // 	4 key
			LVINPUT_KEY_5, // 	5 key
			LVINPUT_KEY_6, // 	6 key
			LVINPUT_KEY_7, // 	7 key
			LVINPUT_KEY_8, // 	8 key
			LVINPUT_KEY_9, // 	9 key
			// A-Z character keys:
			LVINPUT_KEY_A,
			LVINPUT_KEY_B,
			LVINPUT_KEY_C,
			LVINPUT_KEY_D,
			LVINPUT_KEY_E,
			LVINPUT_KEY_F,
			LVINPUT_KEY_G,
			LVINPUT_KEY_H,
			LVINPUT_KEY_I,
			LVINPUT_KEY_J,
			LVINPUT_KEY_K,
			LVINPUT_KEY_L,
			LVINPUT_KEY_M,
			LVINPUT_KEY_N,
			LVINPUT_KEY_O,
			LVINPUT_KEY_P,
			LVINPUT_KEY_Q,
			LVINPUT_KEY_R,
			LVINPUT_KEY_S,
			LVINPUT_KEY_T,
			LVINPUT_KEY_U,
			LVINPUT_KEY_V,
			LVINPUT_KEY_W,
			LVINPUT_KEY_X,
			LVINPUT_KEY_Y,
			LVINPUT_KEY_Z,
			// Punctuation
			LVINPUT_KEY_BACKTICK,
			LVINPUT_KEY_MINUS,
			LVINPUT_KEY_EQUAL,
			LVINPUT_KEY_LBRACKET,
			LVINPUT_KEY_RBRACKET,
			LVINPUT_KEY_BACKSLASH,
			LVINPUT_KEY_SEMICOLON,
			LVINPUT_KEY_QUOTE,
			LVINPUT_KEY_COMMA,
			LVINPUT_KEY_PERIOD,
			LVINPUT_KEY_SLASH,
			// Function keys
			LVINPUT_KEY_F1,
			LVINPUT_KEY_F2,
			LVINPUT_KEY_F3,
			LVINPUT_KEY_F4,
			LVINPUT_KEY_F5,
			LVINPUT_KEY_F6,
			LVINPUT_KEY_F7,
			LVINPUT_KEY_F8,
			LVINPUT_KEY_F9,
			LVINPUT_KEY_F10,
			LVINPUT_KEY_F11,
			LVINPUT_KEY_F12,
			// Numpad
			LVINPUT_NUMPAD0,
			LVINPUT_NUMPAD1,
			LVINPUT_NUMPAD2,
			LVINPUT_NUMPAD3,
			LVINPUT_NUMPAD4,
			LVINPUT_NUMPAD5,
			LVINPUT_NUMPAD6,
			LVINPUT_NUMPAD7,
			LVINPUT_NUMPAD8,
			LVINPUT_NUMPAD9,
			LVINPUT_MULTIPLY,
			LVINPUT_ADD,
			LVINPUT_SUBTRACT,
			LVINPUT_DIVIDE,
			LVINPUT_DECIMAL,
			// Enum max/count/num
			LVINPUT_MAX
		};

		LVINPUT_KEYCODE WindowsVK_To_LvInput(unsigned int VirtualKey);

		using ButtonState_T = bool;
		using KeyState_T = GameInputKeyState;
		using MouseState_T = GameInputMouseState;
		using GamepadState_T = GameInputGamepadState;
		using GameInputReading_T = IGameInputReading;

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
				/*
				bool LeftButton = false;
				bool RightButton = false;
				bool MiddleButton = false;
				bool WheelButton = false; // Vertical scroll wheel
				*/
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
			// CKA_TODO: Rename the above LVINPUT_KEYCODES that conflict with buttons here
			// CKA_TODO: All LVINPUT_KEYCODES should start with LVINPUT_KEY_...
			// CKA_TODO: After the above, remove the TMP_ prefix from enum values below:
			TMP_LVINPUT_GAMEPAD_INVALID = -1,
			// Dpad
			TMP_LVINPUT_DPAD_UP = 0,
			TMP_LVINPUT_DPAD_LEFT,
			TMP_LVINPUT_DPAD_DOWN,
			TMP_LVINPUT_DPAD_RIGHT,
			// Face buttons: use direction here because of different layouts (Playstation, Xbox, Nintendo, etc.)
			TMP_LVINPUT_FACE_UP,
			TMP_LVINPUT_FACE_LEFT,
			TMP_LVINPUT_FACE_DOWN,
			TMP_LVINPUT_FACE_RIGHT,
			// Shoulder buttons
			TMP_LVINPUT_LSHOULDER,
			TMP_LVINPUT_RSHOULDER,
			TMP_LVINPUT_LTRIGGER,
			TMP_LVINPUT_RTRIGGER,
			// Stick buttons
			TMP_LVINPUT_LSTICK,
			TMP_LVINPUT_RSTICK,
			// General interface (legacy: start/select)
			TMP_LVINPUT_MENU, // Main 'Start'/'Options'/etc button
			TMP_LVINPUT_MORE, // The 'Share' button in PS5/XSX
			TMP_LVINPUT_HOME, // General platform 'Logo' button
			//
			TMP_LVINPUT_GAMEPAD_MAX
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
				ButtonState_T Buttons[TMP_LVINPUT_GAMEPAD_MAX];
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