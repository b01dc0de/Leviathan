#ifndef LEVIATHAN_IMAGE_H
#define LEVIATHAN_IMAGE_H

namespace Leviathan
{

struct RGBA32
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};

struct ImageT
{
    unsigned int Width;
    unsigned int Height;
    size_t PxCount;
    size_t PxBufferSize;
    RGBA32* PxBuffer;
};

void GetDebugImage(ImageT& OutImage);
void LoadBMPFile(const char* Filename, ImageT& OutImage);
void SafeRelease(ImageT& Image);

}

#endif // LEVIATHAN_IMAGE_H

