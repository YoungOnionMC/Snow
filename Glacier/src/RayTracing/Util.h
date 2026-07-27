#pragma once

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <Snow/Math/Random.h>

inline glm::vec3 randomUnitVector() {
	while (true) {
		using Snow::Math::Random;
		glm::vec3 p = glm::vec3(Random::NextFloat(-1, 1), Random::NextFloat(-1, 1), Random::NextFloat(-1, 1));
		
		if (std::numeric_limits<float>::epsilon() < glm::length(p) && glm::length(p) <= 1) {
			return glm::normalize(p);
		}
	}
}

inline glm::vec3 randomOnHemisphere(const glm::vec3& normal) {
	glm::vec3 unitSphere = randomUnitVector();
	return glm::dot(unitSphere, normal) > 0.0 ? unitSphere : -unitSphere;
}