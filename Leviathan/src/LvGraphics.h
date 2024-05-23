#ifndef LVGRAPHICS_H
#define LVGRAPHICS_H

#include "LvGraphicsIncludes.h"
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