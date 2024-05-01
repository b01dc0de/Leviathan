#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool bGlobalRunning = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	LPCWSTR AppName = L"HelloTriangle";

	WNDCLASS WndClass =
	{
		CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW,
		WindowProc,
		0,
		0,
		hInstance,
		0,
		0,
		0,
		nullptr,
		AppName
	};

	ATOM RegClassResult = RegisterClass(&WndClass);

	HINSTANCE hInst = hInstance;
	HWND hWindow = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		AppName,
		AppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		nullptr,
		hInst,
		nullptr // CKA_NOTE: Might want to use this in the future
	);

	if (hWindow)
	{
		ShowWindow(hWindow, nCmdShow);
		UpdateWindow(hWindow);

		bGlobalRunning = true;
		while (bGlobalRunning)
		{
			MSG msg;
			BOOL MsgResult;
			while ((MsgResult = PeekMessage(&msg, hWindow, 0, 0, PM_REMOVE)) > 0)
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
		}
	}
	else
	{
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result;
	switch (uMsg)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_KEYDOWN:
		{
			bGlobalRunning = false;
		}
		default:
		{
			Result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		} break;
	}
	
	return Result;
}
