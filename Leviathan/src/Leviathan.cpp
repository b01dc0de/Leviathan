#include "LvCommon.h"

#include "Leviathan.h"
#include "LvEngine.h"

namespace Leviathan
{
	LRESULT CALLBACK LvWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT Result = 0;
		switch (uMsg)
		{
			case WM_QUIT:
			case WM_CLOSE:
			case WM_DESTROY:
			{
				Leviathan::bLvRunning = false;
			} break;
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_KEYDOWN:
			{
				Leviathan::bLvRunning = false;
			} break;
			default:
			{
				Result = DefWindowProc(hwnd, uMsg, wParam, lParam);
			} break;
		}
		return Result;
	}
} // namepace Leviathan

static int WINAPI WinMain(HINSTANCE Inst, HINSTANCE PrevInst, PSTR CmdLine, int CmdShow)
{
	using namespace Leviathan;

	Lv_Inst = Inst;
	Lv_PrevInst = PrevInst;
	Lv_CmdLine = CmdLine;
	Lv_CmdShow = CmdShow;

	int Result = 1;

	LvEngine::Init();
	LvEngine::MainLoop();
	LvEngine::Term();

	return Result;
}