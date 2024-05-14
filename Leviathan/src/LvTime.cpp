#include "LvCommon.h"

#include "LvTime.h"

namespace Leviathan
{
	double LvTime::ToDouble(__int64 Timestamp)
	{
		return (double)(Timestamp / _Freq);
	}

	double LvTime::Delta()
	{
		return ToDouble(_LatestDelta);
	}

	double LvTime::CurrTime()
	{
		return ToDouble(_LatestFrameTime - _EngineProcessEpoch);
	}

	void LvTime::Init()
	{
		LARGE_INTEGER TmpFreq = {};
		LARGE_INTEGER InitTime = {};
		QueryPerformanceFrequency(&TmpFreq);
		QueryPerformanceCounter(&InitTime);

		_Freq = TmpFreq.QuadPart;
		_EngineProcessEpoch = InitTime.QuadPart;
		Assert(0 != _Freq);

		_LatestFrameTime = _EngineProcessEpoch;
		_LatestDelta = 0;
	}

	void LvTime::Measure_EngineFrame()
	{
		LARGE_INTEGER TmpTimestamp = {};
		QueryPerformanceCounter(&TmpTimestamp);
		__int64 NewTime = TmpTimestamp.QuadPart;

		_LatestDelta = NewTime - _LatestFrameTime;
		_LatestFrameTime = NewTime;
	}
}