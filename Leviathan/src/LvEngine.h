#ifndef LVENGINE_H
#define LVENGINE_H

namespace Leviathan
{
	extern bool bLvRunning;

	extern HWND LvWindow;

	extern HINSTANCE LvInst;
	extern HINSTANCE LvPrevInst;
	extern PSTR LvCmdLine;
	extern int LvCmdShow;

	struct LvEngine
	{
		static void Init();
		static void Term();
		static void MainLoop();
	};
}

#endif // LVENGINE_H