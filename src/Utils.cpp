#include "Leviathan.h" // Utils.h included in Leviathan.h

namespace Leviathan
{
    RGBA Norm8Bit(unsigned char R, unsigned char G, unsigned char B)
    {
        float fR = (float)R / 255.0f;
        float fG = (float)G / 255.0f;
        float fB = (float)B / 255.0f;
        return RGBA{ fR, fG, fB, 1.0f };
    }

    int GetRandomInRange(int Min, int Max)
    {
        std::random_device RandomDevice{};
        std::mt19937 MersenneTwisterEng{ RandomDevice() };
        std::uniform_int_distribution<int> Distrib(Min, Max);
        return Distrib(MersenneTwisterEng);
    }
}
