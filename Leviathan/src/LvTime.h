#ifndef LVTIME_H
#define LVTIME_H

namespace Leviathan
{
	struct LvTime
	{
		__int64 _Freq = 0;
		__int64 _EngineProcessEpoch = 0;

		__int64 _LatestDelta = 0;
		__int64 _LatestFrameTime = 0;

		double ToDouble(__int64 Timestamp);
		double Epoch();
		double Delta();
		double CurrTime();

		void Init();
		void Measure_EngineFrame();
	};
}

#endif // LVTIME_H