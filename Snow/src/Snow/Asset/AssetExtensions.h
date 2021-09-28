#pragma once

#include "Snow/Asset/AssetTypes.h"

#include <unordered_map>

namespace Snow {
	inline static std::unordered_map<std::string, AssetType> s_AssetExtensionMap = {
		{".scene", AssetType::Scene},
		{".mesh", AssetType::Mesh},
		{".mat", AssetType::Material},

		{".fbx", AssetType::MeshAsset},
		{".gltf", AssetType::MeshAsset},
		{".obj", AssetType::MeshAsset},
		{".glb", AssetType::MeshAsset},

		{".png", AssetType::Texture},
		{".tga", AssetType::Texture},
		{".jpg", AssetType::Texture},
		{".jpeg", AssetType::Texture},

		{".hdr", AssetType::EnvMap},

	};
}