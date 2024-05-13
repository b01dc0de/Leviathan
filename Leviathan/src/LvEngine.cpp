#include "LvCommon.h"

#include "LvEngine.h"
#include "Leviathan.h"
#include "LvGraphics.h"

namespace Leviathan
{
	bool bLvRunning = false;

	HWND LvWindow;

	HINSTANCE LvInst;
	HINSTANCE LvPrevInst;
	PSTR LvCmdLine;
	int LvCmdShow;

	HWND LvInitWindow(HINSTANCE hInstance)
	{
		WNDCLASSEX WndClass = {};
		WndClass.cbSize = sizeof(WNDCLASSEX);
		WndClass.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
		WndClass.lpfnWndProc = LvWindowProc;
		WndClass.hInstance = hInstance;
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
			hInstance,
			nullptr // CKA_NOTE: Might want to use this in the future
		);

		return hWindow;
	}

	void LvEngine::Init()
	{
		Outf("LvInitEngine -- BEGIN\n");

		HRESULT Result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		Assert(!FAILED(Result));

		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		HWND hWindow = LvInitWindow(LvInst);
		Assert(hWindow);
		LvWindow = hWindow;

		if (LvWindow)
		{
			LvGraphics::Init();

			ShowWindow(LvWindow, LvCmdShow);

			bLvRunning = true;
		}
		Outf("LvInitEngine -- END\n");
	}

	void LvEngine::Term()
	{
		Outf("LvTermEngine -- BEGIN\n");
		LvGraphics::Term();

		CoUninitialize();

		Outf("LvTermEngine -- END\n");
	}

	void LvEngine::MainLoop()
	{
		Outf("LvMainEngineLoop -- BEGIN\n");
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

		while (bLvRunning)
		{
			PeekNewMessages();

			LvGraphics::UpdateAndDraw();
		}
		Outf("LvMainEngineLoop -- END\n");
	}
}