#ifndef WIN32_DX11_TRIANGLE_H
#define WIN32_DX11_TRIANGLE_H

// C++ Std Lib
#include <cstdio>
#include <vector>
// Win32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
// DX11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

int WINAPI Win32_DX11_Triangle_WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	int nCmdShow
);

#endif // WIN32_DX11_TRIANGLE_H