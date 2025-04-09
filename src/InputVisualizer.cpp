#include "InputVisualizer.h"

namespace Leviathan
{

struct VIDrawParams
{
    v2f Origin{ 0.0f };
    float Scale{ 1.0f };
    fColor ColorBack{ 0.0f, 0.0f, 0.0f, 1.0f };
    fColor ColorFront{ 1.0f, 1.0f, 1.0f, 1.0f };
};

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
    static void DrawKey(BatchDrawCmds& Draw2D, const VisualKey& Key, const VIDrawParams& Params);
    static void Draw(BatchDrawCmds& Draw2D, const VIDrawParams&);
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

void VisualKeyboard::DrawKey(BatchDrawCmds& Draw2D, const VisualKey& Key, const VIDrawParams& Params)
{
    float SizeX = (Key.Size.X * Params.Scale);
    float SizeY = (Key.Size.Y * Params.Scale);
    float PosX = Params.Origin.X + (Key.Pos.X * Params.Scale);
    float PosY = Params.Origin.Y - (Key.Pos.Y * Params.Scale) - SizeY;
    RectF KeyQuad = { PosX, PosY, SizeX, SizeY };
    bool bIsDown = KeyboardState::GetKey(Key.LvCode, true);
    if (bIsDown)
    {
        Draw2D.AddRect(KeyQuad, Params.ColorFront);
    }
    else
    {
        Draw2D.AddBox(KeyQuad, Params.ColorFront, InputVisualizer::LineWidth);
    }
}

void VisualKeyboard::Draw(BatchDrawCmds& Draw2D, const VIDrawParams& Params)
{
    static VisualKeyboard vKeyboard;
    if (!vKeyboard.bInit) { vKeyboard.Init(); }
    static bool bDrawBG = true;
    if (bDrawBG)
    {
        v2f BackgroundSize
        {
            vKeyboard.KeyList[LV_KEY_NONE].Size.X * Params.Scale,
            vKeyboard.KeyList[LV_KEY_NONE].Size.Y * Params.Scale
        };
        RectF BackgroundQuad
        {
            Params.Origin.X,
            Params.Origin.Y - BackgroundSize.Y,
            BackgroundSize.X,
            BackgroundSize.Y
        };
        Draw2D.AddRect(BackgroundQuad, Params.ColorBack);
        Draw2D.AddBox(BackgroundQuad, Params.ColorFront, InputVisualizer::LineWidth);
    }
    for (int KeyIdx = LV_KEY_ESC; KeyIdx < LV_KEY_COUNT; KeyIdx++)
    {
        DrawKey(Draw2D, vKeyboard.KeyList[KeyIdx], Params);
    }
}

void InputVisualizer::DrawKeyboard(BatchDrawCmds& Draw2D)
{
    VIDrawParams Params = { };
    Params.Origin = { 975.0f, (float)AppHeight - 10.0f };
    Params.Scale = 20.0f;
    Params.ColorBack = { 0.0f, 0.9f, 0.9f, 1.0f };
    Params.ColorFront = { 1.0f, 0.1f, 0.1f, 1.0f };
    VisualKeyboard::Draw(Draw2D, Params);
}

struct VisualMouse
{
    static void Draw(BatchDrawCmds& Draw2D, const VIDrawParams& Params);
};

void VisualMouse::Draw(BatchDrawCmds& Draw2D, const VIDrawParams& Params)
{
    static constexpr v2f RegionSize{ 16.0f, 9.0f * 2.0f };
    static constexpr v2f ButtonSize{ RegionSize.X / 3.0f, RegionSize.Y / 2.0f };
    static constexpr float WheelSize = RegionSize.X / 4.0f;

    static constexpr bool bDrawBG = true;
    if (bDrawBG)
    {
        v2f AdjRegionSize{ RegionSize.X * Params.Scale, RegionSize.Y * Params.Scale };
        RectF RegionQuad{ Params.Origin.X, Params.Origin.Y - AdjRegionSize.Y, AdjRegionSize.X, AdjRegionSize.Y };
        Draw2D.AddRect(RegionQuad, Params.ColorBack);
        Draw2D.AddBox(RegionQuad, Params.ColorFront, InputVisualizer::LineWidth);
    }

    const v2f AdjButtonSize{ ButtonSize.X * Params.Scale, ButtonSize.Y * Params.Scale };
    const float AdjWheelSize = WheelSize * Params.Scale;
    RectF LeftQuad{ Params.Origin.X, Params.Origin.Y - AdjButtonSize.Y, AdjButtonSize.X, AdjButtonSize.Y };
    RectF MiddleQuad{ LeftQuad.PosX + AdjButtonSize.X, LeftQuad.PosY, AdjButtonSize.X, AdjButtonSize.Y - AdjWheelSize };
    RectF RightQuad{ MiddleQuad.PosX + AdjButtonSize.X, LeftQuad.PosY, AdjButtonSize.X, AdjButtonSize.Y };
    { // Buttons
        if (MouseState::bLeftKey) { Draw2D.AddRect(LeftQuad, Params.ColorFront); }
        else { Draw2D.AddBox(LeftQuad, Params.ColorFront, InputVisualizer::LineWidth); }
        if (MouseState::bRightKey) { Draw2D.AddRect(RightQuad, Params.ColorFront); }
        else { Draw2D.AddBox(RightQuad, Params.ColorFront, InputVisualizer::LineWidth); }
        if (MouseState::bMiddleKey) { Draw2D.AddRect(MiddleQuad, Params.ColorFront); }
        else { Draw2D.AddBox(MiddleQuad, Params.ColorFront, InputVisualizer::LineWidth); }
    }

    static constexpr v2f MouseAreaSize{ RegionSize.X, RegionSize.Y / 2.0f };
    static constexpr v2f MouseAreaPos{ 0.0f, 0.0f - RegionSize.Y };

    v2f AdjMouseAreaSize{ MouseAreaSize.X * Params.Scale, MouseAreaSize.Y * Params.Scale };
    v2f AdjMouseAreaPos{ MouseAreaPos.X * Params.Scale, MouseAreaPos.Y * Params.Scale };
    RectF MouseWindowQuad
    {
        AdjMouseAreaPos.X + Params.Origin.X,
        AdjMouseAreaPos.Y + Params.Origin.Y,
        AdjMouseAreaSize.X,
        AdjMouseAreaSize.Y
    };
    static float HalfCursorSize = Params.Scale * 0.5f;
    { // Cursor
        v2f CursorPos =
        {
            ((float)MouseState::MouseX / (float)AppWidth * MouseWindowQuad.SizeX) + MouseWindowQuad.PosX,
            AdjMouseAreaSize.Y - ((float)MouseState::MouseY / (float)AppHeight * MouseWindowQuad.SizeY) + MouseWindowQuad.PosY
        };
        RectF CursorQuad
        {
            CursorPos.X - HalfCursorSize,
            CursorPos.Y - HalfCursorSize,
            HalfCursorSize * 2.0f,
            HalfCursorSize * 2.0f
        };
        Draw2D.AddRect(MouseWindowQuad, Params.ColorFront);
        Draw2D.AddRect(CursorQuad, Params.ColorBack);
    }

    const v2f AdjWheelPos{ Params.Origin.X + (RegionSize.X * Params.Scale * 0.5f) - (AdjWheelSize * 0.5f), Params.Origin.Y - AdjWheelSize };
    static float WheelAngle = 0.0f;
    static float VisualSpeed = 0.25f;
    WheelAngle += MouseState::MouseWheel * VisualSpeed;
    { // Wheel
        RectF WheelQuad{ AdjWheelPos.X, AdjWheelPos.Y, AdjWheelSize, AdjWheelSize };
        Draw2D.AddBox(WheelQuad, Params.ColorFront, InputVisualizer::LineWidth);
        const v2f WheelLineCenter{ AdjWheelPos.X + AdjWheelSize * 0.5f, AdjWheelPos.Y + AdjWheelSize * 0.5f };
        const v2f WheelLineEnd{ WheelLineCenter.X + (cosf(WheelAngle) * AdjWheelSize * 0.5f), WheelLineCenter.Y + (sinf(WheelAngle) * AdjWheelSize * 0.5f) };
        /*
        RectF WheelAngleQuad
        {
            Min(WheelLineCenter.X, WheelLineEnd.X),
            Min(WheelLineCenter.Y, WheelLineEnd.Y),
            Abs(WheelLineCenter.X - WheelLineEnd.X),
            Abs(WheelLineCenter.Y - WheelLineEnd.Y)
        };
        Draw2D.AddRect(WheelAngleQuad, Params.ColorFront);
        */
        LineF WheelLine
        {
            WheelLineCenter.X,
            WheelLineCenter.Y,
            WheelLineEnd.X,
            WheelLineEnd.Y
        };
        Draw2D.AddLine(WheelLine, Params.ColorFront);
    }
}

void InputVisualizer::DrawMouse(BatchDrawCmds& Draw2D)
{
    VIDrawParams Params = {};
    Params.Origin = { 1000.0f, AppHeight - 200.0f };
    Params.Scale = 5.0f;
    Params.ColorBack = { 0.9f, 0.0f, 0.9f, 1.0f };
    Params.ColorFront = { 0.1f, 1.0f, 0.1f, 1.0f };
    VisualMouse::Draw(Draw2D, Params);
}

struct VisualGamepad
{
    static void Draw(BatchDrawCmds& Draw2D, int GamepadIdx, const VIDrawParams& Params);
};

void VisualGamepad::Draw(BatchDrawCmds& Draw2D, int GamepadIdx, const VIDrawParams& Params)
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

