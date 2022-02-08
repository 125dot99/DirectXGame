#pragma once
#include <random>

namespace gamelib
{

template<typename T>
inline T Random(const T& random)
{
	return (T)rand() / RAND_MAX * random - random / (T)2;
}

template<typename T>
inline T Random(const T& minRam, const T& maxRam)
{
	return minRam + (T)(rand() * (maxRam - minRam + 1) / (1 + RAND_MAX));
}

} // namespace gamelib