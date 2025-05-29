#ifndef LEVIATHAN_UTILS_H
#define LEVIATHAN_UTILS_H

/* Included in Leviathan.h */

namespace Leviathan
{

void Outf(const char* Fmt, ...);

struct fColor
{
    float R = 0.0f;
    float G = 0.0f;
    float B = 0.0f;
    float A = 0.0f;
};

constexpr fColor Norm8Bit(unsigned char R, unsigned char G, unsigned char B)
{
    float fR = (float)R / 255.0f;
    float fG = (float)G / 255.0f;
    float fB = (float)B / 255.0f;
    return fColor{ fR, fG, fB, 1.0f };
}

struct FileContentsT
{
    size_t Size = 0;
    uchar* Contents = nullptr;

    size_t Read(uchar*& ReadPtr, size_t DataSize, void* DstData);
};

FileContentsT LoadFileContents(const char* Filename, bool bNullTerm = false);

int GetRandomInRange(int Min, int Max);

}

#endif // LEVIATHAN_UTILS_H

