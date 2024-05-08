#ifndef COMMON_H
#define COMMON_H

/* C STD Library */
#include <cstdio>
/* C++ STD Library */
#include <random>
#include <algorithm>

//#include "LvCommon.h"

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