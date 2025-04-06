#include "BulletLimbo.h"

namespace Game
{
    using namespace Leviathan;

    namespace BulletLimboState
    {
        constexpr struct DrawInfoT
        {
            float PlayerSize = 25.0f;
            float BulletSize = 15.0f;
        } DrawInfo;
        v2f PlayerPos;
        float PlayerAngle;
        float PlayerSpeed;
        double LastBulletTime;
        constexpr double SecondsPerBullet = 0.05f;

        void Update();
        void Draw(BatchDraw2D& Draw2D);
        void Init();
        void Term();
    }

    struct BulletData
    {
        v2f Pos;
        v2f Vel;
        double SpawnTime;
    };

    struct PlayerBulletManager
    {
        static constexpr double BulletLifetime = 10.0;
        static constexpr float BulletSize = 10.0f;
        static constexpr float BaseBulletSpeed = 5.0f;
        static Array<BulletData> Bullets;
        static void SpawnBullet(v2f Pos, v2f Vel, double SpawnTime);
        static void Update();
        static bool IsOffscreen(v2f Pos);
    };

    Array<BulletData> PlayerBulletManager::Bullets{};

    bool PlayerBulletManager::IsOffscreen(v2f Pos)
    {
        return (Pos.X + BulletSize) < 0.0f ||
            AppWidth < (Pos.X - BulletSize) ||
            (Pos.Y + BulletSize) < 0.0f ||
            AppHeight < (Pos.Y - BulletSize);
    }

    void PlayerBulletManager::SpawnBullet(v2f Pos, v2f Vel, double SpawnTime)
    {
        Bullets.Add(BulletData{Pos, Vel, SpawnTime});
    }

    void PlayerBulletManager::Update()
    {
        double CurrTime = Clock::Time();
        for (int BulletIdx = 0; BulletIdx < Bullets.Num; BulletIdx++)
        {
            BulletData& CurrData = Bullets[BulletIdx];
            CurrData.Pos = CurrData.Pos + (CurrData.Vel * BaseBulletSpeed);
            if (CurrTime - Bullets[BulletIdx].SpawnTime > BulletLifetime|| IsOffscreen(Bullets[BulletIdx].Pos))
            {
                Bullets.Remove(BulletIdx--);
            }
        }
    }

    void BulletLimboState::Update()
    {
        PlayerBulletManager::Update();

        v2f LeftStick = GamepadState::GetLeftStick();
        v2f RightStick = GamepadState::GetRightStick();
        PlayerPos = PlayerPos + (LeftStick * PlayerSpeed);
        if (GamepadState::GetButton(LV_GAMEPAD_RIGHT_SHOULDER)) { }
        else if (Length(RightStick) > 0.01f){ PlayerAngle = atan2f(RightStick.Y, RightStick.X); }
        else if (Length(LeftStick) > 0.025f) { PlayerAngle = atan2f(LeftStick.Y, LeftStick.X); }

        double CurrTime = Clock::Time();
        if ((CurrTime - LastBulletTime) > SecondsPerBullet)
        {
            v2f BulletVel = { cosf(PlayerAngle), sinf(PlayerAngle) };
            PlayerBulletManager::SpawnBullet(PlayerPos + (BulletVel * 12.0f), BulletVel, CurrTime);
            LastBulletTime = CurrTime;
        }
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
        for (int BulletIdx = 0; BulletIdx < PlayerBulletManager::Bullets.Num; BulletIdx++)
        {
            BulletData& CurrBullet = PlayerBulletManager::Bullets[BulletIdx];
            Draw2D.AddRect(
                RectF{ CurrBullet.Pos.X, CurrBullet.Pos.Y, PlayerBulletManager::BulletSize, PlayerBulletManager::BulletSize },
                BulletTexRect,
                0.0f
            );
        }
    }
    void BulletLimboState::Init()
    {
        PlayerPos = { (float)AppWidth / 2.0f, (float)AppHeight / 2.0f };
        PlayerAngle = 0.0f;
        PlayerSpeed = 1.0f;
        LastBulletTime = Clock::Time();
    }
    void BulletLimboState::Term()
    {
    }

    void BulletLimbo::Update() { BulletLimboState::Update(); }

    void BulletLimbo::Draw(GameGraphicsContext& GFXContext)
    {
        BatchDraw2D& Draw2D = *GFXContext.Draw2D;
        BulletLimboState::Draw(Draw2D);
    }
    void BulletLimbo::Init() { BulletLimboState::Init(); }
    void BulletLimbo::Term() { BulletLimboState::Term(); }
}

