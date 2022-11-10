#include <spch.h>
#include "Snow/Asset/AssetImporter.h"
#include "Snow/Asset/AssetManager.h"

namespace Snow {
	void AssetImporter::Init() {
		s_Serializers[AssetType::Texture] = CreateScope<TextureSerializer>();
		s_Serializers[AssetType::MeshSource] = CreateScope<MeshSourceSerializer>();
		s_Serializers[AssetType::Scene] = CreateScope<SceneAssetSerializer>();
	}

	void AssetImporter::Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset) {
		if (s_Serializers.find(metadata.Type) == s_Serializers.end()) {
			SNOW_CORE_WARN("No importer for asset type {0}", metadata.FilePath.stem().string());
			return;
		}
		s_Serializers[asset->GetAssetType()]->Serialize(metadata, asset);
	}

	void AssetImporter::Serialize(const Ref<Asset>& asset) {
		const AssetMetadata& metadata = AssetManager::GetMetadata(asset->Handle);
		Serialize(metadata, asset);
	}

	bool AssetImporter::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) {
		if (s_Serializers.find(metadata.Type) == s_Serializers.end()) {
			SNOW_CORE_WARN("No importer for asset type {0}", metadata.FilePath.stem().string());
			return false;
		}
		return s_Serializers[metadata.Type]->TryLoadData(metadata, asset);
	}

	std::unordered_map<AssetType, Scope<AssetSerializer>> AssetImporter::s_Serializers;
}