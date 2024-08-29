#include "AtollUtil.h"

namespace ShaderAtoll
{
	SysInfo GlobalSysInfo;

	SysInfo::SysInfo()
	{
		RECT ResXY = {};
		/*
		SystemParametersInfoA(
			...,
			0,
			&ResXY,
			0
		);
		PrimaryResX = 0;
		PrimaryResY = 0;
		*/

		SystemParametersInfoA(
			SPI_GETWORKAREA,
			0,
			&ResXY,
			0
		);
		PrimaryWorkAreaX = ResXY.right - ResXY.left;
		PrimaryWorkAreaY = ResXY.bottom - ResXY.top;
	}

	AtollClock::TsType AtollClock::Freq = {};

	double AtollClock::ConvertTs_s(TsType InTs)
	{
		// Protect against divide by zero
		if (0 == Freq.QuadPart) { return 0.0; }

		double fTs_us = (double)(InTs.QuadPart * Ticks_to_us);
		double fFreq_us = (double)Freq.QuadPart;

		double fTime_s = (fTs_us / (fFreq_us * (double)Ticks_to_us));
		return fTime_s;
	}

	void AtollClock::Tick()
	{
		TsType Ts_Prev = Ts_Last;
		TsType Ts_QPC = {};
		QueryPerformanceCounter(&Ts_QPC);

		// Convert to app time
		Ts_Last.QuadPart = Ts_QPC.QuadPart - T0.QuadPart;
		Ts_Delta.QuadPart = Ts_Last.QuadPart - Ts_Prev.QuadPart;

		fLast_s = ConvertTs_s(Ts_Last);
		fDelta_ms = ConvertTs_s(Ts_Delta) * (double)ms_per_s;
	}

	void AtollClock::Rest()
	{
		TsType CurrFrameTime;
		QueryPerformanceCounter(&CurrFrameTime);

		CurrFrameTime.QuadPart -= T0.QuadPart;
		float CurrFrameTime_ms = (ConvertTs_s(CurrFrameTime) - fLast_s) * 1000.0f;

		constexpr float MS_PER_FRAME_60Hz = 1000.0f / 60.0f;
		DWORD msToSleep = DWORD(MS_PER_FRAME_60Hz - CurrFrameTime_ms);
		Sleep(DWORD(MS_PER_FRAME_60Hz - CurrFrameTime_ms));
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
