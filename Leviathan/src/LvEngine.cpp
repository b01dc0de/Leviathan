#include "LvCommon.h"

#include "LvEngine.h"
#include "Leviathan.h"
#include "LvGraphics.h"
#include "LvTime.h"

namespace Leviathan
{
	bool bLvRunning = false;

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

		UINT NumRawInputDevices = 0;
		Assert(GetRawInputDeviceList(nullptr, &NumRawInputDevices, sizeof(RAWINPUTDEVICELIST)) != -1);
		RAWINPUTDEVICELIST* RawInputList = new RAWINPUTDEVICELIST[NumRawInputDevices];
		Assert(GetRawInputDeviceList(RawInputList, &NumRawInputDevices, sizeof(RAWINPUTDEVICELIST)) != -1);
		HANDLE HRawKeyboard = nullptr, HRawMouse = nullptr;
		LvArray<RAWINPUTDEVICELIST> OtherHandles;
		for (UINT RawInputIdx = 0; RawInputIdx < NumRawInputDevices; RawInputIdx++)
		{
			const RAWINPUTDEVICELIST& CurrInputDesc = RawInputList[RawInputIdx];
			if (!HRawKeyboard && CurrInputDesc.dwType == RIM_TYPEKEYBOARD)
			{
				HRawKeyboard = CurrInputDesc.hDevice;
			}
			else if (!HRawMouse && CurrInputDesc.dwType == RIM_TYPEMOUSE)
			{
				HRawMouse = CurrInputDesc.hDevice;
			}
			else
			{
				OtherHandles.AddItem(CurrInputDesc);
			}
		}
		RID_DEVICE_INFO TmpKeyboardDeviceInfo; TmpKeyboardDeviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
		RID_DEVICE_INFO TmpMouseDeviceInfo; TmpMouseDeviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
		UINT SzData = 0;
		Assert(GetRawInputDeviceInfo(HRawKeyboard, RIDI_DEVICEINFO, &TmpKeyboardDeviceInfo, &SzData) > 0);
		Assert(GetRawInputDeviceInfo(HRawMouse, RIDI_DEVICEINFO, &TmpMouseDeviceInfo, &SzData) > 0);
		RID_DEVICE_INFO_KEYBOARD KeyboardDeviceInfo = TmpKeyboardDeviceInfo.keyboard;
		RID_DEVICE_INFO_MOUSE MouseDeviceInfo = TmpMouseDeviceInfo.mouse;
		LvArray<RID_DEVICE_INFO_MOUSE> OtherMouseDevs;
		LvArray<RID_DEVICE_INFO_KEYBOARD> OtherKeyboardDevs;
		LvArray<RID_DEVICE_INFO_HID> OtherHIDs;
		for (UINT OtherInputIdx = 0; OtherInputIdx < OtherHandles.NumItems; OtherInputIdx++)
		{
			// RIM_TYPEMOUSE       0
			// RIM_TYPEKEYBOARD    1
			// RIM_TYPEHID         2
			RID_DEVICE_INFO CurrInput;
			CurrInput.cbSize = sizeof(RID_DEVICE_INFO);
			Assert(GetRawInputDeviceInfo(OtherHandles[OtherInputIdx].hDevice, RIDI_DEVICEINFO, &CurrInput, &SzData) > 0);
			if (CurrInput.dwType == RIM_TYPEMOUSE)
			{
				OtherMouseDevs.AddItem(CurrInput.mouse);
			}
			else if (CurrInput.dwType == RIM_TYPEKEYBOARD)
			{
				OtherKeyboardDevs.AddItem(CurrInput.keyboard);
			}
			else
			{
				Assert(CurrInput.dwType == RIM_TYPEHID);
				OtherHIDs.AddItem(CurrInput.hid);
			}
		}

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

			if (LvEngineFrameID % EnginePrintStatFreq == 0)
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

			LvGraphics::UpdateAndDraw();

			UpdateEngineFrameTimings();
			LvEngineFrameID++;
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