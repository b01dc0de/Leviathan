#ifndef ATOLLENGINE_H
#define ATOLLENGINE_H

#include "AtollCommon.h"

namespace ShaderAtoll
{
	// Globals
	extern bool bRunning;
	extern HWND hWindow;
	extern UINT WinResX;
	extern UINT WinResY;
	extern double AppTime_s;
	extern double DeltaTime_ms;

	namespace AtollEngine
	{
		enum struct AtollErrorType : int
		{
			NO_ERR,
			ERR_STARTUP,
			ERR_RUNTIME,
			ERR_SHUTDOWN,
		};

#if PLATFORM_WINDOWS()
		int WINAPI Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
		void HandleKeyInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
#elif PLATFORM_OTHER()
		int Run(int argc, const char* argv[]);
		void HandleKeyInput();
#endif // PLATFORM
	};

#if PLATFORM_WINDOWS()
	struct WindowsPlatformState
	{
		HINSTANCE hInstance = nullptr;
		HINSTANCE hPrevInstance = nullptr;
		PSTR lpCmdLine = nullptr;
		int nCmdShow = 0;
	};

	using GlobalPlatformStateType = WindowsPlatformState;
#elif PLATFORM_OTHER()
	struct OtherPlatformState
	{
		int argc = 0;
		const char** argv = nullptr;
	};
	
	using GlobalPlatformStateType = OtherPlatformState;
#endif // PLATFORM
	extern GlobalPlatformStateType GlobalPlatformState;
}

#endif // ATOLLENGINE_H