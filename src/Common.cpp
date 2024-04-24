#include "Common.h"

int GenRandInRange(int Min, int Max)
{
	static std::random_device rand_dev;
	static std::mt19937 mersenne_twister_eng(rand_dev());
	static std::uniform_int_distribution<> uint_dist;

	Assert(0 <= Min && Min <= Max);

	uint_dist.param(std::uniform_int_distribution<>::param_type(Min, Max));
	return uint_dist(mersenne_twister_eng);
}
