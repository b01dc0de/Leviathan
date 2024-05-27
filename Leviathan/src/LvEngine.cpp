#include "LvCommon.h"

#include "LvEngine.h"
#include "Leviathan.h"
#include "LvGraphics.h"
#include "LvTime.h"
#include "LvInput.h"

namespace Leviathan
{
	bool bLvRunning = false;

	struct LvEngineInstance
	{
		HWND LvWindow{};

		LvTime EngineTime{};

		IGameInput* GameInputInst = nullptr;
		GameInputKind EnabledInputTypes = GameInputKindKeyboard|GameInputKindMouse|GameInputKindGamepad;
		LvKeyboardState KeyboardState;
		LvMouseState MouseState;
		LvGamepadState GamepadState;

		void PrivLvInitWindow();

		void Init();
		void Term();
		void MainLoop();

		void PollInput();

		LvEngineInstance() = default;
		~LvEngineInstance() = default;
		LvEngineInstance(const LvEngineInstance&) = delete;
		LvEngineInstance& operator=(const LvEngineInstance&) = delete;
	};

	static LvEngineInstance* LvPrvEngInst = nullptr;

#if LV_PLATFORM_WINDOWS()
	HINSTANCE Lv_Inst;
	HINSTANCE Lv_PrevInst;
	PSTR Lv_CmdLine;
	int Lv_CmdShow;

	HWND Lv_GetWindowHandle()
	{
		Assert(nullptr != LvPrvEngInst);
		return LV_SAFE_DEREF(LvPrvEngInst, LvWindow);
	}
#endif // LV_PLATFORM_WINDOWS

	void LvEngineInstance::PrivLvInitWindow()
	{
		// Unreleated to actual window initialization
		{
		#if LV_CONFIG_DEBUG() && LV_PLATFORM_WINDOWS()
			Assert(!FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)));
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		#endif // LV_PLATFORM_WINDOWS()
		}

		WNDCLASSEX WndClass = {};
		WndClass.cbSize = sizeof(WNDCLASSEX);
		WndClass.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
		WndClass.lpfnWndProc = LvWindowProc;
		WndClass.hInstance = Lv_Inst;
		WndClass.lpszClassName = AppName;

		RegisterClassEx(&WndClass);

		RECT WndRect = { 0, 0, (LONG)ResX, (LONG)ResY };
		DWORD WndStyle = WS_CAPTION;
		DWORD WndExStyle = WS_EX_OVERLAPPEDWINDOW|WS_EX_APPWINDOW;
		AdjustWindowRectEx(&WndRect, WndStyle, FALSE, WndExStyle);

		HWND hWindow = CreateWindowEx(
			WndExStyle,
			AppName,
			AppName,
			WndStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			WndRect.right - WndRect.left,
			WndRect.bottom - WndRect.top,
			nullptr,
			nullptr,
			Lv_Inst,
			nullptr // CKA_NOTE: Might want to use this in the future
		);

		// Init GameInput
		Assert(!FAILED(GameInputCreate(&GameInputInst)));

		Assert(hWindow);
		LvWindow = hWindow;
	}

	void LvEngineInstance::Init()
	{
		LV_DBGTRACESCOPE();
		PrivLvInitWindow();
		if (LvWindow)
		{
			LvGraphics::Init();

			EngineTime.Init();

			ShowWindow(LvWindow, Lv_CmdShow);

			bLvRunning = true;
		}
	}

	void LvEngineInstance::Term()
	{
		LV_DBGTRACESCOPE();
		LvGraphics::Term();
	#if LV_PLATFORM_WINDOWS()
		CoUninitialize();
	#endif // LV_PLATFORM_WINDOWS()
	}

	void LvEngineInstance::MainLoop()
	{
		LV_DBGTRACESCOPE();
		auto PeekNewMessages = [&]()
		{
			MSG Msg;
			BOOL MsgResult;
			while ((MsgResult = PeekMessage(&Msg, LvWindow, 0, 0, PM_REMOVE)) > 0)
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
			if (MsgResult == -1) { /*CKA_TODO*/ }
		};

		__int64 LvEngineFrameID = 1;
		// Lv misc perf stats:
		constexpr int EnginePrintStatFreq = 600; // Every 10 seconds (@ 60 FPS)
		auto UpdateEngineFrameTimings = [&]()
		{
			EngineTime.Measure_EngineFrame();

			(void)EnginePrintStatFreq;
			if (0) //(LvEngineFrameID % EnginePrintStatFreq == 0)
			{
				double LvTimeElapsed = EngineTime.CurrTime();
				double LvAvgFrameCost = LvTimeElapsed / ((double)LvEngineFrameID + 1);

				LV_DBGOUTF("Leviathan misc perf stats:\n");
				LV_DBGOUTF("\tFrameID: %d\n", LvEngineFrameID);
				LV_DBGOUTF("\tTime Elapsed (s): %.04F\n", LvTimeElapsed);
				LV_DBGOUTF("\tAverage Frame Cost (ms): %.02F\n", LvAvgFrameCost * 1000.0);
			}
		};

		while (bLvRunning)
		{
			PeekNewMessages();
			PollInput(); // CKA_TODO: Where should this go within the loop?

			LvGraphics::UpdateAndDraw();

			UpdateEngineFrameTimings();
			LvEngineFrameID++;
		}
	}

	constexpr LvKeyInputCombo EscapeShortcuts[] =
	{
		{LVINPUT_ESC},
		{LVINPUT_ALT, LVINPUT_KEY_F4},
		{LVINPUT_CTRL, LVINPUT_KEY_Q},
	};
	constexpr s32 NumEscapeShortcuts = LV_ARRAYSIZE(EscapeShortcuts);

	void LvEngineInstance::PollInput()
	{
		IGameInputReading* CurrInputReading = nullptr;
		if (SUCCEEDED(GameInputInst->GetCurrentReading(EnabledInputTypes, nullptr, &CurrInputReading)))
		{
			Assert(CurrInputReading);
			// uint64_t ReadingTs = CurrInputReading->GetTimestamp();
			// uint64_t GetCurrentTimestamp()
			GameInputKind InputDeviceType = CurrInputReading->GetInputKind();
			bool bResult = false;
			if (InputDeviceType == GameInputKindKeyboard)
			{
				bResult = KeyboardState.ProcessInput(CurrInputReading);
			}
			else if (InputDeviceType == GameInputKindMouse)
			{
				bResult = MouseState.ProcessInput(CurrInputReading);
			}
			else if (InputDeviceType == GameInputKindGamepad)
			{
				bResult = GamepadState.ProcessInput(CurrInputReading);
			}
			if (bResult)
			{
				if (KeyboardState.AnyCombosPressed(EscapeShortcuts, NumEscapeShortcuts))
				{
					bLvRunning = false;
				}
			}
			else
			{
				// Idk, some error handling
			}
			
		}
	}

	void LvEngine::Init()
	{
		Assert(nullptr == LvPrvEngInst);
		LvPrvEngInst = new LvEngineInstance;
		LvPrvEngInst->Init();
	}

	void LvEngine::Term()
	{
		Assert(LvPrvEngInst);
		LvPrvEngInst->Term();

		delete LvPrvEngInst;
		LvPrvEngInst = nullptr;
	}

	void LvEngine::MainLoop()
	{
		Assert(LvPrvEngInst);
		LvPrvEngInst->MainLoop();
	}
}