    if ((GamepadIdx < 0 || GamepadIdx > GamepadState::NumGamepads) || !GamepadState::bActive[GamepadIdx])
    {
        return;
    }

    const v2f& Origin = Params.Origin;
    float Scale = Params.Scale;
    const fColor ColorBack = Params.ColorBack;
    const fColor ColorFront = Params.ColorFront;

    v2f AdjRegionSize{ GamepadRegionSize.X * Scale, GamepadRegionSize.Y * Scale };

    static constexpr bool bDrawBG = true;
    if (bDrawBG)
    {
        RectF BackgroundQuad
        {
            Origin.X, Origin.Y - AdjRegionSize.Y,
            AdjRegionSize.X, AdjRegionSize.Y
        };
        Draw2D.AddRect(BackgroundQuad, ColorBack);
        Draw2D.AddBox(BackgroundQuad, ColorFront, InputVisualizer::LineWidth);
    }

    v2f AdjDPadOrigin{ DPadButtonsCenter.X * AdjRegionSize.X + Origin.X, DPadButtonsCenter.Y * AdjRegionSize.Y + Origin.Y };
    v2f AdjFaceOrigin{ FaceButtonsCenter.X * AdjRegionSize.X + Origin.X, FaceButtonsCenter.Y * AdjRegionSize.Y + Origin.Y };
    float AdjFaceButtonsSize = FaceButtonsSize * AdjRegionSize.X;
    float AdjButtonsOffset = ButtonsOffset * AdjRegionSize.X;
    for (int ButtonIdx = LV_GAMEPAD_DPAD_UP; ButtonIdx < LV_GAMEPAD_START; ButtonIdx++)
    {
        RectF ButtonQuad
        {
            ButtonPos[ButtonIdx].X * GamepadRegionSize.X * Scale + Origin.X - AdjFaceButtonsSize / 2.0f,
            ButtonPos[ButtonIdx].Y * GamepadRegionSize.Y * Scale + Origin.Y - AdjFaceButtonsSize,
            AdjFaceButtonsSize, AdjFaceButtonsSize
        };
        if (GamepadState::GetButton((LvGamepadButton)ButtonIdx, GamepadIdx)) { Draw2D.AddRect(ButtonQuad, ColorFront); }
        else { Draw2D.AddBox(ButtonQuad, ColorFront, InputVisualizer::LineWidth); }
    }
    for (int ButtonIdx = LV_GAMEPAD_START; ButtonIdx < LV_GAMEPAD_LEFT_THUMB; ButtonIdx++)
    {
        RectF ButtonQuad
        {
            (ButtonPos[ButtonIdx].X - ControlButtonsSize.X / 2.0f) * GamepadRegionSize.X * Scale + Origin.X,
            (ButtonPos[ButtonIdx].Y - ControlButtonsSize.Y) * GamepadRegionSize.Y * Scale + Origin.Y,
            ControlButtonsSize.X * GamepadRegionSize.X * Scale,
            ControlButtonsSize.Y * GamepadRegionSize.X * Scale
        };
        if (GamepadState::GetButton((LvGamepadButton)ButtonIdx, GamepadIdx)) { Draw2D.AddRect(ButtonQuad, ColorFront); }
        else { Draw2D.AddBox(ButtonQuad, ColorFront, InputVisualizer::LineWidth); }
    }

