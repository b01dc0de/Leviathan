#ifndef LVCOMMON_H
#define LVCOMMON_H

// LV MACRO DEFINITIONS
#include "LvBuildDefines.h"

namespace Leviathan
{
	// LV GLOBAL CONSTANTS DECLARATIONS
	extern const int ResX;
	extern const int ResY;
	extern const WCHAR* AppName;

	// LV GLOBAL FUNCTION DECLARATIONS
	void Outf(const char* fmt, ...);
}

// LV SHARED COMMON INCLUDES
#include "LvMath.h"

#endif // LVCOMMON_H