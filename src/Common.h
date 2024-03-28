#ifndef COMMON_H
#define COMMON_H

/* C STD Library */
#include <cstdio>
/* C++ STD Library */
#include <random>

#define PLATFORM_WINDOWS (_MSC_VER > 0)
#define PLATFORM_OTHER (!(PLATFORM_WINDOWS))

/* MSVC */
#if PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define Assert(exp) \
	if (!(exp))	\
	{ \
		OutputDebugStringA("Failed expression: '" #exp "'\nIn File: " __FILE__ "\n"/*":" __LINE__*/); \
		DebugBreak(); \
	}
#elif PLATFORM_OTHER
#define Assert(exp)	\
	if (!(exp)) \
	{ \
		(int*)0 = 0; \
	}
#else
#error "No platform defined"
#endif // PLATFORM

#endif // COMMON_H