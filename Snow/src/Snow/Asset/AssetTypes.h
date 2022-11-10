#pragma once

#include "Snow/Core/Base.h"
#include "Snow/Core/Log.h"

namespace Snow {
	enum class AssetFlag : uint16_t {
		None = 0,
		Missing = BIT(0),
		Invalid = BIT(1)
	};

	enum class AssetType : uint16_t {
		None = 0,
		Scene = 1,

		MeshSource = 2,
		Mesh = 3,
		Material = 4,
		Texture = 5,
		EnvMap = 6,
		Shader = 7,

		Audio = 8,

		Script = 9,

		PhysicsMat = 10,

		Font = 11,

		Prefab = 12,
	};

	namespace Utils {
		inline AssetType AssetTypeFromString(const std::string& assetType) {
			if (assetType == "None")	return AssetType::None;
			if (assetType == "Scene")	return AssetType::Scene;
			if (assetType == "MeshSource")	return AssetType::MeshSource;
			if (assetType == "Mesh")	return AssetType::Mesh;
			if (assetType == "Texture")	return AssetType::Texture;
			if (assetType == "EnvMap")	return AssetType::EnvMap;
			if (assetType == "Shader")	return AssetType::Shader;
			if (assetType == "Audio")	return AssetType::Audio;
			if (assetType == "Script")	return AssetType::Script;
			if (assetType == "Font")	return AssetType::Font;
			if (assetType == "Prefab")	return AssetType::Prefab;
			if (assetType == "PhysicsMat")	return AssetType::PhysicsMat;

			SNOW_CORE_ASSERT(false, "unknown assettype");
			return AssetType::None;
		}

		inline const char* AssetTypeToString(AssetType assetType) {
			switch (assetType) {
			case AssetType::None:	return "None";
			case AssetType::Scene:	return "Scene";
			case AssetType::MeshSource:	return "MeshSource";
			case AssetType::Mesh:	return "Mesh";
			case AssetType::Texture:	return "Texture";
			case AssetType::EnvMap:	return "EnvMap";
			case AssetType::Audio:	return "Audio";
			case AssetType::Script:	return "Script";
			case AssetType::Font:	return "Font";
			case AssetType::Prefab:	return "Prefab";
			case AssetType::PhysicsMat:	return "PhysicsMat";
			}

			SNOW_CORE_ASSERT(false, "unknown assettype");
			return "None";
		}
	}
}