#include "AtollUtil.h"

namespace ShaderAtoll
{
	SysInfo::SysInfo()
	{
	}

	AtollClock::TsType AtollClock::Freq = {};

	double AtollClock::ConvertTs_s(TsType InTs)
	{
		// Protect against divide by zero
		if (0 == Freq.QuadPart) { return 0.0; }

		double fTs_us = (double)(InTs.QuadPart * Ticks_to_us);
		double fFreq_us = (double)Freq.QuadPart;

		double fTime_ms = (fTs_us / fFreq_us) / (double)Ticks_to_us;
		return fTime_ms;
	}

	void AtollClock::Tick()
	{
		TsType Ts_Prev = Ts_Last;
		TsType Ts_QPC = {};
		QueryPerformanceCounter(&Ts_QPC);

		// Convert to app time
		Ts_Last.QuadPart = Ts_QPC.QuadPart - T0.QuadPart;
		Ts_Delta.QuadPart = Ts_Last.QuadPart - Ts_Prev.QuadPart;

		fLast_ms = ConvertTs_s(Ts_Last);
		fDelta_ms = ConvertTs_s(Ts_Delta);
	}

	void AtollClock::Init()
	{
		TsType Ts_T0 = {}, PerfFreq = {};
		QueryPerformanceCounter(&Ts_T0);
		QueryPerformanceFrequency(&PerfFreq);

		T0 = Ts_T0;
		Freq = PerfFreq;
		Ts_Last = Ts_T0;
		Ts_Delta = {};
	}


}
