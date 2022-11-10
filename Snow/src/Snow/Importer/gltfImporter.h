#pragma once

#include "Snow/Utils/FileSystem.h"

#include <string>
#include <optional>
#include <vector> 
#include <map>

#include <glm/glm.hpp>

namespace Snow {

	struct GLTFAsset {
		uint32_t VersionMajor;
		uint32_t VersionMinor;

		std::optional<std::string> Generator;
		std::optional<std::string> Copyright;
	};

	struct GLTFScene {
		std::string Name;
		std::vector<uint32_t> Nodes;
	};

	struct GLTFNode {
		std::string Name;

		uint32_t Camera;
		std::vector<uint32_t> Children;

		glm::vec3 Translation;
		glm::vec3 Rotation;
		glm::vec4 Rotation2;
		glm::vec3 Scale;

		glm::mat4 Transform;

		uint32_t Mesh;
		uint32_t Skin;

	};

	struct GLTFMaterial {
		std::string Name;

	};

	struct GLTFPrimitive {
		std::map<std::string, uint32_t> Attributes;
		uint32_t Indices;
		uint32_t MaterialIndex;

		uint32_t Mode;
	};

	struct GLTFFile {
		GLTFAsset Asset;

		std::vector<GLTFScene> Scenes;
		uint32_t LoadScene;
		std::vector<GLTFNode> Nodes;
	};

	class GLTFImporter {
	public:
		static GLTFFile Import(std::filesystem::path& filepath);
	};

}