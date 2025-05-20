#include "UserInterface.h"
#include "Clock.h"
#include "Graphics.h"
#include "InputVisualizer.h"
#include "PerformanceGraph.h"

namespace Leviathan
{

bool UserInterface::bDrawUI = true;

namespace UserInterfaceState
{

}

using namespace UserInterfaceState;

bool bVisualizeInput = true;
bool bDrawTimeStats = true;

void UserInterface::Draw(BatchDrawCmds& Draw2D, LvFont& InFont)
{
    if (KeyboardState::GetKey(LV_KEY_F1)) { bVisualizeInput = !bVisualizeInput; }
    if (KeyboardState::GetKey(LV_KEY_F2)) { bDrawTimeStats = !bDrawTimeStats; }

    if (bVisualizeInput)
    {
        InputVisualizer::DrawKeyboard(Draw2D);
        InputVisualizer::DrawMouse(Draw2D);
        InputVisualizer::DrawGamepads(Draw2D);
    }

    if (bDrawTimeStats)
    {
        static constexpr int MsgBufferSize = 64;
        char MsgBuffer[MsgBufferSize];
        double CurrTime = Clock::Time();
        int MsgLength = sprintf_s(MsgBuffer, "CurrTime:%.02f", CurrTime);
        constexpr v2f Origin{ 1100.0f, 720.0f * 0.46f };
        InFont.Draw(Draw2D, Origin, 1.5f, MsgBuffer, MsgLength);
        PerformanceGraph::Draw(Draw2D);
    }
}

}

