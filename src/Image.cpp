#include "Image.h"

namespace Leviathan
{
    void GetDebugImage(ImageT& OutImage)
    {
        unsigned int DebugImageLength = 16;
        OutImage.Width = DebugImageLength;
        OutImage.Height = DebugImageLength;
        OutImage.PxCount = OutImage.Width * OutImage.Height;
        OutImage.PxBytes = sizeof(RGBA32) * OutImage.PxCount;
        OutImage.PxBuffer = new RGBA32[OutImage.PxCount];

        const RGBA32 Pink{ 255u, 73u, 173u, 255u };
        const RGBA32 Black{ 0u, 0u, 0u, 255u };
        const RGBA32 Red{ 255u, 0u, 0u, 255u };
        const RGBA32 Green{ 0u, 255u, 0u, 255u };
        const RGBA32 Blue{ 0u, 0u, 255u, 255u };
        const RGBA32 White{ 255u, 255u, 255u, 255u };
        for (int PxIdx = 0; PxIdx < OutImage.PxCount; PxIdx++)
        {
            int PxRow = PxIdx / OutImage.Width;
            int PxCol = PxIdx % OutImage.Width;
            if (PxRow == 0 && PxCol == 0)
            {
                OutImage.PxBuffer[PxIdx] = Red;
            }
            else if (PxRow == 0 && PxCol == (OutImage.Width - 1))
            {
                OutImage.PxBuffer[PxIdx] = Green;
            }
            else if (PxRow == (OutImage.Height - 1) && PxCol == 0)
            {
                OutImage.PxBuffer[PxIdx] = Blue;
            }
            else if (PxRow == (OutImage.Height - 1) && PxCol == (OutImage.Width - 1))
            {
                OutImage.PxBuffer[PxIdx] = White;
            }
            else
            {
                bool bEvenCell = (PxRow + PxCol) % 2 == 0;
                OutImage.PxBuffer[PxIdx] = bEvenCell ? Black : Pink;
            }
        }
    }
}