#ifndef LEVIATHAN_MATH_H
#define LEVIATHAN_MATH_H

namespace Leviathan
{
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

    struct m4f
    {
        v4f R0;
        v4f R1;
        v4f R2;
        v4f R3;
    };

    float LengthSq(const v2f& A);
    float LengthSq(const v3f& A);
    float Length(const v2f& A);
    float Length(const v3f& A);

    v2f Norm(const v2f& A);
    v3f Norm(const v3f& A);

    v2f Add(const v2f& A, const v2f& B);
    v3f Add(const v3f& A, const v3f& B);
    v2f Subtract(const v2f& A, const v2f& B);
    v3f Subtract(const v3f& A, const v3f& B);
    v2f operator+(const v2f& A, const v2f& B);
    v3f operator+(const v3f& A, const v3f& B);
    v2f operator-(const v2f& A, const v2f& B);
    v3f operator-(const v3f& A, const v3f& B);

    float Dot(const v2f& A, const v2f& B);
    float Dot(const v3f& A, const v3f& B);
    v3f Cross(const v3f& A, const v3f& B);
}

#endif // LEVIATHAN_MATH_H
