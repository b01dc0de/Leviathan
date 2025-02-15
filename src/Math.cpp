#include "Leviathan.h" // NOTE: Math.h included in Leviathan.h

namespace Leviathan
{
    m2f m2f::Zero()
    {
        m2f Result;
        Result.R0 = { 0.0f, 0.0f };
        Result.R1 = { 0.0f, 0.0f };
        return Result;
    }

    m2f m2f::Identity()
    {
        m2f Result;
        Result.R0 = { 1.0f, 0.0f };
        Result.R1 = { 0.0f, 1.0f };
        return Result;
    }

    m3f m3f::Zero()
    {
        m3f Result;
        Result.R0 = { 0.0f, 0.0f, 0.0f };
        Result.R1 = { 0.0f, 0.0f, 0.0f };
        Result.R2 = { 0.0f, 0.0f, 0.0f };
        return Result;
    }

    m3f m3f::Identity()
    {
        m3f Result;
        Result.R0 = { 1.0f, 0.0f, 0.0f };
        Result.R1 = { 0.0f, 1.0f, 0.0f };
        Result.R2 = { 0.0f, 0.0f, 1.0f };
        return Result;
    }

    m4f m4f::Zero()
    {
        m4f Result;
        Result.R0 = { 0.0f, 0.0f, 0.0f, 0.0f };
        Result.R1 = { 0.0f, 0.0f, 0.0f, 0.0f };
        Result.R2 = { 0.0f, 0.0f, 0.0f, 0.0f };
        Result.R3 = { 0.0f, 0.0f, 0.0f, 0.0f };
        return Result;
    }

    m4f m4f::Identity()
    {
        m4f Result;
        Result.R0 = { 1.0f, 0.0f, 0.0f, 0.0f };
        Result.R1 = { 0.0f, 1.0f, 0.0f, 0.0f };
        Result.R2 = { 0.0f, 0.0f, 1.0f, 0.0f };
        Result.R3 = { 0.0f, 0.0f, 0.0f, 1.0f };
        return Result;
    }

    m4f m4f::Trans(float X, float Y, float Z)
    {
        m4f Result = Identity();
        Result.R3 = { X, Y, Z, 1.0f };
        return Result;
    }

    m4f m4f::Trans(const v3f& InV)
    {
        return Trans(InV.X, InV.Y, InV.Z);
    }

    m4f m4f::Scale(float X, float Y, float Z)
    {
        m4f Result = Identity();
        Result.R0.X = X;
        Result.R1.Y = Y;
        Result.R2.Z = Z;
        return Result;
    }

    m4f m4f::Scale(float InS)
    {
        return Scale(InS, InS, InS);
    }

    m4f m4f::Scale(const v3f& InV)
    {
        return Scale(InV.X, InV.Y, InV.Z);
    }

    m4f m4f::RotAxisX(float Angle)
    {
		const float fCos = cosf(Angle);
		const float fSin = sinf(Angle);

        m4f Result = Identity();
        Result.R0 = { 1.0f,  0.0f,  0.0f, 0.0f };
        Result.R1 = { 0.0f,  fCos, -fSin, 0.0f };
        Result.R2 = { 0.0f,  fSin,  fCos, 0.0f };
        return Result;
    }

    m4f m4f::RotAxisY(float Angle)
    {
		const float fCos = cosf(Angle);
		const float fSin = sinf(Angle);

        m4f Result = Identity();
        Result.R0 = {  fCos, 0.0f,  fSin, 0.0f };
        Result.R1 = {  0.0f, 1.0f,  0.0f, 0.0f };
        Result.R2 = { -fSin, 0.0f,  fCos, 0.0f };
        return Result;
    }

    m4f m4f::RotAxisZ(float Angle)
    {
		const float fCos = cosf(Angle);
		const float fSin = sinf(Angle);

        m4f Result = Identity();
        Result.R0 = { fCos,  -fSin, 0.0f, 0.0f };
        Result.R1 = { fSin,   fCos, 0.0f, 0.0f };
        Result.R2 = { 0.0f,   0.0f, 1.0f, 0.0f };
        return Result;
    }

