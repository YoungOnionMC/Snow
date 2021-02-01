#include <spch.h>

#include "Snow/Render/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include "Snow/Render/Renderer.h"

namespace Snow {
	namespace Render {
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

		Mesh::Mesh(const std::string& filePath) :
			m_Path(filePath) {
			CreateMesh();
		}

		void Mesh::CreateMesh() {
			Assimp::Importer importer;

			uint32_t flags = aiProcess_CalcTangentSpace |
							aiProcess_Triangulate |
							aiProcess_JoinIdenticalVertices |
							aiProcess_SortByPType;
			const aiScene* scene = importer.ReadFile(m_Path, flags);

			if (!scene || !scene->HasMeshes())
				SNOW_CORE_ERROR("Scene must have meshes!");
			
			uint32_t vertexCount = 0;
			uint32_t indexCount = 0;

			for (size_t m = 0; m < scene->mNumMeshes; m++) {
				aiMesh* mesh = scene->mMeshes[m];

				vertexCount += mesh->mNumVertices;
				indexCount += mesh->mNumFaces * 3;

				SNOW_CORE_ASSERT(mesh->HasPositions());
				SNOW_CORE_ASSERT(mesh->HasNormals());

				for (size_t i = 0; i < mesh->mNumVertices; i++) {
					Vertex vertex;
					vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
					vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

					if (mesh->HasTangentsAndBitangents()) {
						vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
						vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
					}

					if (mesh->HasTextureCoords(0))
						vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

					m_VertexData.push_back(vertex);
				}

				for (size_t i = 0; i < mesh->mNumFaces; i++) {
					Index index;
					index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
					m_IndexData.push_back(index);
				}
			}


			m_VBO = API::VertexBuffer::Create(m_VertexData.data(), m_VertexData.size() * sizeof(Vertex));

			m_IBO = API::IndexBuffer::Create(m_IndexData.data(), m_IndexData.size() * sizeof(Index));
			
			PipelineSpecification pipelineSpec;
			pipelineSpec.Layout = {
				{ AttribType::Float3, "a_Position" },
				{ AttribType::Float3, "a_Normal" },
				{ AttribType::Float3, "a_Tangent" },
				{ AttribType::Float3, "a_Bitangent" },
				{ AttribType::Float2, "a_TexCoord" }
			};
			pipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("PBR");
			pipelineSpec.Type = PrimitiveType::Triangle;
			Ref<Pipeline> pipeline = Pipeline::Create(pipelineSpec);
			m_MaterialInstance = Ref<MaterialInstance>::Create(Ref<Material>::Create(pipeline));

			

		}

	}
}