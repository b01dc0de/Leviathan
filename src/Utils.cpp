#include "Leviathan.h" // Utils.h included in Leviathan.h

namespace Leviathan
{
    FileContentsT LoadFileContents(const char* Filename)
    {
        FileContentsT Result = {};

        FILE* FileHandle = nullptr;
        fopen_s(&FileHandle, Filename, "rb");
        if (FileHandle)
        {
            fseek(FileHandle, 0, SEEK_END);
            long FileSize = ftell(FileHandle);
            fseek(FileHandle, 0, SEEK_SET);

            uchar* Buffer = new uchar[FileSize];
            fread_s(Buffer, FileSize, FileSize, 1, FileHandle);

            Result.Size = FileSize;
            Result.Contents = Buffer;

            fclose(FileHandle);
        }
        
        return Result;
    }

    int GetRandomInRange(int Min, int Max)
    {
        std::random_device RandomDevice{};
        std::mt19937 MersenneTwisterEng{ RandomDevice() };
        std::uniform_int_distribution<int> Distrib(Min, Max);
        return Distrib(MersenneTwisterEng);
    }
}
