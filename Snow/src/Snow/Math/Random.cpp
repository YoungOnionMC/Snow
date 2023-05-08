#include "Snow/Math/Random.h"

#include <random>

namespace Snow::Math {
	std::random_device Random::s_RandomDevice;
	std::mt19937_64 Random::s_Engine(s_RandomDevice());
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_UniformDistribution;

}