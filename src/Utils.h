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

    constexpr RGBA Norm8Bit(unsigned char R, unsigned char G, unsigned char B)
    {
        float fR = (float)R / 255.0f;
        float fG = (float)G / 255.0f;
        float fB = (float)B / 255.0f;
        return RGBA{ fR, fG, fB, 1.0f };
    }

    struct FileContentsT
    {
        size_t Size = 0;
        uchar* Contents = nullptr;
    };

    FileContentsT LoadFileContents(const char* Filename);

    int GetRandomInRange(int Min, int Max);
}

#endif // LEVIATHAN_UTILS_H
