#ifndef ETCETERA_H
#define ETCETERA_H

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using s32 = int;

struct RGBA32
{
    u8 R;
    u8 G;
    u8 B;
    u8 A;

    u32 SwizzleAsU32()
    {
        return u32((A << 24) | (B << 16) | (G << 8) | (R << 0));
    }
};

struct Image32
{
    u32 Width;
    u32 Height;
    u32 PxCount;
    u32 PxBytes;
    RGBA32* PixelBuffer;
};

#pragma pack(push, 1)
struct BMPFileHeader
{
    u16 Type; // Always Ascii BM
    u32 SizeInBytes; // Size (bytes) of file
    u16 Res1; // 0
    u16 Res2; // 0
    u32 OffsetBytes; // Offset (bytes) to actual pixel data
};

struct BMPInfoHeader
{
    u32 StructSize; // Size (bytes) of InfoHeader
    s32 Width;
    s32 Height; // NOTE(chris): If positive, pixel data is bottom to top
    u16 Planes; // Must be 1
    u16 BitsPerPixel; // Bits-per-pixel (0, 1, 4, 8, 16, 24, 32)
    u32 Compression; // *Should* be 0
    u32 Unused_ImgSize; // Only used if Compression is weird (not 0)
    s32 HRes; // Horizontal resolution
    s32 VRes; // Vertical resolution
    u32 ColorsUsed; // 0 for our purposes
    u32 ColorsImportant; // 0 for our purposes
};
#pragma pack(pop)

struct BMP
{
    BMPFileHeader FileHeader;
    BMPInfoHeader InfoHeader;
};

#endif // ETCETERA_H
