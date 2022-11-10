#include <spch.h>
#include "Snow/Asset/AssetSerializer.h"
#include "Snow/Asset/AssetManager.h"

#include "Snow/Render/Mesh.h"
#include "Snow/Render/Texture.h"

#include "Snow/Scene/SceneSerializer.h"

namespace Snow {
	bool TextureSerializer::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const {
		asset = Render::Texture2D::Create(AssetManager::GetFileSystemPathString(metadata));
		asset->Handle = metadata.Handle;

		bool result = asset.As<Render::Texture2D>()->Loaded();

		if (!result)
			asset->SetFlag(AssetFlag::Invalid, true);

		return result;
	}

	bool MeshSourceSerializer::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const {
		asset = Ref<Render::MeshSource>::Create(AssetManager::GetFileSystemPathString(metadata));
		asset->Handle = metadata.Handle;
		return (asset.As<Render::MeshSource>())->GetVertices().size() > 0;
	}

	bool SceneAssetSerializer::Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset) const {
		SceneSerializer serializer(asset.As<Scene>());
		serializer.SerializeText(AssetManager::GetFileSystemPath(metadata).string());
		return true;
	}

	bool SceneAssetSerializer::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const {
		asset = Ref<Asset>::Create();
		asset->Handle = metadata.Handle;
		return true;
	}
}