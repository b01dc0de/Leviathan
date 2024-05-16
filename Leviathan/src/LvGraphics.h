#ifndef LVGRAPHICS_H
#define LVGRAPHICS_H

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgidebug.h>

#include "LvGraphicsTypes.h"
#include "LvGraphicsUtils.h"

namespace Leviathan
{
	namespace LvGraphics
	{
		void Init();
		void Term();
		void UpdateAndDraw();
	};
}

#endif // LVGRAPHICS_H