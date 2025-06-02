#include "Leviathan.h"

namespace Leviathan
{

int AppWidth = 1280;
int AppHeight = 720;
float AspectRatio = (float)AppWidth / (float)AppHeight;
bool bAppRunning = true;
HWND AppWindow = nullptr;

} // namespace Leviathan

int WinMain
(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{
    return Leviathan::WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

