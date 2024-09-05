#include "BitmapTest.h"
#include "LvBitmapFile.h"

#include <cstdio>

using Image32 = Lv::Image32;
using RGBA32 = Lv::RGBA32;

int BitmapTest_Main(int argc, const char* argv[])
{
	u32 ImgSize = 512;
	Image32 ImgData = {};
	ImgData.Width = ImgSize;
	ImgData.Height = ImgSize;
	ImgData.PxCount = ImgData.Width * ImgData.Height;
	ImgData.PxBytes = ImgData.PxCount * sizeof(RGBA32);
	ImgData.PixelBuffer = new RGBA32[ImgSize * ImgSize];

	const RGBA32 Pink{ 255, 73, 173, 255 };
	const RGBA32 Black{ 0, 0, 0, 255 };

	constexpr UINT Size = 512;
	UINT CellSize = Size / 16;
	for (UINT RowIdx = 0; RowIdx < Size; RowIdx++)
	{
		for (UINT ColIdx = 0; ColIdx < Size; ColIdx++)
		{
			int CellRow = RowIdx / CellSize;
			int CellCol = ColIdx / CellSize;
			bool bEvenCell = ((CellRow + CellCol) % 2 == 0);
			RGBA32 SelectedColor = bEvenCell ? Pink : Black;
			ImgData.PixelBuffer[(RowIdx * Size) + ColIdx] = { SelectedColor.B, SelectedColor.G, SelectedColor.R, SelectedColor.A };
		}
	}

	/*
		CKA_NOTE:
			  RED       0x00FF0000
			  GREEN     0x0000FF00
			  BLUE      0x000000FF
	*/
	Lv::WriteBMP("Test.bmp", ImgData);

	return(0);
}
