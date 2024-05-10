#include "LvCommon.h"

#include "Leviathan.h"
#include "LvGraphics.h"

LRESULT CALLBACK LvWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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

	void LvInitEngine()
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
	void LvTermEngine()
	{
		Outf("LvTermEngine -- BEGIN\n");
		LvGraphics::Term();

		CoUninitialize();

		Outf("LvTermEngine -- END\n");
	}

	void LvMainEngineLoop()
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

} // namepace Leviathan

LRESULT CALLBACK LvWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;
	switch (uMsg)
	{
		case WM_QUIT:
		case WM_CLOSE:
		case WM_DESTROY:
		{
			Leviathan::bLvRunning = false;
		} break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_KEYDOWN:
		{
			Leviathan::bLvRunning = false;
		} break;
		default:
		{
			Result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		} break;
	}
	return Result;
}

int WINAPI WinMain(HINSTANCE Inst, HINSTANCE PrevInst, PSTR CmdLine, int CmdShow)
{
	using namespace Leviathan;

	LvInst = Inst;
	LvPrevInst = PrevInst;
	LvCmdLine = CmdLine;
	LvCmdShow = CmdShow;

	int Result = 1;

	LvInitEngine();
	LvMainEngineLoop();
	LvTermEngine();

	return Result;
}