    v2f AdjTriggerSize{ TriggerButtonsSize.X * AdjRegionSize.X, TriggerButtonsSize.Y * AdjRegionSize.Y };
    float LTrigger = GamepadState::GetLeftTrigger(GamepadIdx);
    float RTrigger = GamepadState::GetRightTrigger(GamepadIdx);
    { // Triggers
        static constexpr float TriggerDeadzone = 0.05f;
        RectF LeftTriggerQuad
        {
            Origin.X, Origin.Y - AdjTriggerSize.Y,
            AdjTriggerSize.X, AdjTriggerSize.Y
        };
        RectF RightTriggerQuad
        {
            Origin.X + AdjTriggerSize.X, Origin.Y - AdjTriggerSize.Y,
            AdjTriggerSize.X, AdjTriggerSize.Y
        };
        Draw2D.AddBox(LeftTriggerQuad, ColorFront, InputVisualizer::LineWidth);
        Draw2D.AddBox(RightTriggerQuad, ColorFront, InputVisualizer::LineWidth);
        if (LTrigger > TriggerDeadzone)
        {
            LeftTriggerQuad.SizeX = AdjTriggerSize.X * LTrigger;
            Draw2D.AddRect(LeftTriggerQuad, ColorFront);
        }
        if (RTrigger > TriggerDeadzone)
        {
            RightTriggerQuad.SizeX = AdjTriggerSize.X * RTrigger;
            RightTriggerQuad.PosX += AdjTriggerSize.X - RightTriggerQuad.SizeX;
            Draw2D.AddRect(RightTriggerQuad, ColorFront);
        }
    }

