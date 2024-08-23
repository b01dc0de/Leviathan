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

	// Globals
	extern bool bRunning;
	extern HWND hWindow;
	extern UINT WinResX;
	extern UINT WinResY;
	extern float AppTime_ms;
	extern float DeltaTime_ms;
}

#endif // ATOLLUTIL_H
