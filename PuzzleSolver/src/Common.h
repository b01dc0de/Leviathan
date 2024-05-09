#ifndef COMMON_H
#define COMMON_H

/* C STD Library */
#include <cstdio>
/* C++ STD Library */
#include <random>
#include <algorithm>

//#include "LvCommon.h"

// CKA_NOTE: Only stub this in temporarily to get this to compile on Windows
//		Don't want to fiddle with making LVCommon a shared library right now
#if (_MSC_VER > 0)
#include <Windows.h>
#define Assert(exp) \
		if (!(exp))	\
		{ \
			DebugBreak(); \
		}
#else
#define Assert(exp) (void)0
#endif

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