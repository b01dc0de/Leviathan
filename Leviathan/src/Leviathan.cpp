#include "LvCommon.h"

#include "Leviathan.h"
#include "LvEngine.h"

namespace Leviathan
{
#if LV_PLATFORM_WINDOWS()
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
			case WM_LBUTTONUP:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_INPUT:
			{
			} break;
			default:
			{
				Result = DefWindowProc(hwnd, uMsg, wParam, lParam);
			} break;
		}
		return Result;
	}
#endif // LV_PLATFORM_WINDOWS
} // namepace Leviathan

#if LV_PLATFORM_WINDOWS()
int WINAPI WinMain(HINSTANCE Inst, HINSTANCE PrevInst, PSTR CmdLine, int CmdShow)
#else // LV_PLATFORM_LINUX()
int main(int argc, const char* argv[])
#endif // LV_PLATFORM
{
	using namespace Leviathan;

#if LV_PLATFORM_WINDOWS()
	Lv_Inst = Inst;
	Lv_PrevInst = PrevInst;
	Lv_CmdLine = CmdLine;
	Lv_CmdShow = CmdShow;
#endif // LV_PLATFORM_WINDOWS

	int Result = 1;
	LvEngine::Init();
	LvEngine::MainLoop();
	LvEngine::Term();
	return Result;
}