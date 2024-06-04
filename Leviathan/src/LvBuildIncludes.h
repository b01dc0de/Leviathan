#ifndef LVBUILDINCLUDES_H
#define LVBUILDINCLUDES_H

/* LV BUILD PLATFORM INCLUDES */
#if LV_PLATFORM_WINDOWS()
	#if LV_CONFIG_DEBUG()
		#define _CRTDBG_MAP_ALLOC
		// Memory leak watchdog
		#include <crtdbg.h>
	#endif
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	// Windows GameInput library
	#include <GameInput.h>
#elif LV_PLATFORM_LINUX() // N/A
#else // N/A
	#error "Build platform is undefined"
#endif /* LV BUILD PLATFORM INCLUDES */

/* C/C++ STANDARD LIBRARY INLCUDES */
#include <type_traits>
/* C/C++ STANDARD LIBRARY INLCUDES */

#endif // LVBUILDINCLUDES_H
