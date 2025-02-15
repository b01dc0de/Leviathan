#include "Leviathan.h" // Utils.h included in Leviathan.h

namespace Leviathan
{
    int GetRandomInRange(int Min, int Max)
    {
        std::random_device RandomDevice{};
        std::mt19937 MersenneTwisterEng{ RandomDevice() };
        std::uniform_int_distribution<int> Distrib(Min, Max);
        return Distrib(MersenneTwisterEng);
    }
}
