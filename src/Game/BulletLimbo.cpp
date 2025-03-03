#include "BulletLimbo.h"

namespace Game
{
    using namespace Leviathan;

    namespace BulletLimboState
    {
        constexpr struct DrawInfoT
        {
            float PlayerSize = 25.0f;
            float OutlineSize = 2.0f;
            float BulletSize = 15.0f;
        } DrawInfo;
        v2f PlayerPos;
        float PlayerAngle;
        float PlayerSpeed;

        void Update();
        void Draw(BatchDraw2D& Draw2D);
        void Init();
        void Term();
    }

    bool StickInputNotDead(v2f& Stick)
    {
        constexpr float fDeadzone = 0.01f;
        return Abs(Stick.X) > fDeadzone || Abs(Stick.Y) > fDeadzone;
    }

    void BulletLimboState::Update()
    {
        v2f LeftStick = GamepadState::GetLeftStick();
        v2f RightStick = GamepadState::GetRightStick();
        PlayerPos = PlayerPos + (LeftStick * PlayerSpeed);
        if (GamepadState::GetButton(LV_GAMEPAD_RIGHT_SHOULDER)) { }
        else if (Length(RightStick) > 0.01f){ PlayerAngle = atan2f(RightStick.Y, RightStick.X); }
        else if (Length(LeftStick) > 0.025f) { PlayerAngle = atan2f(LeftStick.Y, LeftStick.X); }
    }
    void BulletLimboState::Draw(BatchDraw2D& Draw2D)
    {
        constexpr float PlayerAngleVisualOffset = -fPI / 4.0f;
        float fOneThird = 1.0f / 3.0f;
        RectF PlayerTexRect{ 0.0f, 0.0f, fOneThird, 1.0f };
        RectF EnemyTexRect{ fOneThird, 0.0f, fOneThird, 1.0f };
        RectF BulletTexRect{ fOneThird * 2.0f, 0.0f, fOneThird, 1.0f };
        Draw2D.AddRect
        (
            RectF{ PlayerPos.X, PlayerPos.Y, DrawInfo.PlayerSize, DrawInfo.PlayerSize},
            PlayerTexRect,
            PlayerAngle + PlayerAngleVisualOffset
        );
    }
    void BulletLimboState::Init()
    {
        PlayerPos = { (float)AppWidth / 2.0f, (float)AppHeight / 2.0f };
        PlayerAngle = 0.0f;
        PlayerSpeed = 1.0f;
    }
    void BulletLimboState::Term()
    {
    }

    void BulletLimbo::Update() { BulletLimboState::Update(); }
    void BulletLimbo::Draw(BatchDraw2D& Draw2D) { BulletLimboState::Draw(Draw2D); }
    void BulletLimbo::Init() { BulletLimboState::Init(); }
    void BulletLimbo::Term() { BulletLimboState::Term(); }
}
