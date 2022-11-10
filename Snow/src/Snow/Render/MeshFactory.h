#pragma once

#include "Snow/Render/Mesh.h"

#include <glm/glm.hpp>

namespace Snow {
	namespace Render {


		class MeshFactory {
		public:
			static Ref<Mesh> CreateSquare(float xLength, float yLength);
			static Ref<Mesh> CreateCube(float xLength, float yLength, float zLength);
			static Ref<Mesh> CreateCube(const glm::vec3& size);

			static Ref<Mesh> CreateSphere(float radius, float segments);
			static Ref<Mesh> CreateCapsule(float radius, float height, float segments);


		};
	}
}