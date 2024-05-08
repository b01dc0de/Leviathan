#ifndef LVCOMMON_H
#define LVCOMMON_H

#if defined(DEBUG) || defined(_DEBUG)
	#define LV_DEBUG_BUILD() (1)
#else
	#define LV_DEBUG_BUILD() (0)
#endif

#define UNUSED_VAR(var) (void)var

#define PLATFORM_WINDOWS() (_MSC_VER > 0)
#define PLATFORM_OTHER() (!(PLATFORM_WINDOWS()))

void Outf(const char* fmt, ...);

/* MSVC */
#if PLATFORM_WINDOWS()
	#if LV_DEBUG_BUILD()
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
	#endif
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#define Assert(exp) \
		if (!(exp))	\
		{ \
			Outf("Failed expression: '" #exp "'\nIn File: " __FILE__ "\tLine: %d \n", __LINE__); \
			DebugBreak(); \
		}
#elif PLATFORM_OTHER()
	#define Assert(exp)	\
		if (!(exp)) \
		{ \
			Outf("Failed expression: '" #exp "'\nIn File: " __FILE__ "\tLine: %d \n", __LINE__); \
			__builtin_trap(); \
		}
#else
	#error "No platform defined"
#endif // PLATFORM

#include <cstdio>
#include <vector>

#include "LvMath.h"

#endif // LVCOMMON_H