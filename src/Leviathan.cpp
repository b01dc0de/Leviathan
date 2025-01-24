#include "Leviathan.h"

namespace Leviathan
{
    int WinMain
    (
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR     lpCmdLine,
        int       nShowCmd
    )
    {
        OutputDebugStringA("LEVIATHAN -- INIT\n");
        return 0;
    }
}

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
