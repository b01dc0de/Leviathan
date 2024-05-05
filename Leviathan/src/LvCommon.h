#ifndef LVCOMMON_H
#define LVCOMMON_H

#if defined(DEBUG) || defined(_DEBUG)
	#define LV_DEBUG_BUILD() (1)
#else
	#define LV_DEBUG_BUILD() (0)
#endif

#define PLATFORM_WINDOWS() (_MSC_VER > 0)
#define PLATFORM_OTHER() (!(PLATFORM_WINDOWS()))

/* MSVC */
#if PLATFORM_WINDOWS()
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#define Assert(exp) \
		if (!(exp))	\
		{ \
			OutputDebugStringA("Failed expression: '" #exp "'\nIn File: " __FILE__ "\n"/*":" __LINE__*/); \
			DebugBreak(); \
		}
#elif PLATFORM_OTHER()
	#define Assert(exp)	\
		if (!(exp)) \
		{ \
			printf("Failed expression: '" #exp "'\nIn File: " __FILE__ "\n"/*":" __LINE__*/); \
			__builtin_trap(); \
		}
#else
	#error "No platform defined"
#endif // PLATFORM

#include <cstdio>
#include <vector>

#endif // LVCOMMON_H