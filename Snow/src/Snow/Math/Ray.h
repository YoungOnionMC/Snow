#pragma once

#include <glm/glm.hpp>

namespace Snow {
	namespace Core {
		struct Ray {
			glm::vec3 Origin;
			glm::vec3 Direction;

			Ray(glm::vec3 dir, glm::vec3 origin = glm::vec3(0.0f)) :
				Direction(dir), Origin(origin) {}


		};
	}
}