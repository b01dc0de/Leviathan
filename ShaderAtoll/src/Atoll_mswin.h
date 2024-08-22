#ifndef ATOLL_MSWIN_H
#define ATOLL_MSWIN_H

#include "AtollCommon.h"

namespace ShaderAtoll
{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND InitWindow(HINSTANCE hInstance);
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
}

#endif // ATOLL_MSWIN_H