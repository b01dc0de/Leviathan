#include "UserInterface.h"
#include "Graphics.h"
#include "InputVisualizer.h"

namespace Leviathan
{
    bool UserInterface::bDrawUI = true;

    namespace UserInterfaceState
    {
        ID2D1SolidColorBrush* D2_WhiteBrush = nullptr;
        ID2D1SolidColorBrush* D2_BlackBrush = nullptr;
        ID2D1SolidColorBrush* D2_RedBrush = nullptr;
        ID2D1SolidColorBrush* D2_GreenBrush = nullptr;
        ID2D1SolidColorBrush* D2_BlueBrush = nullptr;
        ID2D1SolidColorBrush* D2_CyanBrush = nullptr;
        ID2D1SolidColorBrush* D2_MagentaBrush = nullptr;
        ID2D1SolidColorBrush* D2_YellowBrush = nullptr;
    }

    using namespace UserInterfaceState;

    void UserInterface::Init(ID2D1RenderTarget* In2DRT)
    {
        ASSERT(In2DRT);
        DX_CHECK(In2DRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &D2_WhiteBrush));
        DX_CHECK(In2DRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &D2_BlackBrush));
        DX_CHECK(In2DRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &D2_RedBrush));
        DX_CHECK(In2DRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &D2_GreenBrush));
        DX_CHECK(In2DRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &D2_BlueBrush));
        DX_CHECK(In2DRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Cyan), &D2_CyanBrush));
        DX_CHECK(In2DRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Magenta), &D2_MagentaBrush));
        DX_CHECK(In2DRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &D2_YellowBrush));
    }

    bool bVisualizeKeyboard = true;
    bool bVisualizeMouse = true;
    bool bVisualizeGamepad = true;

    void UserInterface::Draw(ID2D1RenderTarget* In2DRT)
    {
        ASSERT(In2DRT);

        if (KeyboardState::GetKeyState(LV_KEY_F1)) { bVisualizeKeyboard = !bVisualizeKeyboard; }
        if (KeyboardState::GetKeyState(LV_KEY_F2)) { bVisualizeMouse = !bVisualizeMouse; }
        if (KeyboardState::GetKeyState(LV_KEY_F3)) { bVisualizeGamepad = !bVisualizeGamepad; }

        static bool bD2Foreground = true;
        if (bVisualizeKeyboard || bVisualizeMouse || bVisualizeGamepad)
        {
            In2DRT->BeginDraw();

            if (bVisualizeKeyboard) { InputVisualizer::DrawKeyboard(In2DRT, D2_WhiteBrush, D2_BlackBrush); }
            if (bVisualizeMouse) { InputVisualizer::DrawMouse(In2DRT, D2_WhiteBrush, D2_BlackBrush); }
            if (bVisualizeGamepad) { InputVisualizer::DrawGamepad(In2DRT, D2_WhiteBrush, D2_BlackBrush); }

            DX_CHECK(In2DRT->EndDraw());
        }
    }

    void UserInterface::Term()
    {
        DX_SAFE_RELEASE(D2_WhiteBrush);
        DX_SAFE_RELEASE(D2_BlackBrush);
        DX_SAFE_RELEASE(D2_RedBrush);
        DX_SAFE_RELEASE(D2_GreenBrush);
        DX_SAFE_RELEASE(D2_BlueBrush);
        DX_SAFE_RELEASE(D2_CyanBrush);
        DX_SAFE_RELEASE(D2_MagentaBrush);
        DX_SAFE_RELEASE(D2_YellowBrush);
    }
}