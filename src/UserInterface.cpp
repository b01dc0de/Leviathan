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

bool bVisualizeInput = false;
bool bDrawPerfGraph = false;

void UserInterface::Draw(BatchDrawCmds& Draw2D, LvFont& InFont)
{
    if (KeyboardState::GetKey(LV_KEY_F1)) { bVisualizeInput = !bVisualizeInput; }
    if (KeyboardState::GetKey(LV_KEY_F2)) { bDrawPerfGraph = !bDrawPerfGraph; }

    if (bVisualizeInput)
    {
        InputVisualizer::DrawKeyboard(Draw2D);
        InputVisualizer::DrawMouse(Draw2D);
        InputVisualizer::DrawGamepads(Draw2D);
    }

    if (bDrawPerfGraph)
    {
        PerformanceGraph::Draw(Draw2D);
    }

    static constexpr bool bDrawTimeStats = true;
    if (bDrawTimeStats)
    {
        constexpr v2f Origin{ 1100.0f, 0.0f };
        constexpr int MsgBufferSize = 64;

        char MsgBuffer[MsgBufferSize];
        double CurrTime = Clock::Time();
        int MsgLength = sprintf_s(MsgBuffer, "CurrTime:%.02f", CurrTime);
        InFont.Draw(Draw2D, Origin, 1.5f, MsgBuffer, MsgLength);

        double DeltaTimeMs = Clock::DeltaTime() * 1000.0;
        MsgLength = sprintf_s(MsgBuffer, "Frame:%.02f ms", DeltaTimeMs);
        InFont.Draw(Draw2D, Origin + v2f{ 0.0f, 16.0f }, 1.5f, MsgBuffer, MsgLength);
    }
}

}

