#ifndef LEVIATHAN_H
#define LEVIATHAN_H

// C/C++ std lib
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <random>

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
// XInput
#include <Xinput.h>

namespace Leviathan
{

using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long;
using ullong = unsigned long long;

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

extern int AppWidth;
extern int AppHeight;
extern float AspectRatio;
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

#include "Utils.h"
#include "Array.h"
#include "Clock.h"
#include "Engine.h"
#include "Graphics.h"
#include "Input.h"
#include "Math.h"

#endif // LEVIATHAN_H

