#ifndef LEVIATHAN_H
#define LEVIATHAN_H

// C/C++ std lib
#include <cmath>

// Win32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>
#include <hidusage.h>
// DirectX 11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <dxgi1_2.h>
// Direct2D
#include <d2d1.h>

namespace Leviathan
{
    extern int AppWidth;
    extern int AppHeight;
    extern bool bAppRunning;
    extern HWND AppWindow;
}

#define LV_UNUSED(Var) (void)Var
#define ASSERT(Expr) if (!(Expr)) { DebugBreak(); /* TODO: Log */ }
#define ARRAY_SIZE(Array) (sizeof(Array) / sizeof(Array[0]))
#define APPNAMEW() (L"Leviathan")
#define APPNAMEA() ("Leviathan")
#if UNICODE
    #define APPNAME() APPNAMEW()
#else // UNICODE
    #define APPNAME() APPNAMEA()
#endif // UNICODE

#include "Engine.h"
#include "Input.h"
#include "Math.h"

#endif // LEVIATHAN_H

