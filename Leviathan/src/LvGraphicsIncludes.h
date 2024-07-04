//#ifndef LVGRAPHICS_H
//#error "LvGraphicsIncludes.h must only be included with LvGraphics.h"
//#endif // CHECK FOR LVGRAPHICS_H
#ifndef LVGRAPHICSINCLUDES_H
#define LVGRAPHICSINCLUDES_H

#if LV_PLATFORM_WINDOWS()
	// DirectX11 headers
	#include <d3dcommon.h>
	#include <d3dcompiler.h>
	#include <dxgi.h>
	#include <dxgidebug.h>
	#include <d3d11.h>
	#include <d3d11_1.h>
	#include <dxgi1_2.h>
	#include <dwrite.h>
	#include <d2d1.h>
#elif LV_PLATFORM_LINUX()
	// TBD: This will likely end up being Vulkan headers
#else
	#error "Build platform is undefined"
#endif // LV_PLATFORM

#endif // LVGRAPHICSINCLUDES_H