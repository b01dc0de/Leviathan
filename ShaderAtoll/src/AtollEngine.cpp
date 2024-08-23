#include "AtollEngine.h"
#include "AtollGraphics.h"
#if PLATFORM_WINDOWS()
	#include "Atoll_mswin.h"
#endif // PLATFORM

namespace ShaderAtoll
{
	bool bRunning = false;
	HWND hWindow;
	UINT WinResX = Defaults::WinResX;
	UINT WinResY = Defaults::WinResY;
	float AppTime_ms = 0.0f;
	float DeltaTime_ms = 0.0f;
	GlobalPlatformStateType GlobalPlatformState;

	enum AtollErrorType : int
	{
		NO_ERR,
		ERR_STARTUP,
		ERR_RUNTIME,
		ERR_SHUTDOWN,
	};

#if PLATFORM_WINDOWS()
	int AtollEngine::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
#elif PLATFORM_OTHER()
	int AtollEngine::Run(int argc, const char* argv[])
#else
	int AtollEngine::Run()
#endif // PLATFORM
	{
#if PLATFORM_WINDOWS()
		GlobalPlatformState.hInstance = hInstance;
		GlobalPlatformState.hPrevInstance = hPrevInstance;
		GlobalPlatformState.lpCmdLine = lpCmdLine;
		GlobalPlatformState.nCmdShow = nCmdShow;
#elif PLATFORM_OTHER()
		GlobalPlatformState.argc = argc;
		GlobalPlatformState.argv = argv;
#endif // PLATFORM

		if (!Init()) { return AtollErrorType::ERR_STARTUP; }

		if (!MainLoop()) { return AtollErrorType::ERR_RUNTIME; }

		if (!Term()) { return AtollErrorType::ERR_SHUTDOWN; }

		return AtollErrorType::NO_ERR;
	}

	bool AtollEngine::Init()
	{
		bool bResult = true;

		if (HWND hWnd = InitWindow(GlobalPlatformState.hInstance))
		{
			ShaderAtoll::hWindow = hWnd;

			HRESULT Result = AtollGraphics::InitGraphics();
			if (Result != S_OK)
			{
				DebugBreak();
			}

			ShowWindow(ShaderAtoll::hWindow, GlobalPlatformState.nCmdShow);

			ShaderAtoll::bRunning = true;
		}
		else
		{
			bResult = false;
		}

		return bResult;
	}

	bool AtollEngine::MainLoop()
	{
		bool bResult = true;

		while (ShaderAtoll::bRunning)
		{
			// Get input
			MSG msg;
			BOOL MsgResult;
			while ((MsgResult = PeekMessage(&msg, ShaderAtoll::hWindow, 0, 0, PM_REMOVE)) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (MsgResult == -1)
			{
				ShaderAtoll::bRunning = false;
				break;
			}

			// Update
			UpdateWindow(ShaderAtoll::hWindow);

			AtollGraphics::Draw();
		}

		return bResult;
	}

	bool AtollEngine::Term()
	{
		bool bResult = true;
		return bResult;
	}
}