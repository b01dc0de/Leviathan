#ifndef LEVIATHAN_GRAPHICS_H
#define LEVIATHAN_GRAPHICS_H

#include "Leviathan.h"

namespace Leviathan
{
    struct Graphics
    {
        static void Init();
        static void UpdateAndDraw();
        static void Term();
    };
}

#endif // LEVIATHAN_GRAPHICS_H