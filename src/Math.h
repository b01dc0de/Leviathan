#ifndef LEVIATHAN_MATH_H
#define LEVIATHAN_MATH_H

namespace Leviathan
{
    constexpr float fPI = 3.141592653589793f;
    constexpr float fTAU = fPI * 2.0f;

    struct v2i
    {
        int X = 0;
        int Y = 0;
    };

    struct v2f
    {
        float X = 0.0f;
        float Y = 0.0f;
    };

    struct v3f
    {
        float X = 0.0f;
        float Y = 0.0f;
        float Z = 0.0f;
    };

    struct v4f
    {
        float X = 0.0f;
        float Y = 0.0f;
        float Z = 0.0f;
        float W = 0.0f;
    };

    struct m2f
    {
        v2f R0;
        v2f R1;
        static m2f Zero();
        static m2f Identity();
    };

    struct m3f
    {
        v3f R0;
        v3f R1;
        v3f R2;
        static m3f Zero();
        static m3f Identity();
    };

    struct m4f
    {
        v4f R0;
        v4f R1;
        v4f R2;
        v4f R3;
        static m4f Zero();
        static m4f Identity();
        static m4f Trans(float X, float Y, float Z);
        static m4f Trans(const v3f& InV);
        static m4f Scale(float X, float Y, float Z);
        static m4f Scale(float InS);
        static m4f Scale(const v3f& InV);
        static m4f RotAxisX(float Angle);
        static m4f RotAxisY(float Angle);
        static m4f RotAxisZ(float Angle);
        static m4f RotAxis(v3f Axis, float Angle);
    };

    bool Equals(const v2i& A, const v2i& B);
    v2i Add(const v2i& A, const v2i& B);
    v2i Subtract(const v2i& A, const v2i& B);
    v2i Mult(const v2i& A, int S);
    v2i Divide(const v2i& A, int D);
    v2i Remainder(const v2i& A, int D);
    bool operator==(const v2i& A, const v2i& B);
    bool operator!=(const v2i& A, const v2i& B);
    v2i operator+(const v2i& A, const v2i& B);
    v2i operator-(const v2i& A, const v2i& B);
    v2i operator*(const v2i& A, int S);
    v2i operator/(const v2i& A, int D);
    v2i operator%(const v2i& A, int D);

    float LengthSq(const v2f& A);
    float LengthSq(const v3f& A);
    float Length(const v2f& A);
    float Length(const v3f& A);

    v2f Norm(const v2f& A);
    v3f Norm(const v3f& A);

    v2f Add(const v2f& A, const v2f& B);
    v3f Add(const v3f& A, const v3f& B);
    v4f Add(const v4f& A, const v4f& B);
    v2f Subtract(const v2f& A, const v2f& B);
    v3f Subtract(const v3f& A, const v3f& B);
    v4f Subtract(const v4f& A, const v4f& B);
    v2f Mult(const v2f& A, float S);
    v3f Mult(const v3f& A, float S);
    v4f Mult(const v4f& A, float S);
    v2f operator+(const v2f& A, const v2f& B);
    v3f operator+(const v3f& A, const v3f& B);
    v4f operator+(const v4f& A, const v4f& B);
    v2f operator-(const v2f& A, const v2f& B);
    v3f operator-(const v3f& A, const v3f& B);
    v4f operator-(const v4f& A, const v4f& B);
    v2f operator*(const v2f& A, float S);
    v3f operator*(const v3f& A, float S);
    v4f operator*(const v4f& A, float S);

    v2f operator-(const v2f& A);
    v3f operator-(const v3f& A);

    float Dot(const v2f& A, const v2f& B);
    float Dot(const v3f& A, const v3f& B);
    float Dot(const v4f& A, const v4f& B);
    v3f Cross(const v3f& A, const v3f& B);

    v2f Mult(const m2f& M, const v2f& V);
    v3f Mult(const m3f& M, const v3f& V);
    v4f Mult(const m4f& M, const v4f& V);
    v2f operator*(const m2f& M, const v2f& V);
    v3f operator*(const m3f& M, const v3f& V);
    v4f operator*(const m4f& M, const v4f& V);

    m2f Mult(const m2f& A, const m2f& B);
    m3f Mult(const m3f& A, const m3f& B);
    m4f Mult(const m4f& A, const m4f& B);
    m2f operator*(const m2f& A, const m2f& B);
    m3f operator*(const m3f& A, const m3f& B);
    m4f operator*(const m4f& A, const m4f& B);

    int Min(int A, int B);
    int Max(int A, int B);
    int Abs(int A);
    float Min(float A, float B);
    float Max(float A, float B);
    float Abs(float A);

    template <typename T> T Min(T A, T B)
    {
        return B < A ? B : A;
    }
    template <typename T> T Max(T A, T B)
    {
        return B > A ? B : A;
    }
}

#endif // LEVIATHAN_MATH_H