    v2f LStick = GamepadState::GetLeftStick(GamepadIdx);
    v2f RStick = GamepadState::GetRightStick(GamepadIdx);
    { // Sticks
        float AdjStickSize = ThumbStickSize * AdjRegionSize.X;

        RectF LeftStickQuad
        {
            Origin.X + (DPadButtonsCenter.X * AdjRegionSize.X) - AdjStickSize / 2.0f,
            Origin.Y + (ThumbStickCenter.Y * AdjRegionSize.Y) - AdjStickSize * 0.75f,
            AdjStickSize, AdjStickSize
        };
        RectF RightStickQuad
        {
            Origin.X + (FaceButtonsCenter.X * AdjRegionSize.X) - AdjStickSize / 2.0f,
            Origin.Y + (ThumbStickCenter.Y * AdjRegionSize.Y) - AdjStickSize * 0.75f,
            AdjStickSize, AdjStickSize
        };

        fColor LeftLineColor = ColorFront;
        fColor RightLineColor = ColorFront;
        if (GamepadState::GetButton(LV_GAMEPAD_LEFT_THUMB, GamepadIdx))
        {
            Draw2D.AddRect(LeftStickQuad, ColorFront);
            LeftLineColor = ColorBack;
        }
        else { Draw2D.AddBox(LeftStickQuad, ColorFront, InputVisualizer::LineWidth); }
        if (GamepadState::GetButton(LV_GAMEPAD_RIGHT_THUMB, GamepadIdx))
        {
            Draw2D.AddRect(RightStickQuad, ColorFront);
            RightLineColor = ColorBack;
        }
        else { Draw2D.AddBox(RightStickQuad, ColorFront, InputVisualizer::LineWidth); }

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

        LineF LeftStickLine
        {
            LeftStickCenter.X, LeftStickCenter.Y,
            LeftStickCenter.X + LStick.X * AdjStickSize / 2.0f,
            LeftStickCenter.Y + LStick.Y * AdjStickSize / 2.0f
        };
        LineF RightStickLine
        {
            RightStickCenter.X, RightStickCenter.Y,
            RightStickCenter.X + RStick.X * AdjStickSize / 2.0f,
            RightStickCenter.Y + RStick.Y * AdjStickSize / 2.0f
        };
        Draw2D.AddLine(LeftStickLine, LeftLineColor);
        Draw2D.AddLine(RightStickLine, RightLineColor);
    }
}

void InputVisualizer::DrawGamepads(BatchDrawCmds& Draw2D)
{
    VIDrawParams Params = {};
    Params.Origin = { 1100.0f, AppHeight - 200.0f };
    Params.Scale = 7.5f;
    Params.ColorBack = { 0.9f, 0.9f, 0.0f, 1.0f };
    Params.ColorFront = { 0.1f, 0.1f, 1.0f, 1.0f };
    for (int GamepadIdx = 0; GamepadIdx < GamepadState::NumGamepads; GamepadIdx++)
    {
        VIDrawParams CurrParams = Params;
        CurrParams.Origin = { Params.Origin.X, Params.Origin.Y - 150.0f * GamepadIdx };
        VisualGamepad::Draw(Draw2D, GamepadIdx, CurrParams);
    }
}

}

