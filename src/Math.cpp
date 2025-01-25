#include "Leviathan.h" // NOTE: Math.h included in Leviathan.h

namespace Leviathan
{
    float Length(const v2f& A)
    {
        float Result = sqrtf(A.X * A.X + A.Y * A.Y);
        return Result;
    }
    float Length(const v3f& A)
    {
        float Result = sqrtf(A.X*A.X + A.Y*A.Y + A.Z*A.Z);
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

    v3f Subtract(const v3f& A, const v3f& B)
    {
        v3f Result{ A.X - B.X, A.Y - B.Y, A.Z - B.Z };
        return Result;
    }
    v2f Subtract(const v2f& A, const v2f& B)
    {
        v2f Result{ A.X - B.X, A.Y - B.Y };
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