    bool Equals(const v2i& A, const v2i& B)
    {
        return A.X == B.X && A.Y == B.Y;
    }
    v2i Add(const v2i& A, const v2i& B)
    {
        v2i Result{ A.X + B.X, A.Y + B.Y };
        return Result;
    }
    v2i Subtract(const v2i& A, const v2i& B)
    {
        v2i Result{ A.X - B.X, A.Y - B.Y };
        return Result;
    }
    v2i Mult(const v2i& A, int S)
    {
        v2i Result{ A.X * S, A.Y * S };
        return Result;
    }
    v2i Divide(const v2i& A, int D)
    {
        v2i Result{ A.X / D, A.Y / D };
        return Result;
    }
    v2i Remainder(const v2i& A, int D)
    {
        v2i Result{ A.X % D, A.Y % D };
        return Result;
    }
    bool operator==(const v2i& A, const v2i& B)
    {
        return Equals(A, B);
    }
    bool operator!=(const v2i& A, const v2i& B)
    {
        return !Equals(A, B);
    }
    v2i operator+(const v2i& A, const v2i& B)
    {
        return Add(A, B);
    }
    v2i operator-(const v2i& A, const v2i& B)
    {
        return Subtract(A, B);
    }
    v2i operator*(const v2i& A, int S)
    {
        return Mult(A, S);
    }
    v2i operator/(const v2i& A, int D)
    {
        return Divide(A, D);;
    }
    v2i operator%(const v2i& A, int D)
    {
        return Remainder(A, D);
    }

    float LengthSq(const v2f& A)
    {
        float Result = A.X*A.X + A.Y*A.Y;
        return Result;
    }
    float LengthSq(const v3f& A)
    {
        float Result = A.X*A.X + A.Y*A.Y + A.Z*A.Z;
        return Result;
    }

    float Length(const v2f& A)
    {
        float Result = sqrtf(LengthSq(A));
        return Result;
    }
    float Length(const v3f& A)
    {
        float Result = sqrtf(LengthSq(A));
        return Result;
    }

    v2f Norm(const v2f& A)
    {
        // TODO: Protect against divide by zero!
        float fLength = Length(A);
        v2f Result{ A.X / fLength, A.Y / fLength };
        return Result;
    }
    v3f Norm(const v3f& A)
    {
        // TODO: Protect against divide by zero!
        float fLength = Length(A);
        v3f Result{ A.X / fLength, A.Y / fLength, A.Z / fLength };
        return Result;
    }

    v2f Add(const v2f& A, const v2f& B)
    {
        v2f Result{ A.X + B.X, A.Y + B.Y };
        return Result;
    }
    v3f Add(const v3f& A, const v3f& B)
    {
        v3f Result{ A.X + B.X, A.Y + B.Y, A.Z + B.Z };
        return Result;
    }
    v4f Add(const v4f& A, const v4f& B)
    {
        v4f Result{ A.X + B.X, A.Y + B.Y, A.Z + B.Z, A.W + B.W };
        return Result;
    }
    v2f Subtract(const v2f& A, const v2f& B)
    {
        v2f Result{ A.X - B.X, A.Y - B.Y };
        return Result;
    }
    v3f Subtract(const v3f& A, const v3f& B)
    {
        v3f Result{ A.X - B.X, A.Y - B.Y, A.Z - B.Z };
        return Result;
    }
    v4f Subtract(const v4f& A, const v4f& B)
    {
        v4f Result{ A.X - B.X, A.Y - B.Y, A.Z - B.Z, A.W - B.W };
        return Result;
    }

    v2f operator+(const v2f& A, const v2f& B)
    {
        return Add(A, B);
    }
    v3f operator+(const v3f& A, const v3f& B)
    {
        return Add(A, B);
    }
    v4f operator+(const v4f& A, const v4f& B)
    {
        return Add(A, B);
    }

