#pragma once

#include "Snow/Asset/AssetTypes.h"

#include <unordered_map>

namespace Snow {
	inline static std::unordered_map<std::string, AssetType> s_AssetExtensionMap = {
		{".scene", AssetType::Scene},

		{".mesh", AssetType::Mesh},

		{".mat", AssetType::Material},

		{".fbx", AssetType::MeshSource},
		{".gltf", AssetType::MeshSource},
		{".glb", AssetType::MeshSource},
		{".obj", AssetType::MeshSource},

		{".jpeg", AssetType::Texture},
		{".jpg", AssetType::Texture},
		{".png", AssetType::Texture},
		{".tga", AssetType::Texture},

		{".hdr", AssetType::EnvMap},

		{".wav", AssetType::Audio},
		{".ogg", AssetType::Audio},
		{".mp3", AssetType::Audio},

		{".cs", AssetType::Script},

		{".prefab", AssetType::Prefab},
	};
}