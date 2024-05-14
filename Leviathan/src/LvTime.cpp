#include "LvCommon.h"

#include "LvTime.h"

namespace Leviathan
{
	double LvTime::Delta()
	{
		return fDelta;
	}

	double LvTime::CurrTime()
	{
		return fCurrTime;
	}

	void LvTime::Init()
	{
		LARGE_INTEGER TmpFreq, TmpEpoch;
		QueryPerformanceFrequency(&TmpFreq);
		QueryPerformanceCounter(&TmpEpoch);

		QP_Freq = TmpFreq.QuadPart;
		QP_Epoch = TmpEpoch.QuadPart;

		fCurrTime = 0.0;
		fPrevTime = 0.0;
		fDelta = 0.0;
	}

	void LvTime::Measure_EngineFrame()
	{
		LARGE_INTEGER TmpNewTime;
		QueryPerformanceCounter(&TmpNewTime);

		fPrevTime = fCurrTime;
		fCurrTime = ((double)TmpNewTime.QuadPart - (double)QP_Epoch) / (double)QP_Freq;;
		fDelta = fCurrTime - fPrevTime;
	}
}