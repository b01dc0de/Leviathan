#ifndef COMMON_H
#define COMMON_H

/* C Std Library */
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
/* C++ Std Library */
#include <random>
#include <algorithm>

#if (_MSC_VER > 0)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define Assert(exp) \
		if (!(exp))	\
		{ \
			DebugBreak(); \
		}
#else
#define Assert(exp) (void)0
#endif

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using s32 = int;

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