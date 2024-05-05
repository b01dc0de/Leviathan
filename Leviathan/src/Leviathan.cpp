#include "LvCommon.h"

#include "Leviathan.h"
#include "LvGraphics.h"

LRESULT CALLBACK LvWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace Leviathan
{
	HINSTANCE LvInst;
	HINSTANCE LvPrevInst;
	PSTR LvCmdLine;
	int LvCmdShow;

	HWND LvWindow;

	int ResX = 1600;
	int ResY = 900;
	const WCHAR* AppName = L"Leviathan";
	bool bRunning = false;

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

		LvInitGraphics();

		ShowWindow(hWindow, LvCmdShow);

		bRunning = true;
	}
	void LvTermEngine()
	{
	}

	void LvMainEngineLoop()
	{
		while (bRunning)
		{
			// Get input
			MSG msg;
			BOOL MsgResult;
			while ((MsgResult = PeekMessage(&msg, LvWindow, 0, 0, PM_REMOVE)) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (MsgResult == -1)
			{
				// CKA_TODO: Error handling?
				//bGlobalRunning = false;
				break;
			}

			// Update
			UpdateWindow(LvWindow);

			LvDraw();
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