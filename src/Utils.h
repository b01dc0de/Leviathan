#ifndef LEVIATHAN_UTILS_H
#define LEVIATHAN_UTILS_H

/* Included in Leviathan.h */

namespace Leviathan
{
    struct RGBA
    {
        float R = 0.0f;
        float G = 0.0f;
        float B = 0.0f;
        float A = 0.0f;
    };

    RGBA Norm8Bit(unsigned char R, unsigned char G, unsigned char B);

    int GetRandomInRange(int Min, int Max);
}

#endif // LEVIATHAN_UTILS_H