    v2f operator-(const v2f& A, const v2f& B)
    {
        return Subtract(A, B);
    }
    v3f operator-(const v3f& A, const v3f& B)
    {
        return Subtract(A, B);
    }
    v4f operator-(const v4f& A, const v4f& B)
    {
        return Subtract(A, B);
    }

    v2f operator-(const v2f& A)
    {
        v2f Result{ -A.X, -A.Y };
        return Result;
    }

    v3f operator-(const v3f& A)
    {
        v3f Result{ -A.X, -A.Y, -A.Z };
        return Result;
    }

    float Dot(const v2f& A, const v2f& B)
    {
        float Result = A.X*B.X + A.Y*B.Y;
        return Result;
    }

    float Dot(const v3f& A, const v3f& B)
    {
        float Result = A.X*B.X + A.Y*B.Y + A.Z*B.Z;
        return Result;
    }

    float Dot(const v4f& A, const v4f& B)
    {
        float Result = A.X*B.X + A.Y*B.Y + A.Z*B.Z + A.W*B.W;
        return Result;
    }

    v3f Cross(const v3f& A, const v3f& B)
    {
        v3f Result = {
            A.Y*B.Z - A.Z*B.Y,
            A.Z*B.X - A.X*B.Z,
            A.X*B.Y - A.Y*B.X,
        };
        return Result;
    }

    v2f Mult(const m2f& M, const v2f& V)
    {
        v2f Result;
        Result.X = Dot(M.R0, V);
        Result.Y = Dot(M.R1, V);
        return Result;
    }

    v3f Mult(const m3f& M, const v3f& V)
    {
        v3f Result;
        Result.X = Dot(M.R0, V);
        Result.Y = Dot(M.R1, V);
        Result.Z = Dot(M.R2, V);
        return Result;
    }

    v4f Mult(const m4f& M, const v4f& V)
    {
        v4f Result;
        Result.X = Dot(M.R0, V);
        Result.Y = Dot(M.R1, V);
        Result.Z = Dot(M.R2, V);
        Result.W = Dot(M.R3, V);
        return Result;
    }

    v2f operator*(const m2f& M, const v2f& V)
    {
        return Mult(M, V);
    }

    v3f operator*(const m3f& M, const v3f& V)
    {
        return Mult(M, V);
    }

    v4f operator*(const m4f& M, const v4f& V)
    {
        return Mult(M, V);
    }

    m2f Mult(const m2f& A, const m2f& B)
    {
        m2f Result;

        Result.R0.X = A.R0.X * B.R0.X + A.R0.Y * B.R1.X;
        Result.R0.Y = A.R0.X * B.R0.Y + A.R0.Y * B.R1.Y;

        Result.R1.X = A.R1.X * B.R0.X + A.R1.Y * B.R1.X;
        Result.R1.Y = A.R1.X * B.R0.Y + A.R1.Y * B.R1.Y;

        return Result;
    }

    m3f Mult(const m3f& A, const m3f& B)
    {
        m3f Result;

        Result.R0.X = A.R0.X * B.R0.X + A.R0.Y * B.R1.X + A.R0.Z * B.R2.X;
        Result.R0.Y = A.R0.X * B.R0.Y + A.R0.Y * B.R1.Y + A.R0.Z * B.R2.Y;
        Result.R0.Z = A.R0.X * B.R0.Z + A.R0.Y * B.R1.Z + A.R0.Z * B.R2.Z;

        Result.R1.X = A.R1.X * B.R0.X + A.R1.Y * B.R1.X + A.R1.Z * B.R2.X;
        Result.R1.Y = A.R1.X * B.R0.Y + A.R1.Y * B.R1.Y + A.R1.Z * B.R2.Y;
        Result.R1.Z = A.R1.X * B.R0.Z + A.R1.Y * B.R1.Z + A.R1.Z * B.R2.Z;

        Result.R2.X = A.R2.X * B.R0.X + A.R2.Y * B.R1.X + A.R2.Z * B.R2.X;
        Result.R2.Y = A.R2.X * B.R0.Y + A.R2.Y * B.R1.Y + A.R2.Z * B.R2.Y;
        Result.R2.Z = A.R2.X * B.R0.Z + A.R2.Y * B.R1.Z + A.R2.Z * B.R2.Z;

        return Result;
    }

