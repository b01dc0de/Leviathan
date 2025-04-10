#ifndef LEVIATHAN_USERINTERFACE_H
#define LEVIATHAN_USERINTERFACE_H

#include "Leviathan.h"

namespace Leviathan
{

struct UserInterface
{
    static bool bDrawUI;
    static void Draw(BatchDrawCmds& Draw2D, LvFont& InFont);
};

}

#endif // LEVIATHAN_USERINTERFACE_H

