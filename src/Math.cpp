#include "Leviathan.h" // NOTE: Math.h included in Leviathan.h

namespace Leviathan
{
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

    v3f Cross(const v3f& A, const v3f& B)
    {
        v3f Result = {
            A.Y*B.Z - A.Z*B.Y,
            A.X*B.Z - A.Z*B.X,
            A.X*B.Y - A.Y*B.X,
        };
        return Result;
    }
}
