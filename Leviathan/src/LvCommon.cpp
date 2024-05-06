#include "LvCommon.h"

void Outf(const char* fmt, ...)
{
	constexpr size_t OutfBufferSize = 1024;
	char OutfBuffer[OutfBufferSize];

	va_list args;
	va_start(args, fmt);
	vsprintf_s(OutfBuffer, OutfBufferSize, fmt, args);
	OutputDebugStringA(OutfBuffer);
	va_end(args);
}

