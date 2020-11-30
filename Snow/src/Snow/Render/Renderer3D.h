#pragma once

#include "Snow/Render/Mesh.h"

#include "Snow/Core/Ref.h"

namespace Snow {
	namespace Render {
		class Renderer3D {
		public:
			static void DrawMesh(Ref<Mesh> mesh, const glm::mat4& transform);
		};
	}
}
