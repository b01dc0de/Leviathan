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
	double AppTime_s = 0.0f;
	double DeltaTime_ms = 0.0f;

	GlobalPlatformStateType GlobalPlatformState;

	// Local/'private' engine functions
	namespace AtollEngine
	{
		AtollClock Clock;
		bool bMaximized = false;

		bool Init();
		bool MainLoop();
		bool Term();

		void PollInput();
		void UpdateState();

		void ToggleMaximize();
	}

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

		if (!Init()) { return (int)AtollErrorType::ERR_STARTUP; }

		if (!MainLoop()) { return (int)AtollErrorType::ERR_RUNTIME; }

		if (!Term()) { return (int)AtollErrorType::ERR_SHUTDOWN; }

		return (int)AtollErrorType::NO_ERR;
	}

	void AtollEngine::HandleKeyInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (WM_KEYUP == uMsg)
		{
			if (VK_ESCAPE == wParam)
			{
				ShaderAtoll::bRunning = false;
			}
			else if (VK_F11 == wParam)
			{
				ToggleMaximize();
			}
		}
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

			Clock.Init();

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
			// Update
			PollInput();
			UpdateState();

			// Draw
			AtollGraphics::Draw();
		}

		return bResult;
	}

	bool AtollEngine::Term()
	{
		bool bResult = true;
		return bResult;
	}

	void AtollEngine::PollInput()
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
		}
	}

	void AtollEngine::UpdateState()
	{
		Clock.Tick();
		AppTime_s = Clock.fLast_s;
		DeltaTime_ms = Clock.fDelta_ms;

		UpdateWindow(ShaderAtoll::hWindow);
	}

	void AtollEngine::ToggleMaximize()
	{
		bMaximized = !bMaximized;
		if (bMaximized)
		{
			WinResX = GlobalSysInfo.PrimaryWorkAreaX;
			WinResY = GlobalSysInfo.PrimaryWorkAreaY;
		}
		else
		{
			WinResX = Defaults::WinResX;
			WinResY = Defaults::WinResY;
		}
		UINT WndFlags = 0;
		SetWindowPos(ShaderAtoll::hWindow, HWND_TOP, 0, 0, WinResX, WinResY, WndFlags);
	}
}