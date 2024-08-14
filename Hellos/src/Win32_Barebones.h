#ifndef WIN32_BAREBONES_H
#define WIN32_BAREBONES_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

int WINAPI Win32_Barebones_WinMain
(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	PSTR CmdLine,
	int ShowWnd
);

#endif // WIN32_BAREBONES_H