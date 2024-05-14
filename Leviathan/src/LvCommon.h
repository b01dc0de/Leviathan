#ifndef LVCOMMON_H
#define LVCOMMON_H

// Build configuration:
#define LV_CONFIG_DEBUG() (defined(DEBUG) || defined(_DEBUG))
#define LV_CONFIG_SHIPPING() (!LV_CONFIG_DEBUG())
#define LV_BUILD_CONFIG_UNDEFINED() (!(LV_CONFIG_DEBUG() || LV_CONFIG_SHIPPING()))

// Build platform:
#define LV_PLATFORM_WINDOWS() (_MSC_VER > 0)
#define LV_PLATFORM_LINUX() (!(LV_PLATFORM_WINDOWS()))
#define LV_BUILD_PLATFORM_UNDEFINED() (!(LV_PLATFORM_WINDOWS()) && !(LV_PLATFORM_LINUX()))

#if LV_BUILD_CONFIG_UNDEFINED() || LV_BUILD_PLATFORM_UNDEFINED()
#error "Must have defined build configuration and platform"
#endif // UNDEFINED LV BUILD


/* LV BUILD PLATFORM INCLUDES */
#if LV_PLATFORM_WINDOWS()
	#if LV_CONFIG_DEBUG()
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
	#endif
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif LV_PLATFORM_LINUX() // N/A
#else // N/A
#endif /* LV BUILD PLATFORM INCLUDES */

/* LV BUILD PLATFORM DEFINITIONS */
#if LV_PLATFORM_WINDOWS()
	#define LV_DBG_BREAK() DebugBreak()
	#define LV_PLATFORM_DBGPRINT(STR) OutputDebugStringA(STR)
#elif LV_PLATFORM_LINUX()
	#define LV_DBG_BREAK() __builtin_trap()
	#define LV_PLATFORM_DBGPRINT(STR) printf(STR) // CKA_TODO: Find out correct Linux debug print call here
#else
	#error "LV ERROR: NO BUILD PLATFORM DEFINED"
#endif /* LV BUILD PLATFORM DEFINITIONS */

/* LV BUILD CONFIG DEFINITIONS */
#if LV_CONFIG_DEBUG()
	#define LV_OUTF(...) Outf(__VA_ARGS__)
	#define LV_DBGOUTF(...) Outf(__VA_ARGS__)
	#define LV_DBGTRACESCOPE() \
		struct __LvDbgTraceScopeStruct \
		{ \
			const char* __ContainingFuncName; \
			inline __LvDbgTraceScopeStruct(const char* InFuncName) \
				: __ContainingFuncName(InFuncName) { LV_DBGOUTF("%s -- BEGIN\n", __ContainingFuncName); } \
			inline ~__LvDbgTraceScopeStruct() { LV_DBGOUTF("%s -- END\n", __ContainingFuncName); } \
		} __FuncLocalStruct(__func__);
	#define Assert(exp) \
		if (!(exp))	\
		{ \
			LV_OUTF("Failed expression: '" #exp "'\n\tIn Function: %s\nIn File: " __FILE__ "\tLine: %d \n", __func__, __LINE__); \
			LV_DBG_BREAK(); \
		}
	#define LV_FAIL() \
		LV_OUTF("LV FAIL: In Function: %s\nIn File: " __FILE__ "\tLine: %d \n", __func__, __LINE__); \
		LV_DBG_BREAK()
#else
	#define LV_OUTF(...) Outf(__VA_ARGS__)
	#define LV_DBGOUTF(...) LV_NOOP()
	#define LV_DBGTRACESCOPE() LV_NOOP()
	#define Assert(exp) \
		if (!(exp)) LV_OUTF("LV ERROR: Failed expression '" #exp "'\n\tIn Function: %s\nIn File: " __FILE__ "\tLine: %d \n", __func__, __LINE__)
	#define LV_FAIL() \
		LV_OUTF("LV FAIL: In Function: %s\nIn File: " __FILE__ "\tLine: %d \n", __func__, __LINE__); \
		*(int*)0 = 0xFF
#endif /* LV BUILD CONFIG DEFINITIONS */

/* LV COMMON MACRO DEFINITIONS */
#define LV_NOOP() (void)0
#define LV_UNUSED_VAR(var) (void)var
// The following is to stop MSVC from complaining
#define LV_SAFE_DEREF(ptr, member) (ptr == nullptr) ? nullptr : ptr->member

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