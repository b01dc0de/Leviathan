#ifndef ATOLLUTIL_H
#define ATOLLUTIL_H

#include "AtollCommon.h"

#if UNICODE
	#define APPNAME() (L"ShaderAtoll")
#else // UNICODE
	#define APPNAME() ("ShaderAtoll")
#endif // UNICODE

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))

namespace ShaderAtoll
{
	namespace Consts
	{
		static constexpr bool bUnicode = (bool)UNICODE;
	};

	namespace Defaults
	{
		static constexpr UINT WinResX = 1024U;
		static constexpr UINT WinResY = 1024U;
	};

	struct SysInfo
	{
		SysInfo();
		SysInfo(const SysInfo&) = delete;
		SysInfo& operator=(const SysInfo&) = delete;
		~SysInfo() = delete;
	};

	struct AtollClock
	{
		static constexpr __int64 Ticks_to_us = 100000; // ticks -> microseconds
		static constexpr __int64 us_per_ms = 1000;

		using TsType = LARGE_INTEGER;
		static TsType Freq;

		TsType T0 = {};
		TsType Ts_Last = {};
		TsType Ts_Delta = {};
		double fLast_ms = 0.0;
		double fDelta_ms = 0.0;

		static double ConvertTs_s(TsType InTs);
		void Tick();
		void Init();
	};
}

#endif // ATOLLUTIL_H
