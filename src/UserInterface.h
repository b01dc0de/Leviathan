#ifndef LEVIATHAN_USERINTERFACE_H
#define LEVIATHAN_USERINTERFACE_H

#include "Leviathan.h"

namespace Leviathan
{

struct UserInterface
{
    static bool bDrawUI;

    static void Init(ID2D1RenderTarget* In2DRT);
    static void Draw(ID2D1RenderTarget* In2DRT, BatchDrawCmds& Draw2D);
    static void Term();
};

}

#endif // LEVIATHAN_USERINTERFACE_H

