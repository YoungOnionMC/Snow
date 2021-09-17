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
		MeshAsset = 2,
		Mesh = 3,
		Texture = 4,
		EnvMap = 5,
		Audio = 6,
		PhysicsMat = 7,

	};

	namespace Utils {
		inline AssetType AssetTypeFromString(const std::string& assetType) {
			if (assetType == "None")	return AssetType::None;
			if (assetType == "Scene")	return AssetType::Scene;
			if (assetType == "MeshAsset")	return AssetType::MeshAsset;
			if (assetType == "Mesh")	return AssetType::Mesh;
			if (assetType == "Texture")	return AssetType::Texture;
			if (assetType == "EnvMap")	return AssetType::EnvMap;
			if (assetType == "Audio")	return AssetType::Audio;
			if (assetType == "PhysicsMat")	return AssetType::PhysicsMat;

			SNOW_CORE_ASSERT(false, "unknown assettype");
			return AssetType::None;
		}

		inline const char* AssetTypeFromString(AssetType assetType) {
			switch (assetType) {
			case AssetType::None:	return "None";
			case AssetType::Scene:	return "Scene";
			case AssetType::MeshAsset:	return "MeshAsset";
			case AssetType::Mesh:	return "Mesh";
			case AssetType::Texture:	return "Texture";
			case AssetType::EnvMap:	return "EnvMap";
			case AssetType::Audio:	return "Audio";
			case AssetType::PhysicsMat:	return "PhysicsMat";
			}

			SNOW_CORE_ASSERT(false, "unknown assettype");
			return "None";
		}
	}
}