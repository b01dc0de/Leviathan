#include "Atoll_mswin.h"
#include "AtollGraphics.h"

namespace ShaderAtoll
{
	HWND InitWindow(HINSTANCE hInstance)
	{
		WNDCLASSEX WndClass = {};
		WndClass.cbSize = sizeof(WNDCLASSEX);
		WndClass.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
		WndClass.lpfnWndProc = WindowProc;
		WndClass.hInstance = hInstance;
		WndClass.lpszClassName = APPNAME();

		ATOM RegClassResult = RegisterClassEx(&WndClass);

		RECT WndRect = { 0, 0, (LONG)ShaderAtoll::WinResX, (LONG)ShaderAtoll::WinResY };
		UINT WndStyle = WS_CAPTION;
		UINT WndExStyle = WS_EX_OVERLAPPEDWINDOW;
		AdjustWindowRectEx(&WndRect, WndStyle, FALSE, WndExStyle);

		HWND hWindow = CreateWindowEx(
			WndExStyle,
			APPNAME(),
			APPNAME(),
			WndStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			WndRect.right - WndRect.left,
			WndRect.bottom - WndRect.top,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);

		return hWindow;
	}

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT Result = 0;
		switch (uMsg)
		{
			case WM_KEYUP:
			{
				if (VK_ESCAPE == wParam)
				{
					ShaderAtoll::bRunning = false;
				}
			} break;
			default:
			{
				Result = DefWindowProc(hwnd, uMsg, wParam, lParam);
			} break;
		}

		return Result;
	}

	int WINAPI WinMain
	(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		PSTR lpCmdLine,
		int nCmdShow
	)
	{
		return AtollEngine::Run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	}
}
