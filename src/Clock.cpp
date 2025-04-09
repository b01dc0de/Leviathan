#include "Leviathan.h" // NOTE: Clock.h included in Leviathan.h

namespace Leviathan
{

using TimeT = __int64;

namespace ClockState
{
TimeT _Epoch = 0;
TimeT _Freq = 0;
double _FreqD = 0.0;

TimeT _CurrTime = 0;
TimeT _LastDelta = 0;

double CurrTime = 0.0;
double LastDelta = 0.0;
}

using namespace ClockState;

double ConvertTime(TimeT Ts)
{
    double Result = (double)Ts / _FreqD;
    return Result;
}

double Clock::Time()
{
    return CurrTime;
}

double Clock::DeltaTime()
{
    return LastDelta;
}

void Clock::Tick()
{
    LARGE_INTEGER TmpLG = {};
    QueryPerformanceCounter(&TmpLG);

    TimeT LastTime = _CurrTime;
    _CurrTime = TmpLG.QuadPart - _Epoch;
    _LastDelta = _CurrTime - LastTime;

    CurrTime = ConvertTime(_CurrTime);
    LastDelta = ConvertTime(_LastDelta);
}

void Clock::Init()
{
    LARGE_INTEGER TmpLG = {};

    QueryPerformanceFrequency(&TmpLG);
    _Freq = TmpLG.QuadPart;
    _FreqD = (double)_Freq;

    QueryPerformanceCounter(&TmpLG);
    _Epoch = TmpLG.QuadPart;

    _CurrTime = _Epoch;
}

}

