#ifndef LEVIATHAN_H
#define LEVIATHAN_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <dxgi1_2.h>

namespace Leviathan
{
    extern int AppWidth;
    extern int AppHeight;
    extern bool bAppRunning;
    extern HWND AppWindow;
}

#endif // LEVIATHAN_H