    m4f Mult(const m4f& A, const m4f& B)
    {
        m4f Result;

        Result.R0.X = A.R0.X * B.R0.X + A.R0.Y * B.R1.X + A.R0.Z * B.R2.X + A.R0.W * B.R3.X;
        Result.R0.Y = A.R0.X * B.R0.Y + A.R0.Y * B.R1.Y + A.R0.Z * B.R2.Y + A.R0.W * B.R3.Y;
        Result.R0.Z = A.R0.X * B.R0.Z + A.R0.Y * B.R1.Z + A.R0.Z * B.R2.Z + A.R0.W * B.R3.Z;
        Result.R0.W = A.R0.X * B.R0.W + A.R0.Y * B.R1.W + A.R0.Z * B.R2.W + A.R0.W * B.R3.W;

        Result.R1.X = A.R1.X * B.R0.X + A.R1.Y * B.R1.X + A.R1.Z * B.R2.X + A.R1.W * B.R3.X;
        Result.R1.Y = A.R1.X * B.R0.Y + A.R1.Y * B.R1.Y + A.R1.Z * B.R2.Y + A.R1.W * B.R3.Y;
        Result.R1.Z = A.R1.X * B.R0.Z + A.R1.Y * B.R1.Z + A.R1.Z * B.R2.Z + A.R1.W * B.R3.Z;
        Result.R1.W = A.R1.X * B.R0.W + A.R1.Y * B.R1.W + A.R1.Z * B.R2.W + A.R1.W * B.R3.W;

        Result.R2.X = A.R2.X * B.R0.X + A.R2.Y * B.R1.X + A.R2.Z * B.R2.X + A.R2.W * B.R3.X;
        Result.R2.Y = A.R2.X * B.R0.Y + A.R2.Y * B.R1.Y + A.R2.Z * B.R2.Y + A.R2.W * B.R3.Y;
        Result.R2.Z = A.R2.X * B.R0.Z + A.R2.Y * B.R1.Z + A.R2.Z * B.R2.Z + A.R2.W * B.R3.Z;
        Result.R2.W = A.R2.X * B.R0.W + A.R2.Y * B.R1.W + A.R2.Z * B.R2.W + A.R2.W * B.R3.W;

        Result.R3.X = A.R3.X * B.R0.X + A.R3.Y * B.R1.X + A.R3.Z * B.R2.X + A.R3.W * B.R3.X;
        Result.R3.Y = A.R3.X * B.R0.Y + A.R3.Y * B.R1.Y + A.R3.Z * B.R2.Y + A.R3.W * B.R3.Y;
        Result.R3.Z = A.R3.X * B.R0.Z + A.R3.Y * B.R1.Z + A.R3.Z * B.R2.Z + A.R3.W * B.R3.Z;
        Result.R3.W = A.R3.X * B.R0.W + A.R3.Y * B.R1.W + A.R3.Z * B.R2.W + A.R3.W * B.R3.W;

        return Result;
    }

    m2f operator*(const m2f& A, const m2f& B)
    {
        return Mult(A, B);
    }

    m3f operator*(const m3f& A, const m3f& B)
    {
        return Mult(A, B);
    }

    m4f operator*(const m4f& A, const m4f& B)
    {
        return Mult(A, B);
    }

    int Min(int A, int B)
    {
        return B < A ? B : A;
    }
    int Max(int A, int B)
    {
        return B > A ? B : A;
    }
    int Abs(int A)
    {
        return A < 0 ? -A : A;
    }
    float Min(float A, float B)
    {
        return B < A ? B : A;
    }
    float Max(float A, float B)
    {
        return B > A ? B : A;
    }
    float Abs(float A)
    {
        return A < 0 ? -A : A;
    }
}
