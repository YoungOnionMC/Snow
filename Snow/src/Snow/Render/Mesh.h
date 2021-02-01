#pragma once

#include "Snow/Core/Ref.h"

#include <string>

#include "Snow/Render/Material.h"

#include "Snow/Render/API/Buffer.h"

#include <glm/glm.hpp>

namespace Snow {
	namespace Render {
		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
			glm::vec2 TexCoord;
		};

		struct Index {
			uint32_t V1, V2, V3;
		};

		class Mesh : public RefCounted {
		public:
			Mesh(const std::string& filePath);

			const std::string& GetPath() { return m_Path; }

			Ref<MaterialInstance> GetMaterialInstance() const { return m_MaterialInstance; }

			Ref<API::VertexBuffer> GetVertexBuffer() const { return m_VBO; }
			Ref<API::IndexBuffer> GetIndexBuffer() const { return m_IBO; }
		private:
			void CreateMesh();

			std::string m_Path;

			Ref<MaterialInstance> m_MaterialInstance;

			Ref<API::VertexBuffer> m_VBO;
			Ref<API::IndexBuffer> m_IBO;

			std::vector<Vertex> m_VertexData;
			std::vector<Index> m_IndexData;
		};
	}
}
