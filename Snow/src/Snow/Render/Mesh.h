#pragma once

#include "Snow/Core/Ref.h"

#include <string>

#include "Snow/Asset/Asset.h"

#include "Snow/Render/Material.h"
#include "Snow/Render/MaterialAsset.h"
#include "Snow/Render/Buffer.h"
#include "Snow/Render/Pipeline.h"

#include "Snow/Math/AABB.h"

#include <glm/glm.hpp>

struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;

namespace Assimp {
	class Importer;
}

namespace Snow {
	class VulkanRenderer;
	class OpenGLRenderer;
	class SceneHierarchyPanel;
	class MeshViewerPanel;

	namespace Render {
		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec3 Tangent;
			glm::vec3 Binormal;
			glm::vec2 TexCoord;
		};

		struct Index {
			uint32_t V1, V2, V3;
		};

		static_assert(sizeof(Index) == 3 * sizeof(uint32_t));

		struct Triangle {
			Vertex V0, V1, V2;

			Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2):
				V0(v0), V1(v1), V2(v2) {}
		};

		class Submesh {
		public:
			uint32_t BaseVertex = 0;
			uint32_t BaseIndex = 0;
			uint32_t MaterialIndex = 0;
			uint32_t IndexCount = 0;
			uint32_t VertexCount = 0;

			glm::mat4 Transform{ 1.0f };
			Core::AABB BoundingBox;

			std::string NodeName, MeshName;
		};

		class MeshSource : public Asset {
		public:
			MeshSource(const std::string& filePath);
			MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform);
			virtual ~MeshSource();

			std::vector<Submesh>& GetSubmeshes() { return m_Submeshes; }
			const std::vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }

			const std::vector<Vertex>& GetVertices() const { return m_StaticVertices; }
			const std::vector<Index>& GetIndices() const { return m_Indices; }

			const Core::AABB& GetBoundingBox() const { return m_BoundingBox; }

			Ref<Shader> GetMeshShader() { return m_MeshShader; }
			std::vector<Ref<Material>>& GetMaterials() { return m_Materials; }
			const std::vector<Ref<Material>>& GetMaterials() const { return m_Materials; }
			const std::vector<Ref<Texture2D>>& GetTextures() const { return m_Textures; }
			const std::string& GetFilePath() const { return m_FilePath; }

			Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
			Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }

			const VertexBufferLayout& GetVertexBufferLayout() const { return m_VertexBufferLayout; }

		private:
			void TraverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);


			std::vector<Submesh> m_Submeshes;

			std::unique_ptr<Assimp::Importer> m_Importer;

			glm::mat4 m_InverseTransform;

			Ref<VertexBuffer> m_VertexBuffer;
			Ref<IndexBuffer> m_IndexBuffer;
			VertexBufferLayout m_VertexBufferLayout;

			std::vector<Vertex> m_StaticVertices;
			std::vector<Index> m_Indices;
			std::unordered_map<aiNode*, std::vector<uint32_t>> m_NodeMap;
			const aiScene* m_Scene;

			Ref<Shader> m_MeshShader;
			std::vector<Ref<Texture2D>> m_Textures;
			std::vector<Ref<Texture2D>> m_NormalTextures;
			std::vector<Ref<Material>> m_Materials;

			std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;

			Core::AABB m_BoundingBox;

			bool m_IsAnimated = false;

			std::string m_FilePath;

			friend class Renderer;
			friend class VulkanRenderer;
			friend class OpenGLRenderer;
			friend class SceneHierarchyPanel;
			friend class MeshViewerPanel;
		};

		class Mesh : public Asset {
		public:
			explicit Mesh(Ref<MeshSource> meshAsset);
			Mesh(Ref<MeshSource> meshAsset, const std::vector<uint32_t>& submeshes);
			Mesh(const Ref<Mesh>& other);
			virtual ~Mesh();

			std::vector<uint32_t>& GetSubmeshes() { return m_Submeshes; }
			const std::vector<uint32_t>& GetSubmeshes() const { return m_Submeshes; }

			void SetSubmeshes(const std::vector<uint32_t>& submeshes);

			Ref<MeshSource> GetMeshAsset() { return m_MeshSource; }
			Ref<MeshSource> GetMeshAsset() const { return m_MeshSource; }
			void SetMeshAsset(Ref<MeshSource> meshSource) { m_MeshSource = meshSource; }

			Ref<Shader> GetMeshShader() { return m_MeshShader; }
			Ref<MaterialTable> GetMaterials() { return m_Materials; }

			static AssetType GetStaticType() { return AssetType::Mesh; }
			virtual AssetType GetAssetType() const override { return GetStaticType(); }
		private:
			Ref<MeshSource> m_MeshSource;
			std::vector<uint32_t> m_Submeshes;

			Ref<Shader> m_MeshShader;
			Ref<MaterialTable> m_Materials;

			friend class Renderer;
			friend class VulkanRenderer;
			friend class OpenGLRenderer;
			friend class SceneHierarchyPanel;
			friend class MeshViewerPanel;

			
		};
	}
}
