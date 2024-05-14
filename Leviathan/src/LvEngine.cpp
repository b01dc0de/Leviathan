#include "LvCommon.h"

#include "LvEngine.h"
#include "Leviathan.h"
#include "LvGraphics.h"
#include "LvTime.h"

namespace Leviathan
{
	bool bLvRunning = false;

#if LV_PLATFORM_WINDOWS()
	HINSTANCE Lv_Inst;
	HINSTANCE Lv_PrevInst;
	PSTR Lv_CmdLine;
	int Lv_CmdShow;
#endif // LV_PLATFORM_WINDOWS()

	struct LvEngineInstance
	{
		HWND LvWindow{};

		LvTime EngineTime{};

		void PrivLvInitWindow();

		void Init();
		void Term();
		void MainLoop();

		LvEngineInstance() = default;
		~LvEngineInstance() = default;
		LvEngineInstance(const LvEngineInstance&) = delete;
		LvEngineInstance& operator=(const LvEngineInstance&) = delete;
	};

	static LvEngineInstance* LvPrvEngInst = nullptr;

	HWND Lv_GetWindowHandle()
	{
		Assert(nullptr != LvPrvEngInst);
		return LV_SAFE_DEREF(LvPrvEngInst, LvWindow);
	}

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
		DWORD WndExStyle = WS_EX_OVERLAPPEDWINDOW;
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

		__int64 LvEngineFrameID = 0;
		// Lv misc perf stats:
		double LvTimeElapsed = 0.0;
		double LvDeltaSum = 0.0;
		double LvAvgFrameCost = 0.0;
		double LvAvgDelta = 0.0;
		constexpr int EnginePrintStatFreq = 600; // Every 10 seconds (@ 60 FPS)
		auto UpdateEngineFrameTimings = [&]()
		{
			EngineTime.Measure_EngineFrame();

			LvTimeElapsed = EngineTime.CurrTime();
			LvDeltaSum += EngineTime.Delta();
			LvAvgFrameCost = LvTimeElapsed / ((double)LvEngineFrameID + 1);
			LvAvgDelta = LvDeltaSum / ((double)LvEngineFrameID + 1);

			if (LvEngineFrameID % EnginePrintStatFreq == 0)
			{
				// CKA_TODO: What's the conceptual difference between average delta and average frame cost?
				// 
				LV_DBGOUTF("Leviathan misc perf stats:\n");
				LV_DBGOUTF("\tFrameID: %d\n", LvEngineFrameID);
				LV_DBGOUTF("\tTime Elapsed: %F\n", LvTimeElapsed);
				LV_DBGOUTF("\tAverage Frame Cost: %F\n", LvAvgFrameCost);
				LV_DBGOUTF("\tAverage Delta: %F\n", LvAvgDelta);
			}

			LvEngineFrameID++;
		};

		while (bLvRunning)
		{
			PeekNewMessages();

			LvGraphics::UpdateAndDraw();

			UpdateEngineFrameTimings();
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