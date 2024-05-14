#ifndef LEVIATHAN_H
#define LEVIATHAN_H

namespace Leviathan
{
#if LV_PLATFORM_WINDOWS()
	LRESULT CALLBACK LvWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif // LV_PLATFORM_WINDOWS
}

#endif // LEVIATHAN_H
