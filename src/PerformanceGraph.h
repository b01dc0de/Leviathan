#ifndef LEVIATHAN_PERFORMANCEGRAPH_H
#define LEVIATHAN_PERFORMANCEGRAPH_H

#include "Graphics.h"

namespace Leviathan
{
    namespace PerformanceGraph
    {
        void Tick();
        void Draw(BatchDraw2D& Draw2D);
    };
}

#endif // LEVIATHAN_PERFORMANCEGRAPH_H