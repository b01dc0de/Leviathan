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
        static bool bDrawBG = true;
        if (bDrawBG)
        {
            v2f BackgroundSize
            {
                vKeyboard.KeyList[LV_KEY_NONE].Size.X * Scale,
                vKeyboard.KeyList[LV_KEY_NONE].Size.Y * Scale
            };
            QuadF BackgroundQuad
            {
                Origin.X,
                Origin.Y - BackgroundSize.Y,
                BackgroundSize.X,
                BackgroundSize.Y
            };
            Draw2D.AddQuad(BackgroundQuad, RGBA{0.0f, 0.0f, 0.0f, 1.0f});
            Draw2D.AddBox(BackgroundQuad, RGBA{1.0f, 1.0f, 1.0f, 1.0f}, InputVisualizer::LineWidth);
        }
        for (int KeyIdx = LV_KEY_ESC; KeyIdx < LV_KEY_COUNT; KeyIdx++)
        {
            DrawKey(Draw2D, vKeyboard.KeyList[KeyIdx], Origin, Scale);
        }
    }

    void InputVisualizer::DrawKeyboard(BatchDraw2D& Draw2D)
    {
        v2f Origin{975.0f, (float)AppHeight - 10.0f};
        float Scale = 20.0f;
        VisualKeyboard::Draw(Draw2D, Origin, Scale);
    }

    struct VisualMouse
    {
        static void Draw(BatchDraw2D& Draw2D, const v2f& Origin, float Scale);
    };

    void VisualMouse::Draw(BatchDraw2D& Draw2D, const v2f& Origin, float Scale)
    {
        static constexpr v2f RegionSize{ 16.0f, 9.0f * 2.0f };
        static constexpr v2f ButtonSize{ RegionSize.X / 3.0f, RegionSize.Y / 2.0f };
        static constexpr float WheelSize = RegionSize.X / 4.0f;

        constexpr RGBA ColorWhite{ 1.0f, 1.0f, 1.0f, 1.0f };
        constexpr RGBA ColorBlack{ 0.0f, 0.0f, 0.0f, 1.0f };

        static constexpr bool bDrawBG = true;
        if (bDrawBG)
        {
            v2f AdjRegionSize { RegionSize.X * Scale, RegionSize.Y * Scale };
            QuadF RegionQuad{ Origin.X, Origin.Y - AdjRegionSize.Y, AdjRegionSize.X, AdjRegionSize.Y };
            Draw2D.AddQuad(RegionQuad, ColorBlack);
            Draw2D.AddBox(RegionQuad, ColorWhite, InputVisualizer::LineWidth);
        }

        const v2f AdjButtonSize{ ButtonSize.X * Scale, ButtonSize.Y * Scale };
        const float AdjWheelSize = WheelSize * Scale;
        QuadF LeftQuad{ Origin.X, Origin.Y - AdjButtonSize.Y, AdjButtonSize.X, AdjButtonSize.Y };
        QuadF MiddleQuad{ LeftQuad.PosX + AdjButtonSize.X, LeftQuad.PosY, AdjButtonSize.X, AdjButtonSize.Y - AdjWheelSize };
        QuadF RightQuad{ MiddleQuad.PosX + AdjButtonSize.X, LeftQuad.PosY, AdjButtonSize.X, AdjButtonSize.Y };
        { // Buttons
            if (MouseState::bLeftKey) { Draw2D.AddQuad(LeftQuad, ColorWhite); }
            else { Draw2D.AddBox(LeftQuad, ColorWhite, InputVisualizer::LineWidth); }
            if (MouseState::bRightKey) { Draw2D.AddQuad(RightQuad, ColorWhite); }
            else { Draw2D.AddBox(RightQuad, ColorWhite, InputVisualizer::LineWidth); }
            if (MouseState::bMiddleKey) { Draw2D.AddQuad(MiddleQuad, ColorWhite); }
            else { Draw2D.AddBox(MiddleQuad, ColorWhite, InputVisualizer::LineWidth); }
        }

        static constexpr v2f MouseAreaSize{ RegionSize.X, RegionSize.Y / 2.0f };
        static constexpr v2f MouseAreaPos{ 0.0f, 0.0f - RegionSize.Y };

        v2f AdjMouseAreaSize { MouseAreaSize.X * Scale, MouseAreaSize.Y * Scale };
        v2f AdjMouseAreaPos{ MouseAreaPos.X * Scale, MouseAreaPos.Y * Scale };
        QuadF MouseWindowQuad
        {
            AdjMouseAreaPos.X + Origin.X,
            AdjMouseAreaPos.Y + Origin.Y,
            AdjMouseAreaSize.X,
            AdjMouseAreaSize.Y
        };
        static float HalfCursorSize = Scale * 0.5f;
        { // Cursor
            v2f CursorPos =
            {
                ((float)MouseState::MouseX / (float)AppWidth * MouseWindowQuad.SizeX) + MouseWindowQuad.PosX,
                AdjMouseAreaSize.Y - ((float)MouseState::MouseY / (float)AppHeight * MouseWindowQuad.SizeY) + MouseWindowQuad.PosY
            };
            QuadF CursorQuad
            {
                CursorPos.X - HalfCursorSize,
                CursorPos.Y - HalfCursorSize,
                HalfCursorSize * 2.0f,
                HalfCursorSize * 2.0f
            };
            Draw2D.AddQuad(MouseWindowQuad, ColorWhite);
            Draw2D.AddQuad(CursorQuad, ColorBlack);
        }

        const v2f AdjWheelPos{ Origin.X + (RegionSize.X * Scale * 0.5f) - (AdjWheelSize * 0.5f), Origin.Y - AdjWheelSize };
        static float WheelAngle = 0.0f;
        static float VisualSpeed = 0.25f;
        WheelAngle += MouseState::MouseWheel * VisualSpeed;
        { // Wheel
            QuadF WheelQuad{ AdjWheelPos.X, AdjWheelPos.Y, AdjWheelSize, AdjWheelSize };
            Draw2D.AddBox(WheelQuad, RGBA{1.0f, 0.0f, 0.0f, 1.0f}, InputVisualizer::LineWidth);
            const v2f WheelLineCenter { AdjWheelPos.X + AdjWheelSize * 0.5f, AdjWheelPos.Y + AdjWheelSize * 0.5f };
            const v2f WheelLineEnd { WheelLineCenter.X + (cosf(WheelAngle) * AdjWheelSize * 0.5f), WheelLineCenter.Y + (sinf(WheelAngle) * AdjWheelSize * 0.5f) };
            QuadF WheelAngleQuad
            {
                Min(WheelLineCenter.X, WheelLineEnd.X),
                Min(WheelLineCenter.Y, WheelLineEnd.Y),
                Abs(WheelLineCenter.X - WheelLineEnd.X),
                Abs(WheelLineCenter.Y - WheelLineEnd.Y)
            };
            Draw2D.AddQuad(WheelAngleQuad, RGBA{ 0.0f, 1.0f, 1.0f, 1.0f });
        }
    }

    void InputVisualizer::DrawMouse(BatchDraw2D& Draw2D)
    {
        static v2f Origin{ 1000.0f, AppHeight - 200.0f };
        static float Scale = 5.0f;
        VisualMouse::Draw(Draw2D, Origin, Scale);
    }

    struct VisualGamepad
    {
        static void Draw(BatchDraw2D& Draw2D, const v2f& Origin, float Scale);
    };

    void VisualGamepad::Draw(BatchDraw2D& Draw2D, const v2f& Origin, float Scale)
    {
        static constexpr v2f GamepadRegionSize{ 16.0f, 16.0f };
        static constexpr v2f DPadButtonsCenter{ 0.25f, -0.5f };
        static constexpr v2f FaceButtonsCenter{ 0.75f, -0.5f };
        static constexpr float FaceButtonsSize{ 0.05f };
        static constexpr v2f ControlButtonsSize{ FaceButtonsSize * 1.5f, FaceButtonsSize };
        static constexpr v2f MenuButtonsCenter{ 0.5f, -(0.25f + ControlButtonsSize.Y) };
        static constexpr float ButtonsOffset{ FaceButtonsSize * 2.0f };
        static constexpr v2f TriggerButtonsSize{ 0.5f, 0.2f };
        static constexpr v2f ThumbStickCenter = { 0.5f, -0.75f };
        static constexpr float ThumbStickSize = { 0.25f };
        static constexpr v2f ButtonPos[LV_GAMEPAD_BUTTON_COUNT] =
        {
            {DPadButtonsCenter.X, DPadButtonsCenter.Y + ButtonsOffset},
            {DPadButtonsCenter.X, DPadButtonsCenter.Y - ButtonsOffset},
            {DPadButtonsCenter.X - ButtonsOffset, DPadButtonsCenter.Y},
            {DPadButtonsCenter.X + ButtonsOffset, DPadButtonsCenter.Y},
            {FaceButtonsCenter.X, FaceButtonsCenter.Y + ButtonsOffset},
            {FaceButtonsCenter.X, FaceButtonsCenter.Y - ButtonsOffset},
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

        static constexpr RGBA ColorBlack{ 0.0f, 0.0f, 0.0f, 0.0f };
        static constexpr RGBA ColorWhite{ 1.0f, 1.0f, 1.0f, 1.0f };

        static constexpr bool bDrawBG = true;
        if (bDrawBG)
        {
            QuadF BackgroundQuad
            {
                Origin.X, Origin.Y - AdjRegionSize.Y,
                AdjRegionSize.X, AdjRegionSize.Y
            };
            Draw2D.AddQuad(BackgroundQuad, ColorBlack);
            Draw2D.AddBox(BackgroundQuad, ColorWhite);
        }

        v2f AdjDPadOrigin{ DPadButtonsCenter.X * AdjRegionSize.X + Origin.X, DPadButtonsCenter.Y * AdjRegionSize.Y + Origin.Y };
        v2f AdjFaceOrigin{ FaceButtonsCenter.X * AdjRegionSize.X + Origin.X, FaceButtonsCenter.Y * AdjRegionSize.Y + Origin.Y };
        float AdjFaceButtonsSize = FaceButtonsSize * AdjRegionSize.X;
        float AdjButtonsOffset = ButtonsOffset * AdjRegionSize.X;
        for (int ButtonIdx = LV_GAMEPAD_DPAD_UP; ButtonIdx < LV_GAMEPAD_START; ButtonIdx++)
        {
            QuadF ButtonQuad
            {
                ButtonPos[ButtonIdx].X * GamepadRegionSize.X * Scale + Origin.X - AdjFaceButtonsSize / 2.0f,
                ButtonPos[ButtonIdx].Y * GamepadRegionSize.Y * Scale + Origin.Y - AdjFaceButtonsSize,
                AdjFaceButtonsSize, AdjFaceButtonsSize
            };
            if (GamepadState::GetButton((LvGamepadButton)ButtonIdx)) { Draw2D.AddQuad(ButtonQuad, ColorWhite); }
            else { Draw2D.AddBox(ButtonQuad, ColorWhite, InputVisualizer::LineWidth); }
        }
        for (int ButtonIdx = LV_GAMEPAD_START; ButtonIdx < LV_GAMEPAD_LEFT_THUMB; ButtonIdx++)
        {
            QuadF ButtonQuad
            {
                (ButtonPos[ButtonIdx].X - ControlButtonsSize.X / 2.0f) * GamepadRegionSize.X * Scale + Origin.X,
                (ButtonPos[ButtonIdx].Y - ControlButtonsSize.Y) * GamepadRegionSize.Y * Scale + Origin.Y,
                ControlButtonsSize.X * GamepadRegionSize.X * Scale,
                ControlButtonsSize.Y * GamepadRegionSize.X * Scale
            };
            if (GamepadState::GetButton((LvGamepadButton)ButtonIdx)) { Draw2D.AddQuad(ButtonQuad, ColorWhite); }
            else { Draw2D.AddBox(ButtonQuad, ColorWhite, InputVisualizer::LineWidth); }
        }

        v2f AdjTriggerSize{ TriggerButtonsSize.X * AdjRegionSize.X, TriggerButtonsSize.Y * AdjRegionSize.Y };
        float LTrigger = GamepadState::GetLeftTrigger();
        float RTrigger = GamepadState::GetRightTrigger();
        { // Triggers
            static constexpr float TriggerDeadzone = 0.05f;
            QuadF LeftTriggerQuad
            {
                Origin.X, Origin.Y - AdjTriggerSize.Y,
                AdjTriggerSize.X, AdjTriggerSize.Y
            };
            QuadF RightTriggerQuad
            {
                Origin.X + AdjTriggerSize.X, Origin.Y - AdjTriggerSize.Y,
                AdjTriggerSize.X, AdjTriggerSize.Y
            };
            Draw2D.AddBox(LeftTriggerQuad, ColorWhite, InputVisualizer::LineWidth);
            Draw2D.AddBox(RightTriggerQuad, ColorWhite, InputVisualizer::LineWidth);
            if (LTrigger > TriggerDeadzone)
            {
                LeftTriggerQuad.SizeX = AdjTriggerSize.X * LTrigger;
                Draw2D.AddQuad(LeftTriggerQuad, ColorWhite);
            }
            if (RTrigger > TriggerDeadzone)
            {
                RightTriggerQuad.SizeX = AdjTriggerSize.X * RTrigger;
                RightTriggerQuad.PosX += AdjTriggerSize.X - RightTriggerQuad.SizeX;
                Draw2D.AddQuad(RightTriggerQuad, ColorWhite);
            }
        }

        v2f LStick = GamepadState::GetLeftStick();
        v2f RStick = GamepadState::GetRightStick();
        { // Sticks
            float AdjStickSize = ThumbStickSize * AdjRegionSize.X;

            QuadF LeftStickQuad
            {
                Origin.X + (DPadButtonsCenter.X * AdjRegionSize.X) - AdjStickSize / 2.0f,
                Origin.Y + (ThumbStickCenter.Y * AdjRegionSize.Y) - AdjStickSize * 0.75f,
                AdjStickSize, AdjStickSize
            };
            QuadF RightStickQuad
            {
                Origin.X + (FaceButtonsCenter.X * AdjRegionSize.X) - AdjStickSize / 2.0f,
                Origin.Y + (ThumbStickCenter.Y * AdjRegionSize.Y) - AdjStickSize * 0.75f,
                AdjStickSize, AdjStickSize
            };

            RGBA LeftLineColor = ColorWhite;
            RGBA RightLineColor = ColorWhite;
            if (GamepadState::GetButton(LV_GAMEPAD_LEFT_THUMB)) { Draw2D.AddQuad(LeftStickQuad, ColorWhite); LeftLineColor = ColorBlack; }
            else { Draw2D.AddBox(LeftStickQuad, ColorWhite, InputVisualizer::LineWidth); }
            if (GamepadState::GetButton(LV_GAMEPAD_RIGHT_THUMB)) { Draw2D.AddQuad(RightStickQuad, ColorWhite); RightLineColor = ColorBlack; }
            else { Draw2D.AddBox(RightStickQuad, ColorWhite, InputVisualizer::LineWidth); }

            v2f LeftStickCenter
            {
                LeftStickQuad.PosX + LeftStickQuad.SizeX / 2.0f,
                LeftStickQuad.PosY + LeftStickQuad.SizeY / 2.0f
            };
            v2f RightStickCenter
            {
                RightStickQuad.PosX + RightStickQuad.SizeX / 2.0f,
                RightStickQuad.PosY + RightStickQuad.SizeY / 2.0f
            };

            v2f LeftStickInputPos
            {
                LeftStickCenter.X + LStick.X * AdjStickSize / 2.0f,
                LeftStickCenter.Y + LStick.Y * AdjStickSize / 2.0f
            };
            v2f RightStickInputPos
            {
                RightStickCenter.X + RStick.X * AdjStickSize / 2.0f,
                RightStickCenter.Y + RStick.Y * AdjStickSize / 2.0f
            };

            QuadF LeftInputQuad
            {
                Min(LeftStickInputPos.X, LeftStickCenter.X),
                Min(LeftStickInputPos.Y, LeftStickCenter.Y),
                Abs(LeftStickInputPos.X - LeftStickCenter.X),
                Abs(LeftStickInputPos.Y - LeftStickCenter.Y)
            };
            QuadF RightInputQuad
            {
                Min(RightStickInputPos.X, RightStickCenter.X),
                Min(RightStickInputPos.Y, RightStickCenter.Y),
                Abs(RightStickInputPos.X - RightStickCenter.X),
                Abs(RightStickInputPos.Y - LeftStickCenter.Y)
            };

            Draw2D.AddQuad(LeftInputQuad, LeftLineColor);
            Draw2D.AddQuad(RightInputQuad, RightLineColor);
        }
    }

    void InputVisualizer::DrawGamepad(BatchDraw2D& Draw2D)
    {
        static const v2f Origin{ 1100.0f, AppHeight - 200.0f };
        static const float Scale = 7.5f;
        VisualGamepad::Draw(Draw2D, Origin, Scale);
    }
}