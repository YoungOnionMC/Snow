#pragma once

#include <glm/glm.hpp>

inline double linearToGamma(double linear) {
	return linear < 0 ? 0 : std::sqrt(linear);
}

