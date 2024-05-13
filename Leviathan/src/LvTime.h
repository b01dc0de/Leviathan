#ifndef LVTIME_H
#define LVTIME_H

namespace Leviathan
{
	struct LvTime
	{
		__int64 Freq = 0;
		__int64 Epoch = 0;

		__int64 LastDelta = 0;
		__int64 LastFrameTime = 0;

		double ToDouble(__int64 Timestamp);

		void Init();
		void Measure_EngineFrame();
	};
}

#endif // LVTIME_H