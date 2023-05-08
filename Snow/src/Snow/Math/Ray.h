#pragma once

#include <glm/glm.hpp>

namespace Snow {
	namespace Math {
		struct Ray {
			glm::vec3 Origin;
			glm::vec3 Direction;

			Ray() :
				Direction(glm::vec3(0.0f)), Origin(glm::vec3(0.0f)) {}

			Ray(glm::vec3 dir) :
				Direction(dir), Origin(glm::vec3(0.0f)) {}

			Ray(glm::vec3 origin, glm::vec3 dir) :
				Direction(dir), Origin(origin) {}


		};
	}
}