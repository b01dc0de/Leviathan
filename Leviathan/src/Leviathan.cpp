#include "LvCommon.h"

#include "Leviathan.h"
#include "LvGraphics.h"

LRESULT CALLBACK LvWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace Leviathan
{
	extern const int ResX = 1600;
	extern const int ResY = 900;
	extern const WCHAR* AppName = L"Leviathan";

	bool bRunning = false;

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

		ATOM RegClassResult = RegisterClassEx(&WndClass);

		RECT WndRect = { 0, 0, (LONG)ResX, (LONG)ResY };
		UINT WndStyle = WS_CAPTION;
		UINT WndExStyle = WS_EX_OVERLAPPEDWINDOW;
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
		HWND hWindow = LvInitWindow(LvInst);
		Assert(hWindow);
		LvWindow = hWindow;

		if (LvWindow)
		{
			LvGraphics::Init();

			ShowWindow(LvWindow, LvCmdShow);

			bRunning = true;
		}
	}
	void LvTermEngine()
	{
		LvGraphics::Term();
	}

	void LvMainEngineLoop()
	{
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

		while (bRunning)
		{
			PeekNewMessages();

			// Update
			UpdateWindow(LvWindow);

			LvGraphics::Draw();
		}
	}

} // namepace Leviathan

LRESULT CALLBACK LvWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result;
	switch (uMsg)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_KEYDOWN:
		{
			Leviathan::bRunning = false;
		}
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

	LvInitEngine();
	LvMainEngineLoop();
	LvTermEngine();
}