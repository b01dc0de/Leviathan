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
        constexpr float fDeadzone = 0.001f;
        return Abs(Stick.X) > fDeadzone || Abs(Stick.Y) > fDeadzone;
    }

    void BulletLimboState::Update()
    {
        v2f LeftStick = GamepadState::GetLeftStick();
        v2f RightStick = GamepadState::GetRightStick();
        bool bLeftStick = StickInputNotDead(LeftStick);
        bool bRightStick = StickInputNotDead(RightStick);
        if (bLeftStick)
        {
            PlayerPos = PlayerPos + (LeftStick * PlayerSpeed);
        }
        if (GamepadState::GetButton(LV_GAMEPAD_RIGHT_SHOULDER)) { }
        else if (bRightStick) { PlayerAngle = atan2f(RightStick.Y, RightStick.X); }
        else if (bLeftStick) { PlayerAngle = atan2f(LeftStick.Y, LeftStick.X); }
    }
    void BulletLimboState::Draw(BatchDraw2D& Draw2D)
    {
        Draw2D.AddRect
        (
            RectF{ PlayerPos.X, PlayerPos.Y, DrawInfo.PlayerSize + DrawInfo.OutlineSize, DrawInfo.PlayerSize + DrawInfo.OutlineSize },
            fColor{ 1.0f, 1.0f, 1.0f, 1.0f },
            PlayerAngle
        );
        Draw2D.AddRect
        (
            RectF{ PlayerPos.X, PlayerPos.Y, DrawInfo.PlayerSize, DrawInfo.PlayerSize},
            fColor{ 0.0f, 0.0f, 0.0f, 1.0f },
            PlayerAngle
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
