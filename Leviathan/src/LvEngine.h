#ifndef LVENGINE_H
#define LVENGINE_H

namespace Leviathan
{
	extern bool bLvRunning;

#if LV_PLATFORM_WINDOWS()
	extern HINSTANCE Lv_Inst;
	extern HINSTANCE Lv_PrevInst;
	extern PSTR Lv_CmdLine;
	extern int Lv_CmdShow;

	HWND Lv_GetWindowHandle();
#endif // LV_PLATFORM_WINDOWS()

	struct LvEngine
	{
		static void Init();
		static void Term();
		static void MainLoop();
	};

#define LVINPUT_ENABLE_AGNOSTIC_STATE() (0)
}

#endif // LVENGINE_H