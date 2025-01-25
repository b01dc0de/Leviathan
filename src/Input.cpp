#include "Leviathan.h" // TODO: Input.h included in Leviathan.h

namespace Leviathan
{
    int KeyboardState::ActiveKeys[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int KeyboardState::ActiveCount = 0;

    bool KeyboardState::GetKeyState(int VKCode)
    {
        for (int KeyIdx = 0; KeyIdx < ActiveCount; KeyIdx++)
        {
            if (ActiveKeys[KeyIdx] == VKCode)
            {
                return true;
            }
        }
        return false;
    }

    void KeyboardState::SetKeyUp(int VKCode)
    {
        if (ActiveCount == 0) { return; }

        bool bKeyFound = false;
        for (int KeyIdx = 0; KeyIdx < ActiveCount; KeyIdx++)
        {
            if (ActiveKeys[KeyIdx] == VKCode)
            {
                bKeyFound = true;
            }
            if (bKeyFound && KeyIdx < (ActiveCount - 1))
            {
                ActiveKeys[KeyIdx] = ActiveKeys[KeyIdx + 1];
            }
        }
        if (bKeyFound) { ActiveCount--; }
    }

    void KeyboardState::SetKeyDown(int VKCode)
    {
        if (ActiveCount == KeyCount) { return; }

        for (int KeyIdx = 0; KeyIdx < KeyCount; KeyIdx++)
        {
            if (ActiveKeys[KeyIdx] == VKCode)
            {
                return;
            }
            if (ActiveKeys[KeyIdx] == 0)
            {
                ActiveKeys[KeyIdx] = VKCode;
                ActiveCount++;
                return;
            }
        }
    }
}