#include <spch.h>

#include "Snow/Render/MeshFactory.h"

#include "Snow/Render/Buffer.h"

#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

namespace Snow {
	namespace Render {
		Ref<Mesh> MeshFactory::CreateSquare(float xLength, float yLength) {
			std::vector<Vertex> vertices(4);


			vertices[0].Position = { -xLength / 2, 0.0f, - yLength / 2 };
			vertices[1].Position = {  xLength / 2, 0.0f, - yLength / 2 };
			vertices[2].Position = {  xLength / 2, 0.0f,   yLength / 2 };
			vertices[3].Position = { -xLength / 2, 0.0f,   yLength / 2 };


			vertices[0].Normal = { 0.0f, 1.0f, 0.0f };
			vertices[1].Normal = { 0.0f, 1.0f, 0.0f };
			vertices[2].Normal = { 0.0f, 1.0f, 0.0f };
			vertices[3].Normal = { 0.0f, 1.0f, 0.0f };


			std::vector<Index> triangles(2);

			triangles[0] = { 0, 1, 2 };
			triangles[1] = { 2, 3, 0 };


			return Ref<Mesh>::Create(Ref<MeshSource>::Create(vertices, triangles, glm::mat4(1.0f)));
		}

		Ref<Mesh> MeshFactory::CreateCube(float xLength, float yLength, float zLength) {
			return CreateCube({ xLength, yLength, zLength });
			
		}

		Ref<Mesh> MeshFactory::CreateCube(const glm::vec3& size) {
			std::vector<Vertex> vertices(8);

			vertices.resize(8);
			vertices[0].Position = { -size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
			vertices[1].Position = { size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
			vertices[2].Position = { size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
			vertices[3].Position = { -size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
			vertices[4].Position = { -size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
			vertices[5].Position = { size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
			vertices[6].Position = { size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };
			vertices[7].Position = { -size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };

			vertices[0].Normal = { -1.0f, -1.0f,  1.0f };
			vertices[1].Normal = { 1.0f, -1.0f,  1.0f };
			vertices[2].Normal = { 1.0f,  1.0f,  1.0f };
			vertices[3].Normal = { -1.0f,  1.0f,  1.0f };
			vertices[4].Normal = { -1.0f, -1.0f, -1.0f };
			vertices[5].Normal = { 1.0f, -1.0f, -1.0f };
			vertices[6].Normal = { 1.0f,  1.0f, -1.0f };
			vertices[7].Normal = { -1.0f,  1.0f, -1.0f };

			std::vector<Index> indices;
			indices.resize(12);
			indices[0] = { 0, 1, 2 };
			indices[1] = { 2, 3, 0 };
			indices[2] = { 1, 5, 6 };
			indices[3] = { 6, 2, 1 };
			indices[4] = { 7, 6, 5 };
			indices[5] = { 5, 4, 7 };
			indices[6] = { 4, 0, 3 };
			indices[7] = { 3, 7, 4 };
			indices[8] = { 4, 5, 1 };
			indices[9] = { 1, 0, 4 };
			indices[10] = { 3, 2, 6 };
			indices[11] = { 6, 7, 3 };


			/*
			vertices[0].Position = { -size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
			vertices[1].Position = {  size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
			vertices[2].Position = {  size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };
			vertices[3].Position = { -size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };
			vertices[4].Position = { -size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
			vertices[5].Position = {  size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
			vertices[6].Position = {  size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
			vertices[7].Position = { -size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };

			vertices[0].Normal = { -1.0f, -1.0f,   1.0f };
			vertices[1].Normal = {  1.0f, -1.0f,   1.0f };
			vertices[2].Normal = {  1.0f,  1.0f,   1.0f };
			vertices[3].Normal = { -1.0f,  1.0f,   1.0f };

			vertices[6].Normal = { -1.0f, -1.0f,  -1.0f };
			vertices[7].Normal = {  1.0f, -1.0f,  -1.0f };
			vertices[4].Normal = {  1.0f,  1.0f,  -1.0f };
			vertices[5].Normal = { -1.0f,  1.0f,  -1.0f };

			std::vector<Index> indices(12);

			indices[0] = { 0, 1, 2 }; // Front Face
			indices[1] = { 2, 3, 0 };

			indices[2] = { 1, 5, 6 }; // Right Face
			indices[3] = { 6, 2, 1 };

			indices[4] = { 5, 4, 7 }; // Back Face
			indices[5] = { 7, 6, 5 };

			indices[6] = { 4, 0, 3 }; // Left Face
			indices[7] = { 3, 7, 4 };

			indices[8] = { 4, 5, 1 }; // Bottom Face
			indices[9] = { 1, 0, 4 };

			indices[10] = { 3, 2, 6 }; // Top Face
			indices[11] = { 6, 7, 3 };
			*/

			/*
			indices[6] = { 4,0,3 };
			indices[7] = { 3,7,4 };
			indices[8] = { 3,2,6 };
			indices[9] = { 6,7,3 };
			indices[10] = { 4,5,1 };
			indices[11] = { 1,0,4 };*/
			/*
			indices[4] = { 0,0,0 };
			indices[5] = { 0,0,0 };
			indices[6] = { 0,0,0 };
			indices[7] = { 0,0,0 };
			indices[8] = { 0,0,0 };
			indices[9] = { 0,0,0 };
			indices[10] = { 0,0,0 };
			indices[11] = { 0,0,0 };
			*/

			return Ref<Mesh>::Create(Ref<MeshSource>::Create(vertices, indices, glm::mat4(1.0f)));
		}

		Ref<Mesh> MeshFactory::CreateSphere(float radius, float segments) {
			std::vector<Vertex> vertices;
			std::vector<Index> indices;

			for (float latitude = 0.0f; latitude <= segments; latitude++) {
				const float theta = latitude * (float)M_PI / segments;
				const float sinTheta = glm::sin(theta);
				const float cosTheta = glm::cos(theta);
				for (float longitude = 0.0f; longitude <= segments; longitude++) {
					const float phi = longitude * 2.f * (float)M_PI / segments;
					const float sinPhi = glm::sin(phi);
					const float cosPhi = glm::cos(phi);

					Vertex vertex;
					vertex.Normal = { cosPhi * sinTheta, cosTheta, sinPhi * sinTheta };
					vertex.Position = { radius * vertex.Normal.x, radius * vertex.Normal.y, radius * vertex.Normal.z };
					vertices.push_back(vertex);
				}
			}


			for (uint32_t latitude = 0; latitude < segments; latitude++) {
				for (uint32_t longitude = 0; longitude < segments; longitude++) {
					const uint32_t first = (latitude * (segments + 1)) + longitude;
					const uint32_t second = first + segments + 1;

					indices.push_back({ first, second, first + 1});
					indices.push_back({ second, second + 1, first + 1 });
				}
			}

			return Ref<Mesh>::Create(Ref<MeshSource>::Create(vertices, indices, glm::mat4(1.0f)));
		}

		Ref<Mesh> MeshFactory::CreateCapsule(float radius, float height, float segments) {
			return nullptr;
		}
	}
}