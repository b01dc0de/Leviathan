#include "LvCommon.h"

#include <cstdio>

void Outf(const char* Fmt, ...)
{
	constexpr size_t OutfBufferSize = 1024;
	char OutfBuffer[OutfBufferSize];

	va_list args;
	va_start(args, Fmt);
	vsprintf_s(OutfBuffer, OutfBufferSize, Fmt, args);
	va_end(args);
	OutputDebugStringA(OutfBuffer);
}

namespace Leviathan
{
	extern const int ResX = 1600;
	extern const int ResY = 900;
	extern const WCHAR* AppName = L"Leviathan";
}
