#include "LvCommon.h"

#include "LvTime.h"

namespace Leviathan
{
	double LvTime::ToDouble(__int64 Timestamp)
	{
		return (double)(Timestamp / Freq);
	}

	void LvTime::Init()
	{
		LARGE_INTEGER TmpFreq = {};
		LARGE_INTEGER InitTime = {};
		QueryPerformanceFrequency(&TmpFreq);
		QueryPerformanceCounter(&InitTime);

		Freq = TmpFreq.QuadPart;
		Epoch = InitTime.QuadPart;
		Assert(0 != Freq);

		LastFrameTime = Epoch;
		LastDelta = 0;
	}

	void LvTime::Measure_EngineFrame()
	{
		LARGE_INTEGER TmpTimestamp = {};
		QueryPerformanceCounter(&TmpTimestamp);
		__int64 TmpTime = TmpTimestamp.QuadPart;

		LastDelta = TmpTime - LastFrameTime;
		LastFrameTime = TmpTime;
	}
}