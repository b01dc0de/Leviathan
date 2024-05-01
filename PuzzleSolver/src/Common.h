#ifndef COMMON_H
#define COMMON_H

/* C STD Library */
#include <cstdio>
/* C++ STD Library */
#include <random>
#include <algorithm>

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
		printf("Failed expression: '" #exp "'\nIn File: " __FILE__ "\n"/*":" __LINE__*/); \
		__builtin_trap(); \
	}
#else
#error "No platform defined"
#endif // PLATFORM

int GenRandInRange(int Min, int Max);

template <typename T>
void SimpleShuffle(T* Src, int Size)
{
	for (int Idx = 0; Idx < Size - 1; Idx++)
	{
		int Min = Idx;
		int Max = Size - 1;
		int SwapWithIdx = GenRandInRange(Min, Max);

		T Temp = Src[Idx];
		Src[Idx] = Src[SwapWithIdx];
		Src[SwapWithIdx] = Temp;
	}
}

#define ENABLE_DBG_PRINT() (1)
#if ENABLE_DBG_PRINT()
#define DBG_PRINT(...) printf(__VA_ARGS__)
#else
#define DBG_PRINT(...) (void)0
#endif // ENABLE_DBG_PRINT()

#endif // COMMON_H