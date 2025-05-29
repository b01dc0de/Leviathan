#include "Leviathan.h" // Utils.h included in Leviathan.h

namespace Leviathan
{

void Outf(const char* Fmt, ...)
{
    constexpr size_t MsgBufferSize = 1024;
    char MsgBuffer[MsgBufferSize];
    va_list args;
    va_start(args, Fmt);
    vsprintf_s(MsgBuffer, MsgBufferSize, Fmt, args);
    va_end(args);
    OutputDebugStringA(MsgBuffer);
}

FileContentsT LoadFileContents(const char* Filename, bool bNullTerm)
{
    FileContentsT Result = {};

    FILE* FileHandle = nullptr;
    fopen_s(&FileHandle, Filename, "rb");
    if (FileHandle)
    {
        fseek(FileHandle, 0, SEEK_END);
        long FileSize = ftell(FileHandle);
        fseek(FileHandle, 0, SEEK_SET);

        uchar* Buffer = nullptr;

        if (bNullTerm)
        {
            Buffer = new uchar[FileSize + 1];
            fread_s(Buffer, FileSize, FileSize, 1, FileHandle);
            Buffer[FileSize] = '\0';
        }
        else
        {
            Buffer = new uchar[FileSize];
            fread_s(Buffer, FileSize, FileSize, 1, FileHandle);
        }


        Result.Size = FileSize;
        Result.Contents = Buffer;

        fclose(FileHandle);
    }

    return Result;
}

size_t FileContentsT::Read(uchar*& ReadPtr, size_t DataSize, void* DstData)
{
    bool bReadPtrInRange = Contents <= ReadPtr && ReadPtr <= Contents + Size;
    ASSERT(bReadPtrInRange);
    if (!bReadPtrInRange) { return 0; }

    size_t ReadSize = Min(DataSize, (size_t)(Contents + Size - ReadPtr));
    memcpy(DstData, ReadPtr, ReadSize);
    ReadPtr += ReadSize;
    return ReadSize;
}

int GetRandomInRange(int Min, int Max)
{
    std::random_device RandomDevice{};
    std::mt19937 MersenneTwisterEng{ RandomDevice() };
    std::uniform_int_distribution<int> Distrib(Min, Max);
    return Distrib(MersenneTwisterEng);
}

}

