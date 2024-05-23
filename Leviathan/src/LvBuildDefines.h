#ifndef LVBUILDDEFINES_H
#define LVBUILDDEFINES_H

/* LV BUILD CONFIGURATION */
#if defined(DEBUG) || defined(_DEBUG)
	#define LV_CONFIG_DEBUG() (1)
	#define LV_CONFIG_SHIPPING() (0)
#else
	#define LV_CONFIG_DEBUG() (0)
	#define LV_CONFIG_SHIPPING() (1)
#endif // (defined(DEBUG) || defined(_DEBUG))
#define LV_BUILD_CONFIG_UNDEFINED() (!(LV_CONFIG_DEBUG() || LV_CONFIG_SHIPPING()))

/* LV BUILD PLATFORM */
#define LV_PLATFORM_WINDOWS() (_MSC_VER > 0)
#define LV_PLATFORM_LINUX() (!(LV_PLATFORM_WINDOWS()))
#define LV_BUILD_PLATFORM_UNDEFINED() (!(LV_PLATFORM_WINDOWS()) && !(LV_PLATFORM_LINUX()))

#if LV_BUILD_CONFIG_UNDEFINED() || LV_BUILD_PLATFORM_UNDEFINED()
#error "Must have defined build configuration and platform"
#endif // UNDEFINED LV BUILD

/* LV BUILD PLATFORM DEFINITIONS */
#if LV_PLATFORM_WINDOWS()
	#define LV_DBG_BREAK() DebugBreak()
	#define LV_PLATFORM_DBGPRINT(STR) OutputDebugStringA(STR)
	#define LV_PLATFORM_FUNCNAME() __FUNCSIG__
#elif LV_PLATFORM_LINUX()
	#define LV_DBG_BREAK() __builtin_trap()
	#define LV_PLATFORM_DBGPRINT(STR) printf(STR) // CKA_TODO: Find out correct Linux debug print call here
	#define LV_PLATFORM_FUNCNAME() __PRETTY_FUNCTION
#else
	#error "LV ERROR: NO BUILD PLATFORM DEFINED"
#endif /* LV BUILD PLATFORM DEFINITIONS */

/* LV BUILD CONFIG DEFINITIONS */
#if LV_CONFIG_DEBUG()
	#define LV_OUTF(...) Outf(__VA_ARGS__)
	#define LV_DBGOUTF(...) Outf(__VA_ARGS__)
	#define LV_DBGTRACESCOPE_STRUCTDEF() \
		struct __LvDbgTraceScopeStruct { \
			const char* __ContainingFuncName; \
			inline __LvDbgTraceScopeStruct(const char* InFuncName) \
				: __ContainingFuncName(InFuncName) { LV_DBGOUTF("%s -- BEGIN\n", __ContainingFuncName); } \
			inline ~__LvDbgTraceScopeStruct() { LV_DBGOUTF("%s -- END\n", __ContainingFuncName); } \
		}
	#define LV_DBGTRACESCOPE() \
			LV_DBGTRACESCOPE_STRUCTDEF() \
			__FuncLocalStruct(LV_PLATFORM_FUNCNAME())
	#define LV_ASSERT(exp) \
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
	#define LV_ASSERT(exp) \
		if (!(exp)) LV_OUTF("LV ERROR: Failed expression '" #exp "'\n\tIn Function: %s\nIn File: " __FILE__ "\tLine: %d \n", __func__, __LINE__)
	#define LV_FAIL() \
		LV_OUTF("LV FAIL: In Function: %s\nIn File: " __FILE__ "\tLine: %d \n", __func__, __LINE__); \
		*(int*)0 = 0xFF
#endif /* LV BUILD CONFIG DEFINITIONS */

/* LV COMMON MACRO DEFINITIONS */
#define LvAssert(exp) LV_ASSERT(exp)
#define Assert(exp) LV_ASSERT(exp)
#define LV_NOOP() (void)0
#define LV_UNUSED_VAR(var) (void)var
#define LV_SAFE_DEREF(ptr, member) (ptr == nullptr) ? nullptr : ptr->member // To stop MSVC from complaining
#define LV_ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define LV_MAKE_CSTR(x) #x

#endif // LVBUILDDEFINES_H
