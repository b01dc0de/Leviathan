#ifndef LEVIATHAN_CLOCK_H
#define LEVIATHAN_CLOCK_H

namespace Leviathan
{
    struct Clock
    {
        static double Time();
        static double DeltaTime();

        static void Tick();
        static void Init();
    };
}

#endif // LEVIATHAN_CLOCK_H