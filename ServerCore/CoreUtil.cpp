#include "pch.h"
#include "CoreUtil.h"

std::mt19937 CoreUtil::s_generator = std::mt19937(std::random_device{}());

int32 CoreUtil::GetRandom(int32 min, int32 max)
{
	std::uniform_int_distribution<int32> distribution(min, max);
	return distribution(s_generator);
}