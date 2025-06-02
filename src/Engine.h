#ifndef LEVIATHAN_ENGINE_H
#define LEVIATHAN_ENGINE_H

namespace Leviathan
{

struct Engine
{
    static void Init(HINSTANCE hInstance);
    static void MainLoop();
    static void Term();
};

extern unsigned int FrameNumber;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

} // namespace Leviathan

#endif // LEVIATHAN_ENGINE_H

