#ifndef LVTIME_H
#define LVTIME_H

namespace Leviathan
{
	struct LvTime
	{
		__int64 QP_Freq;
		__int64 QP_Epoch;

		double fCurrTime;
		double fPrevTime;
		double fDelta;

		double Delta();
		double CurrTime();

		void Init();
		void Measure_EngineFrame();
	};
}

#endif // LVTIME_H