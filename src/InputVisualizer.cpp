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
        static void Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush, const v2f& Origin, float Scale);
    };

    void VisualKeyboard::Init()
    {
        for (int KeyIdx = LV_KEY_NONE; KeyIdx < LV_KEY_COUNT; KeyIdx++) { KeyList[KeyIdx].LvCode = (LvKeyCode)KeyIdx; }
        KeyList[LV_KEY_BACKSPACE].Size.X = 2.0f;
        KeyList[LV_KEY_TAB].Size.X = 1.5f;
        KeyList[LV_KEY_BACKSLASH].Size.X = 1.5f;
        KeyList[LV_KEY_CAPSLOCK].Size.X = 1.75f;
        KeyList[LV_KEY_ENTER].Size.X = 2.25f;
        const v2f ModSize{ 2.0f, 1.0f };
        KeyList[LV_KEY_SHIFT].Size = ModSize;
        KeyList[LV_KEY_CTRL].Size = ModSize;
        KeyList[LV_KEY_SUPER].Size = ModSize;
        KeyList[LV_KEY_ALT].Size = ModSize;
        KeyList[LV_KEY_SPACE].Size.X = 6.0f;
        float FuncRowGap = 2.0f / 3.0f; // RowWidth = 15, NumRowKeys = 12, NumGaps = 3, FuncGap = (RowWidth - NumRowKeys) / NumGaps
        float RowX = 1.0f + FuncRowGap;
        for (int KeyIdx = LV_KEY_F1; KeyIdx <= LV_KEY_F12; KeyIdx++)
        {
            KeyList[KeyIdx].Pos = v2f{ RowX, 0.0f };
            RowX += KeyList[KeyIdx].Size.X;
            if (KeyIdx == LV_KEY_F4 || KeyIdx == LV_KEY_F8) { RowX += FuncRowGap; }
        }
        float FuncRowPadding = 0.25f;
        float RowY = 1.0f + FuncRowPadding;
        RowX = 0.0f;
        v2f RightRegionOrigin{ 15.0f, 0.0f };
        for (int KeyIdx = LV_KEY_GRAVE; KeyIdx <= LV_KEY_SLASH; KeyIdx++)
        {
            KeyList[KeyIdx].Pos = v2f{RowX, RowY};
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
        KeyList[LV_KEY_SHIFT].Pos = ModOrigin;
        KeyList[LV_KEY_CTRL].Pos = ModOrigin + v2f{ 0.0f, 1.0f };
        KeyList[LV_KEY_SUPER].Pos = ModOrigin + v2f{ 0.0f, 2.0f };;
        KeyList[LV_KEY_ALT].Pos = ModOrigin + v2f{ 0.0f, 3.0f };
        KeyList[LV_KEY_ARROW_UP].Pos = RightRegionOrigin + v2f{ 1.0f, RowY - 1.0f };
        KeyList[LV_KEY_ARROW_LEFT].Pos = RightRegionOrigin + v2f{ 0.0f, RowY };
        KeyList[LV_KEY_ARROW_DOWN].Pos = RightRegionOrigin + v2f{ 1.0f, RowY };
        KeyList[LV_KEY_ARROW_RIGHT].Pos = RightRegionOrigin + v2f{ 2.0f, RowY };

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

    void VisualKeyboard::Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush, const v2f& Origin, float Scale)
    {
        static VisualKeyboard vKeyboard;
        if (!vKeyboard.bInit) { vKeyboard.Init(); }
        for (int KeyIdx = LV_KEY_ESC; KeyIdx < LV_KEY_COUNT; KeyIdx++)
        {
            DrawKey(InD2RT, InBrush, vKeyboard.KeyList[KeyIdx], Origin, Scale);
        }
    }

    void InputVisualizer::DrawKeyboard(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush)
    {
        v2f Origin{800.0f, 10.0};
        float Scale = 20.0f;
        VisualKeyboard::Draw(InD2RT, InBrush, Origin, Scale);
    }

    struct VisualMouse
    {
        static void Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2, const v2f& Origin, float Scale);
    };

    void VisualMouse::Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2, const v2f& Origin, float Scale)
    {
        static v2f ButtonSize{ Scale, Scale * 2.0f };
        static D2D1_RECT_F LeftRect{ Origin.X, Origin.Y, Origin.X + ButtonSize.X, Origin.Y + ButtonSize.Y };
        static D2D1_RECT_F MiddleRect{ LeftRect.left + ButtonSize.X, LeftRect.top, LeftRect.right + ButtonSize.X, LeftRect.bottom };
        static D2D1_RECT_F RightRect{ MiddleRect.left + ButtonSize.X, MiddleRect.top, MiddleRect.right + ButtonSize.X, MiddleRect.bottom };
        { // Buttons
            if (MouseState::bLeftKey) { InD2RT->FillRectangle(&LeftRect, InBrush1); }
            else { InD2RT->DrawRectangle(&LeftRect, InBrush1, InputVisualizer::LineWidth, nullptr); }
            if (MouseState::bRightKey) { InD2RT->FillRectangle(&RightRect, InBrush1); }
            else { InD2RT->DrawRectangle(&RightRect, InBrush1, InputVisualizer::LineWidth, nullptr); }
            if (MouseState::bMiddleKey) { InD2RT->FillRectangle(&MiddleRect, InBrush1); }
            else { InD2RT->DrawRectangle(&MiddleRect, InBrush1, InputVisualizer::LineWidth, nullptr); }
        }

        static v2f MouseWindowSize{ 128.0f, 72.0f };
        static v2f MouseWindowOrigin{ (LeftRect.left + RightRect.right - MouseWindowSize.X) * 0.5f, Origin.Y + ButtonSize.Y};
        static D2D1_RECT_F MouseWindowRect{ MouseWindowOrigin.X, MouseWindowOrigin.Y, MouseWindowOrigin.X + MouseWindowSize.X, MouseWindowOrigin.Y + MouseWindowSize.Y };
        static float CursorSize = 5.0f;
        static float HalfCursorSize = CursorSize * 0.5f;
        { // Cursor
            v2f CursorPos = {
                ((float)MouseState::MouseX / (float)AppWidth * MouseWindowSize.X) + MouseWindowOrigin.X,
                ((float)MouseState::MouseY / (float)AppHeight * MouseWindowSize.Y) + MouseWindowOrigin.Y
            };
            D2D1_RECT_F CursorRect{
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
            static float WheelSize = 25.0f;
            static D2D1_POINT_2F WheelPos =
            {
                (LeftRect.left + RightRect.right) / 2.0f,
                LeftRect.top - WheelSize
            };
            static D2D1_ELLIPSE WheelEllipse{ WheelPos, WheelSize, WheelSize };
            InD2RT->DrawEllipse(WheelEllipse, InBrush1, InputVisualizer::LineWidth, nullptr);
            D2D1_POINT_2F WheelLineEnd =
            {
                WheelPos.x + (cosf(WheelAngle) * WheelSize),
                WheelPos.y + (sinf(WheelAngle) * WheelSize),
            };
            InD2RT->DrawLine(WheelPos, WheelLineEnd, InBrush1, InputVisualizer::LineWidth, nullptr);
        }
    }

    void InputVisualizer::DrawMouse(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2)
    {
        static v2f Origin{ 1100.0f, AppHeight - 200.0f };
        static float Scale = 25.0f;
        VisualMouse::Draw(InD2RT, InBrush1, InBrush2, Origin, Scale);
    }

    struct VisualGamepad
    {
        static void Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2, const v2f& Origin, float Scale);
    };

    void VisualGamepad::Draw(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2, const v2f& Origin, float Scale)
    {
        const v2f ButtonSize{ Scale, Scale };
        const v2f DPadOrigin = Origin + v2f{ 50.0f, 50.0f };
        const v2f FaceOrigin = DPadOrigin + v2f{ ButtonSize.X * 8.0f, 0.0f };
        v2f ButtonPos[LV_GAMEPAD_BUTTON_COUNT];
        ButtonPos[LV_GAMEPAD_DPAD_UP] = DPadOrigin + v2f{ ButtonSize.X, 0.0f };
        ButtonPos[LV_GAMEPAD_DPAD_DOWN] = DPadOrigin + v2f{ ButtonSize.X, ButtonSize.Y * 2.0f };
        ButtonPos[LV_GAMEPAD_DPAD_LEFT] = DPadOrigin + v2f{ 0.0f, ButtonSize.Y };
        ButtonPos[LV_GAMEPAD_DPAD_RIGHT] = DPadOrigin + v2f{ ButtonSize.X * 2.0f, ButtonSize.Y };
        ButtonPos[LV_GAMEPAD_FACE_UP] = FaceOrigin + v2f{ ButtonSize.X, 0.0f };
        ButtonPos[LV_GAMEPAD_FACE_DOWN] = FaceOrigin + v2f{ ButtonSize.X, ButtonSize.Y * 2.0f };
        ButtonPos[LV_GAMEPAD_FACE_LEFT] = FaceOrigin + v2f{ 0.0f, ButtonSize.Y };
        ButtonPos[LV_GAMEPAD_FACE_RIGHT] = FaceOrigin + v2f{ ButtonSize.X * 2.0f, ButtonSize.Y };
        ButtonPos[LV_GAMEPAD_START] = DPadOrigin + v2f{ ButtonSize.X * 6.0f, 0.0f };
        ButtonPos[LV_GAMEPAD_SELECT] = DPadOrigin + v2f{ ButtonSize.X * 4.0f, 0.0f };
        ButtonPos[LV_GAMEPAD_LEFT_SHOULDER] = DPadOrigin + v2f{ ButtonSize.X, ButtonSize.Y * -2.0f };
        ButtonPos[LV_GAMEPAD_RIGHT_SHOULDER] = FaceOrigin + v2f{ ButtonSize.X, ButtonSize.Y * -2.0f };
        ButtonPos[LV_GAMEPAD_LEFT_THUMB] = DPadOrigin + v2f{ ButtonSize.X, ButtonSize.Y * 4.0f };
        ButtonPos[LV_GAMEPAD_RIGHT_THUMB] = FaceOrigin + v2f{ ButtonSize.X, ButtonSize.Y * 4.0f };

        for (int ButtonIdx = LV_GAMEPAD_DPAD_UP; ButtonIdx < LV_GAMEPAD_LEFT_THUMB; ButtonIdx++)
        {
            D2D1_RECT_F ButtonRect =
            {
                ButtonPos[ButtonIdx].X,
                ButtonPos[ButtonIdx].Y,
                ButtonPos[ButtonIdx].X + ButtonSize.X,
                ButtonPos[ButtonIdx].Y + ButtonSize.Y,
            };
            if (GamepadState::GetButton((LvGamepadButton)ButtonIdx))
            {
                InD2RT->FillRectangle(&ButtonRect, InBrush1);
            }
            else
            {
                InD2RT->DrawRectangle(&ButtonRect, InBrush1, InputVisualizer::LineWidth, nullptr);
            }
        }

        float LTrigger = GamepadState::GetLeftTrigger();
        float RTrigger = GamepadState::GetRightTrigger();
        { // Triggers
            static constexpr float TriggerDeadzone = 0.05f;
            const v2f LTriggerPos = ButtonPos[LV_GAMEPAD_LEFT_SHOULDER] + v2f{ ButtonSize.X * 2.0f, ButtonSize.Y * -1.0f };
            const v2f RTriggerPos = ButtonPos[LV_GAMEPAD_RIGHT_SHOULDER] + v2f{ ButtonSize.X * -2.0f, ButtonSize.Y * -1.0f };
            const v2f TriggerSize{ 25.0f, 50.0f };
            D2D1_RECT_F LeftTriggerRect{ LTriggerPos.X, LTriggerPos.Y,
                LTriggerPos.X + TriggerSize.X, LTriggerPos.Y + TriggerSize.Y };
            D2D1_RECT_F RightTriggerRect{ RTriggerPos.X, RTriggerPos.Y,
                RTriggerPos.X + TriggerSize.X, RTriggerPos.Y + TriggerSize.Y };
            InD2RT->DrawRectangle(&LeftTriggerRect, InBrush1, InputVisualizer::LineWidth, nullptr);
            InD2RT->DrawRectangle(&RightTriggerRect, InBrush1, InputVisualizer::LineWidth, nullptr);
            if (LTrigger > TriggerDeadzone)
            {
                LeftTriggerRect.bottom = LTriggerPos.Y + (TriggerSize.Y * LTrigger);
                InD2RT->FillRectangle(&LeftTriggerRect, InBrush2);
            }
            if (RTrigger > TriggerDeadzone)
            {
                RightTriggerRect.bottom = RTriggerPos.Y + (TriggerSize.Y * RTrigger);
                InD2RT->FillRectangle(&RightTriggerRect, InBrush2);
            }
        }

        v2f LStick = GamepadState::GetLeftStick();
        v2f RStick = GamepadState::GetRightStick();
        { // Sticks
            static const float StickSize = 25.0f;
            const D2D1_POINT_2F LStickPos { ButtonPos[LV_GAMEPAD_SELECT].X, ButtonPos[LV_GAMEPAD_DPAD_DOWN].Y + StickSize };
            const D2D1_POINT_2F RStickPos{ ButtonPos[LV_GAMEPAD_START].X + StickSize, ButtonPos[LV_GAMEPAD_FACE_DOWN].Y + StickSize };
            D2D1_ELLIPSE LeftEllipse{ LStickPos, StickSize, StickSize };
            D2D1_ELLIPSE RightEllipse{ RStickPos, StickSize, StickSize };

            if (GamepadState::GetButton(LV_GAMEPAD_LEFT_THUMB)) { InD2RT->FillEllipse(&LeftEllipse, InBrush1); }
            else { InD2RT->DrawEllipse(&LeftEllipse, InBrush1, InputVisualizer::LineWidth, nullptr); }
            if (GamepadState::GetButton(LV_GAMEPAD_RIGHT_THUMB)) { InD2RT->FillEllipse(&RightEllipse, InBrush1); }
            else { InD2RT->DrawEllipse(&RightEllipse, InBrush1, InputVisualizer::LineWidth, nullptr); }

            D2D1_POINT_2F LStickInput
            {
                LStickPos.x + LStick.X * StickSize,
                LStickPos.y + -LStick.Y * StickSize
            };
            D2D1_POINT_2F RStickInput
            {
                RStickPos.x + RStick.X * StickSize,
                RStickPos.y + -RStick.Y * StickSize
            };
            InD2RT->DrawLine(LStickPos, LStickInput, InBrush2, InputVisualizer::LineWidth, nullptr);
            InD2RT->DrawLine(RStickPos, RStickInput, InBrush2, InputVisualizer::LineWidth, nullptr);
        }
    }

    void InputVisualizer::DrawGamepad(ID2D1RenderTarget* InD2RT, ID2D1Brush* InBrush1, ID2D1Brush* InBrush2)
    {
        static const v2f Origin{ 0.0f, AppHeight - 200.0f };
        static const float Scale = 25.0f;
        VisualGamepad::Draw(InD2RT, InBrush1, InBrush2, Origin, Scale);
    }
}