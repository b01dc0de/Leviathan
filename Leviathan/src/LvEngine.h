#ifndef LVENGINE_H
#define LVENGINE_H

namespace Leviathan
{
	extern bool bLvRunning;

	extern HINSTANCE Lv_Inst;
	extern HINSTANCE Lv_PrevInst;
	extern PSTR Lv_CmdLine;
	extern int Lv_CmdShow;

	HWND Lv_GetWindowHandle();

	struct LvEngine
	{
		static void Init();
		static void Term();
		static void MainLoop();
	};
}

#endif // LVENGINE_H