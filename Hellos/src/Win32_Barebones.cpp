#if defined(BUILD_PROJECT)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

LRESULT WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#if UNICODE
#define APPNAME() (L"Win32_DirectX11")
#else
#define APPNAME() ("Win32_DirectX11")
#endif

bool bRunning = true;

int WINAPI WinMain
(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	PSTR CmdLine,
	int ShowWnd
)
{
	int ExitCode = 0;

	WNDCLASSEX WndClassEx = {};
	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	WndClassEx.lpfnWndProc = WinProc;
	WndClassEx.hInstance = hInst;
	WndClassEx.lpszClassName = APPNAME();
	RegisterClassEx(&WndClassEx);

	DWORD WndStyle = 0;
	DWORD WndExStyle = WS_EX_OVERLAPPEDWINDOW;

	UINT WindowWidth = 1280;
	UINT WindowHeight= 720;
	
	HWND hWindowEx = CreateWindowEx
	(
		WndExStyle,
		APPNAME(),
		APPNAME(),
		WndStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WindowWidth,
		WindowHeight,
		nullptr,
		nullptr,
		hInst,
		nullptr
	);

	if (hWindowEx)
	{
		bRunning = true;
		ShowWindow(hWindowEx, ShowWnd);

		while (bRunning)
		{
			MSG msg;
			BOOL bResult;
			while (bResult = PeekMessage(&msg, hWindowEx, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	else
	{
		ExitCode = 1;
		DebugBreak();
	}

	return ExitCode;
}

LRESULT WinProc
(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	LRESULT lResult = 0;
	switch (uMsg)
	{
		case WM_KEYUP:
		{
			if (VK_ESCAPE == wParam)
			{
				bRunning = false;
			}
		} break;
		default:
		{
			lResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
		} break;
	}

	return lResult;
}
#endif // defined(BUILD_PROJECT)
