#pragma once

#include <glm/glm.hpp>

namespace Snow {
	namespace Core {
		struct AABB {
			glm::vec3 MinBounds, MaxBounds;

			AABB(const glm::vec3& min, const glm::vec3& max) :
				MinBounds(min), MaxBounds(max) {}

			bool Intersects(const AABB& other) {
				if(other.)
			}
		};
	}
}