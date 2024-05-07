#include "LvCommon.h"

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

