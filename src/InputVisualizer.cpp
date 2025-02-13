#include "InputVisualizer.h"

namespace Leviathan
{
    struct VisualKey
    {
        LvKeyCode LvCode = LV_KEY_NONE;
        v2f Size{ 1.0f, 1.0f };
        v2f Pos{ 0.0f, 0.0f };
    };

    struct VisualKeyboard
    {
        bool bInit = false;
        VisualKey KeyList[LV_KEY_COUNT];

        void Init();
        static void DrawKey(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush, const VisualKey& Key, const v2f& Origin, float Scale);
        static void Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2, const v2f& Origin, float Scale);

        static void DrawKey(BatchDraw2D& Draw2D, const VisualKey& Key, const v2f& Origin, float Scale);
        static void Draw(BatchDraw2D& Draw2D, const v2f& Origin, float Scale);
    };

    void VisualKeyboard::Init()
    {
        for (int KeyIdx = LV_KEY_NONE; KeyIdx < LV_KEY_COUNT; KeyIdx++) { KeyList[KeyIdx].LvCode = (LvKeyCode)KeyIdx; }
        KeyList[LV_KEY_BACKSPACE].Size.X = 2.0f;
        KeyList[LV_KEY_TAB].Size.X = 1.5f;
        KeyList[LV_KEY_BACKSLASH].Size.X = 1.5f;
        KeyList[LV_KEY_CAPSLOCK].Size.X = 1.75f;
        KeyList[LV_KEY_ENTER].Size.X = 2.25f;
        KeyList[LV_KEY_SHIFT].Size.X = 2.25f;
        KeyList[LV_KEY_CTRL].Size.X = 1.25f;
        KeyList[LV_KEY_SUPER].Size.X = 1.25f;
        KeyList[LV_KEY_ALT].Size.X = 1.25f;
        KeyList[LV_KEY_SPACE].Size.X = 6.0f;

        float FuncRowGap = 2.0f / 3.0f; // RowWidth = 15, NumRowKeys = 12, NumGaps = 3, FuncGap = (RowWidth - NumRowKeys) / NumGaps
        float RowX = 1.0f + FuncRowGap;
        float FuncRowPadding = 0.25f;

        // Setup function row:
        for (int KeyIdx = LV_KEY_F1; KeyIdx <= LV_KEY_F12; KeyIdx++)
        {
            KeyList[KeyIdx].Pos = v2f{ RowX, 0.0f };
            RowX += KeyList[KeyIdx].Size.X;
            if (KeyIdx == LV_KEY_F4 || KeyIdx == LV_KEY_F8) { RowX += FuncRowGap; }
        }

        // Setup rest of 'main' keyboard:
        float RowY = 1.0f + FuncRowPadding;
        RowX = 0.0f;
        v2f RightRegionOrigin{ 15.0f, 0.0f };
        for (int KeyIdx = LV_KEY_GRAVE; KeyIdx <= LV_KEY_SLASH; KeyIdx++)
        {
            KeyList[KeyIdx].Pos = v2f{ RowX, RowY };
            RowX += KeyList[KeyIdx].Size.X;
            switch (KeyIdx)
            {
                case LV_KEY_ENTER:
                {
                    RowY += 1.0f;
                    RowX = KeyList[LV_KEY_SHIFT].Size.X;
                } break;
                case LV_KEY_BACKSPACE:
                case LV_KEY_BACKSLASH:
                case LV_KEY_SLASH:
                {
                    RowY += 1.0f;
                    RowX = 0.0f;
                } break;
            }
        }
        KeyList[LV_KEY_SPACE].Pos = v2f{ 3.75f, RowY };

        v2f ModOrigin = RightRegionOrigin + v2f{ 1.0f, 0.0f };
        KeyList[LV_KEY_SHIFT].Pos = v2f{ 0.0f, RowY - 1.0f };
        KeyList[LV_KEY_CTRL].Pos = v2f{ 0.0f, RowY };
        KeyList[LV_KEY_SUPER].Pos = KeyList[LV_KEY_CTRL].Pos + v2f{ KeyList[LV_KEY_CTRL].Size.X };
        KeyList[LV_KEY_ALT].Pos = KeyList[LV_KEY_SUPER].Pos + v2f{ KeyList[LV_KEY_SUPER].Size.X };
        KeyList[LV_KEY_ARROW_UP].Pos = RightRegionOrigin + v2f{ -2.0f, RowY - 1.0f };
        KeyList[LV_KEY_ARROW_LEFT].Pos = RightRegionOrigin + v2f{ -3.0f, RowY };
        KeyList[LV_KEY_ARROW_DOWN].Pos = RightRegionOrigin + v2f{ -2.0f, RowY };
        KeyList[LV_KEY_ARROW_RIGHT].Pos = RightRegionOrigin + v2f{ -1.0f, RowY };

        // Use LV_KEY_NONE for background
        KeyList[LV_KEY_NONE].Size = { 15.0f, RowY + 1.0f };

        bInit = true;
    }

    void VisualKeyboard::DrawKey(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush, const VisualKey& Key, const v2f& Origin, float Scale)
    {
        float Left = Origin.X + (Key.Pos.X * Scale);
        float Top = Origin.Y + (Key.Pos.Y * Scale);
        float Right = Left + (Key.Size.X * Scale);
        float Bottom = Top + (Key.Size.Y * Scale);
        D2D1_RECT_F KeyRect{ Left, Top, Right, Bottom };
        bool bIsDown = KeyboardState::GetKeyState(Key.LvCode, true);
        if (bIsDown) { InD2RT->FillRectangle(&KeyRect, InBrush); }
        else { InD2RT->DrawRectangle(&KeyRect, InBrush, InputVisualizer::LineWidth, nullptr); }
    }

    void VisualKeyboard::Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2, const v2f& Origin, float Scale)
    {
        static VisualKeyboard vKeyboard;
        if (!vKeyboard.bInit) { vKeyboard.Init(); }
        static constexpr bool bDrawBG = true;
        if (bDrawBG)
        {
            D2D1_RECT_F BGRect
            {
                Origin.X,
                Origin.Y,
                Origin.X + vKeyboard.KeyList[LV_KEY_NONE].Size.X * Scale,
                Origin.Y + vKeyboard.KeyList[LV_KEY_NONE].Size.Y * Scale
            };
            InD2RT->FillRectangle(&BGRect, InBrush2);
            InD2RT->DrawRectangle(&BGRect, InBrush1, InputVisualizer::LineWidth);
        }
        for (int KeyIdx = LV_KEY_ESC; KeyIdx < LV_KEY_COUNT; KeyIdx++)
        {
            DrawKey(InD2RT, InBrush1, vKeyboard.KeyList[KeyIdx], Origin, Scale);
        }
    }

    void VisualKeyboard::DrawKey(BatchDraw2D& Draw2D, const VisualKey& Key, const v2f& Origin, float Scale)
    {
        float SizeX = (Key.Size.X * Scale);
        float SizeY = (Key.Size.Y * Scale);
        float PosX = Origin.X + (Key.Pos.X * Scale);
        float PosY = Origin.Y - (Key.Pos.Y * Scale) - SizeY;
        QuadF KeyQuad = { PosX, PosY, SizeX, SizeY };
        bool bIsDown = KeyboardState::GetKeyState(Key.LvCode, true);
        RGBA ColorWhite{ 1.0f, 1.0f, 1.0f, 1.0f };
        if (bIsDown)
        {
            //KeyQuad.PosY -= SizeY;
            Draw2D.AddQuad(KeyQuad, ColorWhite);
        }
        else
        {
            Draw2D.AddBox(KeyQuad, ColorWhite, InputVisualizer::LineWidth);
        }
    }

    void VisualKeyboard::Draw(BatchDraw2D& Draw2D, const v2f& Origin, float Scale)
    {
        static VisualKeyboard vKeyboard;
        if (!vKeyboard.bInit) { vKeyboard.Init(); }
        /*
        static bool bDrawBG = true;
        if (bDrawBG)
        {
            D2D1_RECT_F BGRect
            {
                Origin.X,
                Origin.Y,
                Origin.X + vKeyboard.KeyList[LV_KEY_NONE].Size.X * Scale,
                Origin.Y + vKeyboard.KeyList[LV_KEY_NONE].Size.Y * Scale
            };
            InD2RT->FillRectangle(&BGRect, InBrush2);
            InD2RT->DrawRectangle(&BGRect, InBrush1, InputVisualizer::LineWidth);
        }
        */
        for (int KeyIdx = LV_KEY_ESC; KeyIdx < LV_KEY_COUNT; KeyIdx++)
        {
            DrawKey(Draw2D, vKeyboard.KeyList[KeyIdx], Origin, Scale);
        }
    }

    void InputVisualizer::DrawKeyboard(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2)
    {
        v2f Origin{975.0f, 10.0};
        float Scale = 20.0f;
        VisualKeyboard::Draw(InD2RT, InBrush1, InBrush2, Origin, Scale);
    }

    void InputVisualizer::DrawKeyboard(BatchDraw2D& Draw2D)
    {
        v2f Origin{975.0f, (float)AppHeight - 10.0f};
        float Scale = 20.0f;
        VisualKeyboard::Draw(Draw2D, Origin, Scale);
    }

    struct VisualMouse
    {
        static void Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2, const v2f& Origin, float Scale);
    };

    void VisualMouse::Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2, const v2f& Origin, float Scale)
    {
        static constexpr v2f RegionSize{ 16.0f, 9.0f * 2.0f };
        static constexpr v2f ButtonSize{ RegionSize.X / 3.0f, RegionSize.Y / 2.0f };
        static constexpr float WheelSize = RegionSize.X / 6.0f;
        static constexpr v2f WheelPos = { RegionSize.X / 2.0f, WheelSize };

        static constexpr bool bDrawBG = true;
        if (bDrawBG)
        {
            D2D1_RECT_F RegionBGRect
            {
                Origin.X,
                Origin.Y,
                Origin.X + RegionSize.X * Scale,
                Origin.Y + RegionSize.Y * Scale
            };
            InD2RT->FillRectangle(&RegionBGRect, InBrush2);
            InD2RT->DrawRectangle(&RegionBGRect, InBrush1, InputVisualizer::LineWidth);
        }

        const v2f AdjButtonSize{ ButtonSize.X * Scale, ButtonSize.Y * Scale };
        D2D1_POINT_2F AdjWheelPos{ WheelPos.X * Scale + Origin.X, WheelPos.Y * Scale + Origin.Y };
        D2D1_RECT_F LeftRect{ Origin.X, Origin.Y, Origin.X + AdjButtonSize.X, Origin.Y + AdjButtonSize.Y };
        D2D1_RECT_F MiddleRect{ LeftRect.left + AdjButtonSize.X, AdjWheelPos.y + WheelSize * Scale, LeftRect.right + AdjButtonSize.X, LeftRect.bottom };
        D2D1_RECT_F RightRect{ MiddleRect.right, LeftRect.top, MiddleRect.right + AdjButtonSize.X, LeftRect.bottom };
        { // Buttons
            if (MouseState::bLeftKey) { InD2RT->FillRectangle(&LeftRect, InBrush1); }
            else { InD2RT->DrawRectangle(&LeftRect, InBrush1, InputVisualizer::LineWidth); }
            if (MouseState::bRightKey) { InD2RT->FillRectangle(&RightRect, InBrush1); }
            else { InD2RT->DrawRectangle(&RightRect, InBrush1, InputVisualizer::LineWidth); }
            if (MouseState::bMiddleKey) { InD2RT->FillRectangle(&MiddleRect, InBrush1); }
            else { InD2RT->DrawRectangle(&MiddleRect, InBrush1, InputVisualizer::LineWidth); }
        }

        static constexpr v2f MouseAreaSize{ RegionSize.X, RegionSize.Y / 2.0f };
        static constexpr v2f MouseAreaPos{ 0.0f, RegionSize.Y / 2.0f };
        D2D1_RECT_F MouseWindowRect
        {
            MouseAreaPos.X + Origin.X,
            MouseAreaPos.Y * Scale + Origin.Y,
            MouseAreaPos.X + MouseAreaSize.X * Scale + Origin.X,
            (MouseAreaPos.Y + MouseAreaSize.Y) * Scale + Origin.Y
        };
        float MouseWindowAdjWidth = MouseWindowRect.right - MouseWindowRect.left;
        float MouseWindowAdjHeight = MouseWindowRect.bottom - MouseWindowRect.top;
        static float HalfCursorSize = Scale * 0.5f;
        { // Cursor
            v2f CursorPos =
            {
                ((float)MouseState::MouseX / (float)AppWidth * MouseWindowAdjWidth) + MouseWindowRect.left,
                ((float)MouseState::MouseY / (float)AppHeight * MouseWindowAdjHeight) + MouseWindowRect.top
            };
            D2D1_RECT_F CursorRect
            {
                CursorPos.X - HalfCursorSize,
                CursorPos.Y - HalfCursorSize,
                CursorPos.X + HalfCursorSize,
                CursorPos.Y + HalfCursorSize
            };
            InD2RT->FillRectangle(&MouseWindowRect, InBrush1);
            InD2RT->FillRectangle(&CursorRect, InBrush2);
        }

        static float WheelAngle = 0.0f;
        static float VisualSpeed = 0.25f;
        WheelAngle += MouseState::MouseWheel * VisualSpeed;
        { // Wheel
            D2D1_ELLIPSE WheelEllipse{ AdjWheelPos, WheelSize*Scale, WheelSize*Scale };
            InD2RT->DrawEllipse(WheelEllipse, InBrush1, InputVisualizer::LineWidth);
            D2D1_POINT_2F WheelLineEnd =
            {
                AdjWheelPos.x + (cosf(WheelAngle) * WheelSize * Scale),
                AdjWheelPos.y + (sinf(WheelAngle) * WheelSize * Scale),
            };
            InD2RT->DrawLine(AdjWheelPos, WheelLineEnd, InBrush1, InputVisualizer::LineWidth);
        }
    }

    void InputVisualizer::DrawMouse(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2)
    {
        static v2f Origin{ 1000.0f, 200.0f };
        static float Scale = 5.0f;
        VisualMouse::Draw(InD2RT, InBrush1, InBrush2, Origin, Scale);
    }

    struct VisualGamepad
    {
        static void Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2, const v2f& Origin, float Scale);
    };

    void VisualGamepad::Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2, const v2f& Origin, float Scale)
    {
        static constexpr v2f GamepadRegionSize{ 16.0f, 16.0f };
        static constexpr v2f DPadButtonsCenter{ 0.25f, 0.5f };
        static constexpr v2f FaceButtonsCenter{ 0.75f, 0.5f };
        static constexpr float FaceButtonsSize{ 0.05f };
        static constexpr v2f ControlButtonsSize{ FaceButtonsSize * 1.5f, FaceButtonsSize };
        static constexpr v2f MenuButtonsCenter{ 0.5f, 0.25f + ControlButtonsSize.Y };
        static constexpr float ButtonsOffset{ FaceButtonsSize * 2.0f };
        static constexpr v2f TriggerButtonsSize{ 0.5f, 0.2f };
        static constexpr v2f ThumbStickCenter = { 0.5f, 0.75f };
        static constexpr float ThumbStickSize = { 0.125f };
        static constexpr v2f ButtonPos[LV_GAMEPAD_BUTTON_COUNT] =
        {
            {DPadButtonsCenter.X, DPadButtonsCenter.Y - ButtonsOffset},
            {DPadButtonsCenter.X, DPadButtonsCenter.Y + ButtonsOffset},
            {DPadButtonsCenter.X - ButtonsOffset, DPadButtonsCenter.Y},
            {DPadButtonsCenter.X + ButtonsOffset, DPadButtonsCenter.Y},
            {FaceButtonsCenter.X, FaceButtonsCenter.Y - ButtonsOffset},
            {FaceButtonsCenter.X, FaceButtonsCenter.Y + ButtonsOffset},
            {FaceButtonsCenter.X - ButtonsOffset, FaceButtonsCenter.Y},
            {FaceButtonsCenter.X + ButtonsOffset, FaceButtonsCenter.Y},
            {MenuButtonsCenter.X + ControlButtonsSize.X, MenuButtonsCenter.Y},
            {MenuButtonsCenter.X - ControlButtonsSize.X, MenuButtonsCenter.Y},
            {DPadButtonsCenter.X - ControlButtonsSize.X * 2.0f, MenuButtonsCenter.Y},
            {FaceButtonsCenter.X + ControlButtonsSize.X * 2.0f, MenuButtonsCenter.Y},
            {},//LV_GAMEPAD_LEFT_THUMB,
            {},//LV_GAMEPAD_RIGHT_THUMB,
        };

        v2f AdjRegionSize{ GamepadRegionSize.X * Scale, GamepadRegionSize.Y * Scale };

        static constexpr bool bDrawBG = true;
        if (bDrawBG)
        {
            D2D1_RECT_F BGRect = {
                Origin.X, Origin.Y,
                Origin.X + AdjRegionSize.X, Origin.Y + AdjRegionSize.Y
            };
            InD2RT->FillRectangle(&BGRect, InBrush2);
            InD2RT->DrawRectangle(&BGRect, InBrush1);
        }

        v2f AdjDPadOrigin{ DPadButtonsCenter.X * AdjRegionSize.X + Origin.X, DPadButtonsCenter.Y * AdjRegionSize.Y + Origin.Y };
        v2f AdjFaceOrigin{ FaceButtonsCenter.X * AdjRegionSize.X + Origin.X, FaceButtonsCenter.Y * AdjRegionSize.Y + Origin.Y };
        float AdjFaceButtonsSize = FaceButtonsSize * AdjRegionSize.X;
        float AdjButtonsOffset = ButtonsOffset * AdjRegionSize.X;
        for (int ButtonIdx = LV_GAMEPAD_DPAD_UP; ButtonIdx < LV_GAMEPAD_START; ButtonIdx++)
        {
            D2D1_ELLIPSE ButtonEllipse
            {
                ButtonPos[ButtonIdx].X * GamepadRegionSize.X * Scale + Origin.X,
                ButtonPos[ButtonIdx].Y * GamepadRegionSize.Y * Scale + Origin.Y,
                AdjFaceButtonsSize, AdjFaceButtonsSize
            };
            if (GamepadState::GetButton((LvGamepadButton)ButtonIdx)) { InD2RT->FillEllipse(&ButtonEllipse, InBrush1); }
            else { InD2RT->DrawEllipse(&ButtonEllipse, InBrush1, InputVisualizer::LineWidth); }
        }
        for (int ButtonIdx = LV_GAMEPAD_START; ButtonIdx < LV_GAMEPAD_LEFT_THUMB; ButtonIdx++)
        {
            D2D1_RECT_F ButtonRect
            {
                (ButtonPos[ButtonIdx].X - ControlButtonsSize.X) * GamepadRegionSize.X * Scale + Origin.X,
                (ButtonPos[ButtonIdx].Y - ControlButtonsSize.Y) * GamepadRegionSize.Y * Scale + Origin.Y,
                (ButtonPos[ButtonIdx].X + ControlButtonsSize.X) * GamepadRegionSize.X * Scale + Origin.X,
                (ButtonPos[ButtonIdx].Y + ControlButtonsSize.Y) * GamepadRegionSize.X * Scale + Origin.Y,
            };
            if (GamepadState::GetButton((LvGamepadButton)ButtonIdx)) { InD2RT->FillRectangle(&ButtonRect, InBrush1); }
            else { InD2RT->DrawRectangle(&ButtonRect, InBrush1, InputVisualizer::LineWidth); }
        }

        v2f AdjTriggerSize{ TriggerButtonsSize.X * AdjRegionSize.X, TriggerButtonsSize.Y * AdjRegionSize.Y };
        float LTrigger = GamepadState::GetLeftTrigger();
        float RTrigger = GamepadState::GetRightTrigger();
        { // Triggers
            static constexpr float TriggerDeadzone = 0.05f;
            D2D1_RECT_F LeftTriggerRect{ Origin.X, Origin.Y,
                Origin.X + AdjTriggerSize.X, Origin.Y + AdjTriggerSize.Y };
            D2D1_RECT_F RightTriggerRect{ LeftTriggerRect.right, Origin.Y,
                Origin.X + AdjRegionSize.X, Origin.Y + AdjTriggerSize.Y };
            InD2RT->DrawRectangle(&LeftTriggerRect, InBrush1, InputVisualizer::LineWidth);
            InD2RT->DrawRectangle(&RightTriggerRect, InBrush1, InputVisualizer::LineWidth);
            if (LTrigger > TriggerDeadzone)
            {
                LeftTriggerRect.right = LeftTriggerRect.left + (AdjTriggerSize.X * LTrigger);
                InD2RT->FillRectangle(&LeftTriggerRect, InBrush1);
            }
            if (RTrigger > TriggerDeadzone)
            {
                RightTriggerRect.left = RightTriggerRect.right - (AdjTriggerSize.X * RTrigger);
                InD2RT->FillRectangle(&RightTriggerRect, InBrush1);
            }
        }

        v2f LStick = GamepadState::GetLeftStick();
        v2f RStick = GamepadState::GetRightStick();
        { // Sticks
            float AdjStickSize = ThumbStickSize * AdjRegionSize.X;
            const D2D1_POINT_2F LStickPos{ Origin.X + (DPadButtonsCenter.X * AdjRegionSize.X), Origin.Y + (ThumbStickCenter.Y * AdjRegionSize.Y) + AdjStickSize / 2.0f};
            const D2D1_POINT_2F RStickPos{ Origin.X + (FaceButtonsCenter.X * AdjRegionSize.X), Origin.Y + (ThumbStickCenter.Y * AdjRegionSize.Y) + AdjStickSize / 2.0f};
            D2D1_ELLIPSE LeftEllipse{ LStickPos, AdjStickSize, AdjStickSize };
            D2D1_ELLIPSE RightEllipse{ RStickPos, AdjStickSize, AdjStickSize };

            ID2D1Brush* LeftLineBrush = InBrush1;
            ID2D1Brush* RightLineBrush = InBrush1;
            if (GamepadState::GetButton(LV_GAMEPAD_LEFT_THUMB)) { InD2RT->FillEllipse(&LeftEllipse, InBrush1); LeftLineBrush = InBrush2; }
            else { InD2RT->DrawEllipse(&LeftEllipse, InBrush1, InputVisualizer::LineWidth); }
            if (GamepadState::GetButton(LV_GAMEPAD_RIGHT_THUMB)) { InD2RT->FillEllipse(&RightEllipse, InBrush1); RightLineBrush = InBrush2; }
            else { InD2RT->DrawEllipse(&RightEllipse, InBrush1, InputVisualizer::LineWidth); }

            D2D1_POINT_2F LStickInput
            {
                LStickPos.x + LStick.X * AdjStickSize,
                LStickPos.y + -LStick.Y * AdjStickSize
            };
            D2D1_POINT_2F RStickInput
            {
                RStickPos.x + RStick.X * AdjStickSize,
                RStickPos.y + -RStick.Y * AdjStickSize
            };
            InD2RT->DrawLine(LStickPos, LStickInput, LeftLineBrush, InputVisualizer::LineWidth);
            InD2RT->DrawLine(RStickPos, RStickInput, RightLineBrush, InputVisualizer::LineWidth);
        }
    }

    void InputVisualizer::DrawGamepad(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2)
    {
        static const v2f Origin{ 1100.0f, 200.0f };
        static const float Scale = 7.5f;
        VisualGamepad::Draw(InD2RT, InBrush1, InBrush2, Origin, Scale);
    }
}