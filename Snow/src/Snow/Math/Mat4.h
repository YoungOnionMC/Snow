#pragma once

#include <glm/glm.hpp>

namespace Snow {
	namespace Math {
		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

		bool FlipMatrix(const glm::mat4& matrix, glm::mat4& outMatrix);
	}
}
