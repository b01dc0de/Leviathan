#include "LvBitmapFile.h"

#include <cstdio>

namespace Lv
{
	RGBA32 InterpColor(RGBA32 ColorA, RGBA32 ColorB, float T)
	{
		if (T > 1.0f) { T = 1.0f; }
		else if (T < 0.0f) { T = 0.0f; }

		// T == 0.0: WeightA = 1.0, WeightB = 0.0
		// T == 1.0, WeightA = 0.0, WeightB = 1.0
		float WeightA = 1.0f - T;
		float WeightB = T;
		RGBA32 ColorResult = {};
		ColorResult.R = u8((ColorA.R * WeightA + ColorB.R * WeightB));
		ColorResult.G = u8((ColorA.G * WeightA + ColorB.G * WeightB));
		ColorResult.B = u8((ColorA.B * WeightA + ColorB.B * WeightB));
		ColorResult.A = 255;
		return (ColorResult);
	}

	void WriteBMP(Image32 Image, const char* FileName)
	{
		u32 PxBytes = sizeof(u32) * Image.PxCount;

		BMP BMP_Data = {};

		BMP_Data.FileHeader.Type = 0x4D42;
		BMP_Data.FileHeader.SizeInBytes = sizeof(BMP_Data) + PxBytes;
		BMP_Data.FileHeader.Res1 = 0;
		BMP_Data.FileHeader.Res2 = 0;
		BMP_Data.FileHeader.OffsetBytes = sizeof(BMP_Data);

		BMP_Data.InfoHeader.StructSize = sizeof(BMP_Data) - 14;
		BMP_Data.InfoHeader.Width = (s32)Image.Width;
		BMP_Data.InfoHeader.Height = -(s32)Image.Height;
		BMP_Data.InfoHeader.Planes = 1;
		BMP_Data.InfoHeader.BitsPerPixel = 32;
		BMP_Data.InfoHeader.Compression = 0;
		BMP_Data.InfoHeader.Unused_ImgSize = PxBytes;
		BMP_Data.InfoHeader.HRes = 0;
		BMP_Data.InfoHeader.VRes = 0;
		BMP_Data.InfoHeader.ColorsUsed = 0;
		BMP_Data.InfoHeader.ColorsImportant = 0;

		FILE* BMP_File = nullptr;
		fopen_s(&BMP_File, FileName, "wb");
		if (BMP_File != nullptr)
		{
			fwrite(&BMP_Data.FileHeader, sizeof(BMP_Data.FileHeader), 1, BMP_File);
			fwrite(&BMP_Data.InfoHeader, sizeof(BMP_Data.InfoHeader), 1, BMP_File);
			fwrite(Image.PixelBuffer, Image.PxBytes, 1, BMP_File);
			fclose(BMP_File);
		}
		else
		{
			printf("ERROR: Cannot open file \"%s\"\n", FileName);
		}
	}

	void ReadBMP(const char* InFilename, Image32 OutImage)
	{
		FILE* BMP_File = nullptr;
		fopen_s(&BMP_File, InFilename, "wb");

		if (nullptr != BMP_File)
		{

		}
		else
		{
			// CKA_TODO: Error handling
		}
	}

}

