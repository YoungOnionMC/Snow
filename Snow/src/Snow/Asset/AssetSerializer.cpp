#include <spch.h>
#include "Snow/Asset/AssetSerializer.h"
#include "Snow/Asset/AssetManager.h"

#include "Snow/Render/Mesh.h"
#include "Snow/Render/Texture.h"

namespace Snow {
	bool TextureSerializer::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const {
		asset = Render::Texture2D::Create(AssetManager::GetFileSystemPathString(metadata));
		asset->Handle = metadata.Handle;

		bool result = asset.As<Render::Texture2D>()->Loaded();

		if (!result)
			asset->SetFlag(AssetFlag::Invalid, true);

		return result;
	}

	bool MeshAssetSerializer::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const {
		asset = Ref<Render::MeshAsset>::Create(AssetManager::GetFileSystemPathString(metadata));
		asset->Handle = metadata.Handle;
		return (asset.As<Render::MeshAsset>())->GetVertices().size() > 0;
	}
}