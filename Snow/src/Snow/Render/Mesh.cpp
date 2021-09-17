#include <spch.h>

#include "Snow/Render/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include "Snow/Render/Renderer.h"

#define MESH_DEBUG_LOG 1
#if MESH_DEBUG_LOG
#define SNOW_MESH_LOG(...) SNOW_CORE_TRACE(__VA_ARGS__)
#else
#define SNOW_MESH_LOG(...)
#endif

namespace Snow {
	namespace Render {

		glm::mat4 Mat4FromAssimpMat4(const aiMatrix4x4& matrix) {
			glm::mat4 result;
			result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
			result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
			result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
			result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
			return result;

		}

		static const uint32_t s_MeshImportFlags =
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ValidateDataStructure;

		struct LogStream : public Assimp::LogStream {
			static void Initialize() {
				if (Assimp::DefaultLogger::isNullLogger()) {
					Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
					Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
				}
			}

			virtual void write(const char* message) override {
				SNOW_CORE_ERROR("Assimp error: {0}", message);
			}
		};

		MeshAsset::MeshAsset(const std::string& filePath) :
			m_FilePath(filePath) {
			
			LogStream::Initialize();

			SNOW_CORE_INFO("Loading mesh: {0}", filePath.c_str());

			m_Importer = std::make_unique<Assimp::Importer>();

			const aiScene* scene = m_Importer->ReadFile(filePath, s_MeshImportFlags);
			if (!scene || !scene->HasMeshes()) {
				SNOW_CORE_ERROR("Failed to load mesh file: {0}", filePath);
				SetFlag(AssetFlag::Invalid);
				return;
			}

			m_Scene = scene;

			m_IsAnimated = scene->mAnimations != nullptr;
			m_MeshShader = m_IsAnimated ? Renderer::GetShaderLibrary()->Get("PBR") : Renderer::GetShaderLibrary()->Get("PBR");
			m_InverseTransform = glm::inverse(Mat4FromAssimpMat4(scene->mRootNode->mTransformation));

			uint32_t vertexCount = 0;
			uint32_t indexCount = 0;


			m_Submeshes.reserve(scene->mNumMeshes);
			for (unsigned m = 0; m < scene->mNumMeshes; m++) {
				aiMesh* mesh = scene->mMeshes[m];

				Submesh& submesh = m_Submeshes.emplace_back();
				submesh.BaseVertex = vertexCount;
				submesh.BaseIndex = indexCount;
				submesh.MaterialIndex = mesh->mMaterialIndex;
				submesh.VertexCount = mesh->mNumVertices;
				submesh.IndexCount = mesh->mNumFaces * 3;
				submesh.MeshName = mesh->mName.C_Str();

				vertexCount += mesh->mNumVertices;
				indexCount += submesh.IndexCount;

				SNOW_CORE_ASSERT(mesh->HasPositions(), "meshes require positions");
				SNOW_CORE_ASSERT(mesh->HasNormals(), "meshes require normals");

				if (m_IsAnimated) {

				}
				else {
					auto& aabb = submesh.BoundingBox;
					aabb.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
					aabb.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
					for (size_t i = 0; i < mesh->mNumVertices; i++) {
						Vertex vertex;
						vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
						vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

						aabb.Min.x = glm::min(vertex.Position.x, aabb.Min.x);
						aabb.Min.y = glm::min(vertex.Position.y, aabb.Min.y);
						aabb.Min.z = glm::min(vertex.Position.z, aabb.Min.z);
						aabb.Max.x = glm::max(vertex.Position.x, aabb.Max.x);
						aabb.Max.y = glm::max(vertex.Position.y, aabb.Max.y);
						aabb.Max.z = glm::max(vertex.Position.z, aabb.Max.z);

						if (mesh->HasTangentsAndBitangents()) {
							vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
							vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
						}

						if (mesh->HasTextureCoords(0))
							vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

						m_StaticVertices.push_back(vertex);
					}
				}

				for (size_t i = 0; i < mesh->mNumFaces; i++) {
					
					Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
					m_Indices.push_back(index);

					if (!m_IsAnimated)
						m_TriangleCache[m].emplace_back(m_StaticVertices[index.V1 + submesh.BaseVertex], m_StaticVertices[index.V2 + submesh.BaseVertex], m_StaticVertices[index.V3 + submesh.BaseVertex]);

				}
			}

			TraverseNodes(scene->mRootNode);

			for (const auto& submesh : m_Submeshes) {
				Core::AABB transformedSubmeshAABB = submesh.BoundingBox;
				glm::vec3 min = glm::vec3(submesh.Transform * glm::vec4(transformedSubmeshAABB.Min, 1.0f));
				glm::vec3 max = glm::vec3(submesh.Transform * glm::vec4(transformedSubmeshAABB.Max, 1.0f));

				m_BoundingBox.Min.x = glm::min(m_BoundingBox.Min.x, min.x);
				m_BoundingBox.Min.y = glm::min(m_BoundingBox.Min.y, min.y);
				m_BoundingBox.Min.z = glm::min(m_BoundingBox.Min.z, min.z);
				m_BoundingBox.Max.x = glm::max(m_BoundingBox.Max.x, max.x);
				m_BoundingBox.Max.y = glm::max(m_BoundingBox.Max.y, max.y);
				m_BoundingBox.Max.z = glm::max(m_BoundingBox.Max.z, max.z);
			}

			Ref<Texture2D> whiteTexture = Renderer::GetWhiteTexture();
			if (scene->HasMaterials()) {
				SNOW_MESH_LOG("----Materials-{0}", filePath);
				m_Textures.resize(scene->mNumMaterials);
				m_Materials.resize(scene->mNumMaterials);

				for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
					auto aiMaterial = scene->mMaterials[i];
					auto aiMaterialName = aiMaterial->GetName();

					auto mi = Material::Create(m_MeshShader, aiMaterialName.data);
					m_Materials[i] = mi;

					SNOW_MESH_LOG("    {0} (Index = {1}}", aiMaterialName.data, i);
					aiString aiTexPath;
					uint32_t textureCount = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);
					SNOW_MESH_LOG("    TextureCount = {0}", textureCount);

					glm::vec3 albedoColor(0.8f);
					float emission = 0.0f;
					aiColor3D aiColor, aiEmission;
					if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor) == aiReturn_SUCCESS)
						albedoColor = { aiColor.r, aiColor.g, aiColor.b };
					if (aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiEmission) == aiReturn_SUCCESS)
						emission = aiEmission.r;

					mi->Set("u_MaterialUniforms.AlbedoColor", albedoColor);
					mi->Set("u_MaterialUniforms.Emission", emission);

					float shininess, metalness;
					if (aiMaterial->Get(AI_MATKEY_SHININESS, shininess) != aiReturn_SUCCESS)
						shininess = 80.0f;
					if (aiMaterial->Get(AI_MATKEY_REFLECTIVITY, metalness) != aiReturn_SUCCESS)
						shininess = 0.0f;

					float roughness = 1.0f - glm::sqrt(shininess / 100.0f);

					bool hasAlbedoMap = aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == AI_SUCCESS;
					bool fallback = !hasAlbedoMap;
					if (hasAlbedoMap) {
						std::filesystem::path path = filePath;
						auto parentPath = path.parent_path();
						parentPath /= std::string(aiTexPath.data);
						std::string texturePath = parentPath.string();
						SNOW_MESH_LOG("    Albedo map path = {0}", texturePath);
						TextureProperties props;
						props.SRGB = true;
						auto texture = Texture2D::Create(texturePath, props);
						if (texture->Loaded()) {
							m_Textures[i] = texture;
							mi->Set("u_AlbedoTexture", texture);
							mi->Set("u_MaterialUniforms.AlbedoColor", texture);
						}
						else {
							SNOW_CORE_ERROR("could not load texture: {0}", texturePath);
							fallback = true;
						}
					}

					if (fallback) {
						SNOW_MESH_LOG("    No albedo map");
						mi->Set("u_AlbedoTexture", whiteTexture);
					}

					bool hasNormalMap = aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &aiTexPath) == AI_SUCCESS;
					fallback = !hasNormalMap;
					if (hasNormalMap) {
						std::filesystem::path path = filePath;
						auto parentPath = path.parent_path();
						parentPath /= std::string(aiTexPath.data);
						std::string texturePath = parentPath.string();
						SNOW_MESH_LOG("    Normal map path = {0}", texturePath);
						auto texture = Texture2D::Create(texturePath);
						if (texture->Loaded()) {
							m_Textures[i] = texture;
							mi->Set("u_NormalTexture", texture);
							mi->Set("u_MaterialUniforms.UseNormalMap", true);
						}
						else {
							SNOW_CORE_ERROR("could not load texture: {0}", texturePath);
							fallback = true;
						}
					}

					if (fallback) {
						SNOW_MESH_LOG("    No normal map");
						mi->Set("u_NormalTexture", whiteTexture);
						mi->Set("u_MaterialUniforms.UseNormalMap", false);
					}

					bool hasRoughnessMap = aiMaterial->GetTexture(aiTextureType_SHININESS, 0, &aiTexPath) == AI_SUCCESS;
					fallback = !hasRoughnessMap;
					if (hasRoughnessMap) {
						std::filesystem::path path = filePath;
						auto parentPath = path.parent_path();
						parentPath /= std::string(aiTexPath.data);
						std::string texturePath = parentPath.string();
						SNOW_MESH_LOG("    Roughness map path = {0}", texturePath);
						auto texture = Texture2D::Create(texturePath);
						if (texture->Loaded()) {
							m_Textures[i] = texture;
							mi->Set("u_RoughnessTexture", texture);
							mi->Set("u_MaterialUniforms.Roughness", 1.0f);
						}
						else {
							SNOW_CORE_ERROR("could not load texture: {0}", texturePath);
							fallback = true;
						}
					}

					if (fallback) {
						SNOW_MESH_LOG("    No roughness map");
						mi->Set("u_RoughnessTexture", whiteTexture);
						mi->Set("u_MaterialUniforms.Roughness", roughness);
					}

					bool metalnessTextureFound = false;
					for (uint32_t p = 0; p < aiMaterial->mNumProperties; p++) {
						auto prop = aiMaterial->mProperties[p];

						SNOW_MESH_LOG("Material Property:");
						SNOW_MESH_LOG("    Name = {0}", prop->mKey.data);

						float data = *(float*)prop->mData;
						SNOW_MESH_LOG("    Value = {0}", data);

						switch (prop->mSemantic) {
						case aiTextureType_NONE:
							SNOW_MESH_LOG("    Semantic = aiTextureType_NONE");
							break;
						case aiTextureType_DIFFUSE:
							SNOW_MESH_LOG("    Semantic = aiTextureType_DIFFUSE");
							break;
						case aiTextureType_SPECULAR:
							SNOW_MESH_LOG("    Semantic = aiTextureType_SPECULAR");
							break;
						case aiTextureType_AMBIENT:
							SNOW_MESH_LOG("    Semantic = aiTextureType_AMBIENT");
							break;
						case aiTextureType_EMISSIVE:
							SNOW_MESH_LOG("    Semantic = aiTextureType_EMISSIVE");
							break;
						case aiTextureType_HEIGHT:
							SNOW_MESH_LOG("    Semantic = aiTextureType_HEIGHT");
							break;
						case aiTextureType_NORMALS:
							SNOW_MESH_LOG("    Semantic = aiTextureType_NORMALS");
							break;
						case aiTextureType_SHININESS:
							SNOW_MESH_LOG("    Semantic = aiTextureType_SHININESS");
							break;
						case aiTextureType_OPACITY:
							SNOW_MESH_LOG("    Semantic = aiTextureType_OPACITY");
							break;
						case aiTextureType_DISPLACEMENT:
							SNOW_MESH_LOG("    Semantic = aiTextureType_DISPLACEMENT");
							break;
						case aiTextureType_LIGHTMAP:
							SNOW_MESH_LOG("    Semantic = aiTextureType_LIGHTMAP");
							break;
						case aiTextureType_REFLECTION:
							SNOW_MESH_LOG("    Semantic = aiTextureType_REFLECTION");
							break;
						case aiTextureType_UNKNOWN:
							SNOW_MESH_LOG("    Semantic = aiTextureType_UNKNOWN");
							break;
						}

						if (prop->mType == aiPTI_String) {
							uint32_t strLength = *(uint32_t*)prop->mData;
							std::string str(prop->mData + 4, strLength);
							std::string key = prop->mKey.data;
							if (key == "$raw.ReflectionFactor|file") {
								std::filesystem::path path = filePath;
								auto parentPath = path.parent_path();
								parentPath /= str;
								std::string texturePath = parentPath.string();
								SNOW_MESH_LOG("    Metalness map path = {0}", texturePath);
								auto texture = Texture2D::Create(texturePath);
								if (texture->Loaded()) {
									metalnessTextureFound = true;
									m_Textures[i] = texture;
									mi->Set("u_MetalnessTexture", texture);
									mi->Set("u_MaterialUniforms.Metalness", 1.0f);
								}
								else {
									SNOW_CORE_ERROR("could not load texture: {0}", texturePath);
								}
								break;
							}
						}
					}

					fallback = !metalnessTextureFound;
					if (fallback) {
						SNOW_MESH_LOG("    no metalness map");
						mi->Set("u_MetalnessTexture", whiteTexture);
						mi->Set("u_MaterialUniforms.Metalness", metalness);
					}
				}
				SNOW_MESH_LOG("-------------------");
			}
			else {
				auto mi = Material::Create(m_MeshShader, "Snow-Default");
				mi->Set("u_MaterialUniforms.AlbedoColor", glm::vec3(0.0f));
				mi->Set("u_MaterialUniforms.Emission", 0.0f);
				mi->Set("u_MaterialUniforms.Metalness", 0.0f);
				mi->Set("u_MaterialUniforms.Roughness", 0.8f);
				mi->Set("u_MaterialUniforms.UseNormalMap", false);

				mi->Set("u_AlbedoTexture", whiteTexture);
				mi->Set("u_MetalnessTexture", whiteTexture);
				mi->Set("u_RoughnessTexture", whiteTexture);
				m_Materials.push_back(mi);
			}

			if (m_IsAnimated) {
				//m_VertexBuffer = VertexBuffer::Create(m_AnimatedVertices.data(), (uint32_t)(m_AnimcatedVertices.size() * sizeof(AnimatedVertex)));

			}
			else {
				m_VertexBuffer = VertexBuffer::Create(m_StaticVertices.data(), (uint32_t)(m_StaticVertices.size() * sizeof(Vertex)));
				m_VertexBufferLayout = {
					{AttribType::Float3, "a_Position"},
					{AttribType::Float3, "a_Normal"},
					{AttribType::Float3, "a_Tangent"},
					{AttribType::Float3, "a_Binormal"},
					{AttribType::Float2, "a_TexCoord"},
				};
			}

			m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint32_t)(m_Indices.size() * sizeof(Index)));
		}

		MeshAsset::MeshAsset(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform) :
			m_StaticVertices(vertices), m_Indices(indices), m_IsAnimated(false) {
			
			Submesh submesh;
			submesh.BaseVertex = 0;
			submesh.BaseIndex = 0;
			submesh.IndexCount = (uint32_t)indices.size() * 3u;
			submesh.Transform = transform;
			m_Submeshes.push_back(submesh);

			m_VertexBuffer = VertexBuffer::Create(m_StaticVertices.data(), (uint32_t)(m_StaticVertices.size() * sizeof(Vertex)));
			m_VertexBufferLayout = {
					{AttribType::Float3, "a_Position"},
					{AttribType::Float3, "a_Normal"},
					{AttribType::Float3, "a_Tangent"},
					{AttribType::Float3, "a_Binormal"},
					{AttribType::Float2, "a_TexCoord"},
			};
			m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint32_t)(m_Indices.size() * sizeof(Index)));
		}

		static std::string LevelToSpaces(uint32_t level) {
			std::string result = "";
			for (uint32_t i = 0; i < level; i++)
				result += "--";
			return result;
		}

		void MeshAsset::TraverseNodes(aiNode* node, const glm::mat4& parentTransform, uint32_t level) {
			glm::mat4 transform = parentTransform * Mat4FromAssimpMat4(node->mTransformation);
			m_NodeMap[node].resize(node->mNumMeshes);
			for (uint32_t i = 0; i < node->mNumMeshes; i++) {
				uint32_t mesh = node->mMeshes[i];
				auto& submesh = m_Submeshes[mesh];
				submesh.NodeName = node->mName.C_Str();
				submesh.Transform = transform;
				m_NodeMap[node][i] = mesh;
			}

			SNOW_MESH_LOG("{0} {1}", LevelToSpaces(level), node->mName.C_Str());

			for (uint32_t i = 0; i < node->mNumChildren; i++)
				TraverseNodes(node->mChildren[i], transform, level + 1);
		}

		Mesh::Mesh(Ref<MeshAsset> meshAsset) :
			m_MeshAsset(meshAsset) {
			SetSubmeshes({});

			const auto& meshMaterials = meshAsset->GetMaterials();
			m_Materials = Ref<MaterialTable>::Create(meshMaterials.size());
			for (size_t i = 0; i < meshMaterials.size(); i++)
				m_Materials->SetMaterial(i, Ref<MaterialAsset>::Create(meshMaterials[i]));
		}

		Mesh::Mesh(Ref<MeshAsset> meshAsset, const std::vector<uint32_t>& submeshes) :
			m_MeshAsset(meshAsset) {
			SetSubmeshes(submeshes);

			const auto& meshMaterials = meshAsset->GetMaterials();
			m_Materials = Ref<MaterialTable>::Create(meshMaterials.size());
			for (size_t i = 0; i < meshMaterials.size(); i++)
				m_Materials->SetMaterial(i, Ref<MaterialAsset>::Create(meshMaterials[i]));
		}

		Mesh::Mesh(const Ref<Mesh>& other) :
			m_MeshAsset(other->m_MeshAsset), m_Materials(other->m_Materials) {
			SetSubmeshes(other->m_Submeshes);
		}

	}
}