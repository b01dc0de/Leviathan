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
        // D2D
        ID2D1SolidColorBrush* D2_WhiteBrush = nullptr;
        ID2D1SolidColorBrush* D2_BlackBrush = nullptr;

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

    bool bVisualizeInput = true;
    bool bDrawTimeStats = true;

    void UserInterface::Draw(ID2D1RenderTarget* In2DRT, BatchDrawCmds& Draw2D)
    {
        ASSERT(In2DRT);

        if (KeyboardState::GetKey(LV_KEY_F1)) { bVisualizeInput = !bVisualizeInput; }
        if (KeyboardState::GetKey(LV_KEY_F2)) { bDrawTimeStats = !bDrawTimeStats; }

        if (bVisualizeInput)
        {
            InputVisualizer::DrawKeyboard(Draw2D);
            InputVisualizer::DrawMouse(Draw2D);
            InputVisualizer::DrawGamepads(Draw2D);
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

            PerformanceGraph::Draw(Draw2D);
        }
    }

    void UserInterface::Term()
    {
        { // DirectWrite:
            SafeRelease(DW_Factory);
            SafeRelease(DW_DefaultTextFormat);
        }

        SafeRelease(D2_WhiteBrush);
        SafeRelease(D2_BlackBrush);
    }
}

