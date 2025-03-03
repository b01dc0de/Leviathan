#include "Camera.h"

namespace Leviathan
{
    void Camera::Ortho(float ResX, float ResY, float fDepth)
    {
        View = m4f::Identity();
        Proj = m4f::Identity();
        Proj.R0.X = +2.0f / ResX;
        Proj.R1.Y = +2.0f / ResY;
        View.R2.Z = -2.0f / fDepth;
    }

    void Camera::Persp(const v3f& InPos, const v3f& InLookAt)
    {
        View = m4f::Zero();
        Proj = m4f::Zero();

        {
            static const m4f NDC = Mult(m4f::Trans(v3f{0.0f, 0.0f, 1.0f}), m4f::Scale(1.0f, 1.0f, 0.5f));

            constexpr v3f AbsUp{ 0.0f, 1.0f, 0.0f };
            v3f Forward = Norm(-(InLookAt - InPos));
            v3f Right = Norm(Cross(AbsUp, Forward));
            v3f Up = Norm(Cross(Forward, Right));

            constexpr float fFOV = 45.0f;
            constexpr float fAspectRatio = 16.0f / 9.0f;
            const float fD = 1.0f / tanf(fFOV / 2.0f);

            constexpr float fNearDist = 1.0f;
            constexpr float fFarDist = 1000.0f;
            constexpr float fDistDelta = fFarDist - fNearDist;

            Proj.R0.X = fD / fAspectRatio;
            Proj.R1.Y = fD;
            Proj.R2.Z = -(fFarDist + fNearDist) / fDistDelta;
            Proj.R2.W = -1.0f;
            Proj.R3.Z = (-2.0f * fFarDist * fNearDist) / fDistDelta;
            Proj = Proj * NDC;

            View.R0 = { Right.X, Up.X, Forward.X, 0.0f };
            View.R1 = { Right.Y, Up.Y, Forward.Y, 0.0f };
            View.R2 = { Right.Z, Up.Z, Forward.Z, 0.0f };
            View.R3.X = -Dot(InPos, Right);
            View.R3.Y = -Dot(InPos, Up);
            View.R3.Z = -Dot(InPos, Forward);
            View.R3.W = 1.0f;
        }
    }


}

