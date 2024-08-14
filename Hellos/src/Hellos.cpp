#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Win32_Barebones.h"
#include "Win32_DX11_Triangle.h"

enum struct HelloProject : int
{
	Win32_Barebones,
	Win32_DX11_Triangle,
	Num
};

#define BUILD_PROJECT() (HelloProject::Win32_DX11_Triangle)

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR CmdLine, int WndShow)
{
	int Result = 0;

	switch (BUILD_PROJECT())
	{
		case HelloProject::Win32_Barebones:
		{
			Result = Win32_Barebones_WinMain(hInst, hPrevInst, CmdLine, WndShow);
		} break;
		case HelloProject::Win32_DX11_Triangle:
		{
			Result = Win32_DX11_Triangle_WinMain(hInst, hPrevInst, CmdLine, WndShow);
		} break;
		default:
		{
			Result = 1;
			DebugBreak();
		} break;
	}

	return Result;
}

