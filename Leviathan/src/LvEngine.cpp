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
		LvInput_SingleKeyState KeyboardState[LVINPUT_MAX];
		LvInput_MouseState MouseState;

		void PrivLvInitWindow();

		void Init();
		void Term();
		void MainLoop();

		void ProcessInput(RAWKEYBOARD* RawKeyboard);
		void ProcessInput(RAWMOUSE* RawMouse);
		void ProcessInput(RAWHID* RawHID);

		LvEngineInstance() = default;
		~LvEngineInstance() = default;
		LvEngineInstance(const LvEngineInstance&) = delete;
		LvEngineInstance& operator=(const LvEngineInstance&) = delete;
	};

	static LvEngineInstance* LvPrvEngInst = nullptr;

	enum LvInputDeviceType
	{
		LVINPUTDEVICE_KEYBOARD,
		LVINPUTDEVICE_MOUSE,
		LVINPUTDEVICE_GAMEPAD,
		LVINPUTDEVICE_NUM
	};

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

		// Register raw input devices
		RAWINPUTDEVICE RIDev[LVINPUTDEVICE_NUM];
		RIDev[LVINPUTDEVICE_KEYBOARD].usUsagePage = 0x01;
		RIDev[LVINPUTDEVICE_KEYBOARD].usUsage = 0x06;
		RIDev[LVINPUTDEVICE_KEYBOARD].dwFlags = RIDEV_NOLEGACY;
		RIDev[LVINPUTDEVICE_KEYBOARD].hwndTarget = hWindow;
		RIDev[LVINPUTDEVICE_MOUSE].usUsagePage = 0x0001;
		RIDev[LVINPUTDEVICE_MOUSE].usUsage = 0x02;
		RIDev[LVINPUTDEVICE_MOUSE].dwFlags = RIDEV_NOLEGACY;
		RIDev[LVINPUTDEVICE_MOUSE].hwndTarget = hWindow;
		RIDev[LVINPUTDEVICE_GAMEPAD].usUsagePage = 0x0001;
		RIDev[LVINPUTDEVICE_GAMEPAD].usUsage = 0x05;
		RIDev[LVINPUTDEVICE_GAMEPAD].dwFlags = 0;
		RIDev[LVINPUTDEVICE_GAMEPAD].hwndTarget = hWindow;
		Assert(RegisterRawInputDevices(RIDev, LVINPUTDEVICE_NUM, sizeof(RIDev[0])) != FALSE);

	#define GET_DETAILED_RID_INFO() (0)
	#if GET_DETAILED_RID_INFO()
		RID_DEVICE_INFO_MOUSE* MouseInfos = nullptr;
		RID_DEVICE_INFO_KEYBOARD* KeyboardInfos = nullptr;
		RID_DEVICE_INFO_HID* HIDInfos = nullptr;
		UINT NumMice = 0, NumKeyboards = 0, NumHIDs = 0;
		{
			RAWINPUTDEVICELIST* RawInputDeviceList = nullptr;
			RID_DEVICE_INFO* RID_DeviceInfos = nullptr;
			UINT Num_RID = 0;
			{
				(void)GetRawInputDeviceList(
					nullptr,
					&Num_RID,
					sizeof(RAWINPUTDEVICELIST)
				);
				RawInputDeviceList = new RAWINPUTDEVICELIST[Num_RID];
				RID_DeviceInfos = new RID_DEVICE_INFO[Num_RID];
			}
			Assert(GetRawInputDeviceList(
				RawInputDeviceList,
				&Num_RID,
				sizeof(RAWINPUTDEVICELIST)
			) == Num_RID);
			UINT NumDevices[] = { 0,0,0 };
			for (UINT RID_Idx = 0; RID_Idx < Num_RID; RID_Idx++)
			{
				UINT DataSz = sizeof(RID_DEVICE_INFO);
				RID_DeviceInfos[RID_Idx].cbSize = sizeof(RID_DEVICE_INFO);
				UINT BytesOut = GetRawInputDeviceInfo
				(
					RawInputDeviceList[RID_Idx].hDevice,
					RIDI_DEVICEINFO, // RIDI_DEVICENAME, RIDI_PREPARSEDDATA,
					&RID_DeviceInfos[RID_Idx],
					&DataSz
				);
				Assert(BytesOut == DataSz);
				NumDevices[RawInputDeviceList[RID_Idx].dwType]++;
			}
			NumMice = NumDevices[RIM_TYPEMOUSE], NumKeyboards = NumDevices[RIM_TYPEKEYBOARD], NumHIDs = NumDevices[RIM_TYPEHID];
			MouseInfos = new RID_DEVICE_INFO_MOUSE[NumMice];
			KeyboardInfos = new RID_DEVICE_INFO_KEYBOARD[NumKeyboards];
			HIDInfos = new RID_DEVICE_INFO_HID[NumHIDs];
			UINT MiceIdx = 0, KeyboardIdx = 0, HIDIdx = 0;
			for (UINT RID_Idx = 0; RID_Idx < Num_RID; RID_Idx++)
			{
				RID_DEVICE_INFO& CurrDeviceInfo = RID_DeviceInfos[RID_Idx];
				if (CurrDeviceInfo.dwType == RIM_TYPEMOUSE)
				{
					MouseInfos[MiceIdx++] = CurrDeviceInfo.mouse;
				}
				else if (CurrDeviceInfo.dwType == RIM_TYPEKEYBOARD)
				{
					KeyboardInfos[KeyboardIdx++] = CurrDeviceInfo.keyboard;
				}
				else
				{
					Assert(CurrDeviceInfo.dwType == RIM_TYPEHID);
					HIDInfos[HIDIdx++] = CurrDeviceInfo.hid;
				}
			}
			delete[] RawInputDeviceList;
			delete[] RID_DeviceInfos;
		}
	#endif

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

			LvGraphics::UpdateAndDraw();

			UpdateEngineFrameTimings();
			LvEngineFrameID++;
		}
	}

	void LvEngineInstance::ProcessInput(RAWKEYBOARD* RawKeyboard)
	{
		/*
			Currently unused:
				USHORT MakeCode = RawKeyboard->MakeCode; 
				UINT Message = RawKeyboard->Message;
				ULONG ExtraInformation = RawKeyboard->ExtraInformation;
				// Unused flag values: RI_KEY_E0, RI_KEY_E1
		*/
		bool KeyDown = RawKeyboard->Flags == RI_KEY_MAKE;
		bool KeyUp = RawKeyboard->Flags == RI_KEY_BREAK;
		UINT VirtualKey = (UINT)RawKeyboard->VKey;
		LVINPUT_KEYCODE LvKeyCode = WindowsVK_To_LvInput(VirtualKey);
		Assert(LvKeyCode != LVINPUT_INVALID);
		if (LVINPUT_INVALID < LvKeyCode && LvKeyCode < LVINPUT_MAX)
		{
			KeyboardState[LvKeyCode] = {KeyDown, KeyUp, 0};
		}
		for (int KeyIdx = 0; KeyIdx < LVINPUT_MAX; KeyIdx++)
		{
			if (LvKeyCode != KeyIdx)
			{
				continue;
			}

			LvInput_SingleKeyState& CurrKey = KeyboardState[KeyIdx];
			if (CurrKey.Pressed)
			{
				CurrKey.RepeatCount++;
			}
			else if (CurrKey.JustReleased)
			{
				CurrKey.JustReleased = false;
				CurrKey.RepeatCount = 0;
			}
		}
	}

	void LvEngineInstance::ProcessInput(RAWMOUSE* RawMouse)
	{
		MouseState.UpdateState(*RawMouse);
	}

	void LvEngineInstance::ProcessInput(RAWHID* RawHID)
	{
		(void)RawHID;
		//DWORD Size = RawHID->dwSizeHid;
		//DWORD Count = RawHID->dwCount;
		//UINT RawDataBufferSz = Size * Count;
		//BYTE* RawData;
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

	void LvEngine::ProcessRawInput(LPARAM lParam)
	{
		UINT BufferSz;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &BufferSz, sizeof(RAWINPUTHEADER));
		b8* RID_ByteBuffer = new b8[BufferSz];
		Assert(GetRawInputData((HRAWINPUT)lParam, RID_INPUT, RID_ByteBuffer, &BufferSz, sizeof(RAWINPUTHEADER)) == BufferSz);
		{
			RAWINPUT* RawInput = (RAWINPUT*)RID_ByteBuffer;
			if (RawInput->header.dwType == RIM_TYPEKEYBOARD)
			{
				LvPrvEngInst->ProcessInput(&RawInput->data.keyboard);
				/*
					USHORT MakeCode;
					USHORT Flags;
					USHORT Reserved;
					USHORT VKey;
					UINT   Message;
					ULONG ExtraInformation;
				*/
			}
			else if (RawInput->header.dwType == RIM_TYPEMOUSE)
			{
				LvPrvEngInst->ProcessInput(&RawInput->data.mouse);
				/*
					USHORT usFlags;
					ULONG ulButtons; == USHORT  usButtonFlags; USHORT  usButtonData;
					ULONG ulRawButtons;
					LONG lLastX;
					LONG lLastY;
					ULONG ulExtraInformation;
				*/
			}
			else if (RawInput->header.dwType == RIM_TYPEHID)
			{
				LvPrvEngInst->ProcessInput(&RawInput->data.hid);
			}
			else
			{
				LV_FAIL();
			}
		}
	}
}
