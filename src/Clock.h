#ifndef LEVIATHAN_CLOCK_H
#define LEVIATHAN_CLOCK_H

#include "Leviathan.h"

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