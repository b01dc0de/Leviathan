#include "UserInterface.h"
#include "Clock.h"
#include "Graphics.h"
#include "InputVisualizer.h"

namespace Leviathan
{
    bool UserInterface::bDrawUI = true;

    namespace UserInterfaceState
    {
        // D2D
        ID2D1SolidColorBrush* D2_WhiteBrush = nullptr;
        ID2D1SolidColorBrush* D2_BlackBrush = nullptr;
        ID2D1SolidColorBrush* D2_RedBrush = nullptr;
        ID2D1SolidColorBrush* D2_GreenBrush = nullptr;
        ID2D1SolidColorBrush* D2_BlueBrush = nullptr;
        ID2D1SolidColorBrush* D2_CyanBrush = nullptr;
        ID2D1SolidColorBrush* D2_MagentaBrush = nullptr;
        ID2D1SolidColorBrush* D2_YellowBrush = nullptr;

        // DirectWrite
        IDWriteFactory* DW_Factory = nullptr;
        IDWriteTextFormat* DW_DefaultTextFormat = nullptr;
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

        { // DirectWrite
            DX_CHECK(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&DW_Factory));
            DX_CHECK(DW_Factory->CreateTextFormat(
                L"Consolas",
                NULL,
                DWRITE_FONT_WEIGHT_REGULAR,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                16.0f,
                L"en-us",
                &DW_DefaultTextFormat
            ));
            DW_DefaultTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            DW_DefaultTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        }
    }

    bool bVisualizeInput = false;
    bool bDrawTimeStats = true;

    void UserInterface::Draw(ID2D1RenderTarget* In2DRT)
    {
        ASSERT(In2DRT);

        //if (KeyboardState::GetKeyState(LV_KEY_F1)) { bVisualizeInput = !bVisualizeInput; }
        //if (KeyboardState::GetKeyState(LV_KEY_F2)) { bDrawTimeStats = !bDrawTimeStats; }

        if (bVisualizeInput)
        {
            In2DRT->BeginDraw();

            if (bVisualizeInput)
            {
                InputVisualizer::DrawKeyboard(In2DRT, D2_WhiteBrush, D2_BlackBrush);
                InputVisualizer::DrawMouse(In2DRT, D2_WhiteBrush, D2_BlackBrush);
                InputVisualizer::DrawGamepad(In2DRT, D2_WhiteBrush, D2_BlackBrush);
            }

            DX_CHECK(In2DRT->EndDraw());
        }

        // DirectWrite Test
        if (bDrawTimeStats)
        {
            In2DRT->BeginDraw();

            static constexpr int MsgBufferSize = 64;
            wchar_t MsgBuffer[MsgBufferSize];
            double CurrTime = Clock::Time();
            swprintf_s(MsgBuffer, MsgBufferSize, L"CurrTime: %.02f", CurrTime);
            size_t MsgLength = wcslen(MsgBuffer);

            static bool bDrawShadow = true;
            if (bDrawShadow)
            {
                D2D1_RECT_F ShadowTextLayoutRect{ 0.5f, 0.5f, 200.0f, 100.0f };
                In2DRT->DrawText
                (
                    MsgBuffer,
                    MsgLength,
                    DW_DefaultTextFormat,
                    &ShadowTextLayoutRect,
                    D2_BlackBrush
                );
            }
            D2D1_RECT_F TextLayoutRect { 0.0f, 0.0f, 200.0f, 100.0f };
            In2DRT->DrawText
            (
                MsgBuffer,
                MsgLength,
                DW_DefaultTextFormat,
                &TextLayoutRect,
                D2_WhiteBrush
            );

            DX_CHECK(In2DRT->EndDraw());
        }
    }

    void UserInterface::Term()
    {
        { // DirectWrite:
            DX_SAFE_RELEASE(DW_Factory);
            DX_SAFE_RELEASE(DW_DefaultTextFormat);
        }

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