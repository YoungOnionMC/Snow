#pragma once

#include "Snow/Asset/Asset.h"
#include "Snow/Asset/AssetMetadata.h"
#include "Snow/Asset/AssetImporter.h"
#include "Snow/Asset/AssetRegistry.h"

#include "Snow/Utils/FileSystem.h"

namespace Snow {


	class AssetManager {
	public:


		static void Init();
		static void Shutdown();



		static const AssetMetadata& GetMetadata(AssetHandle handle);
		static const AssetMetadata& GetMetadata(const std::filesystem::path& filepath);
		static const AssetMetadata& GetMetadata(const Ref<Asset>& asset) { return GetMetadata(asset->Handle); }

		static std::filesystem::path GetFileSystemPath(const AssetMetadata& metadata) { return metadata.FilePath; }
		static std::string GetFileSystemPathString(const AssetMetadata& metadata) { return GetFileSystemPath(metadata).string(); }
		static std::filesystem::path GetRelativePath(const std::filesystem::path& path);

		static AssetHandle GetAssetHandleFromFilePath(const std::filesystem::path& filepath);
		static bool IsAssetHandleValid(AssetHandle handle) { return GetMetadata(handle).IsValid(); }

		static AssetType GetAssetTypeFromExtension(const std::string& extension);
		static AssetType GetAssetTypeFromPath(const std::filesystem::path& path);

		static AssetHandle ImportAsset(const std::filesystem::path& filepath);
		static bool ReloadData(AssetHandle handle);

		template<typename T, typename... Args>
		static Ref<T> CreateNewAsset(const std::string& filename, const std::string& directoryPath, Args&&... args) {
			static_assert(std::is_base_of<Asset, T>::value, "CreateNewAsset only works for types of Assets");

			AssetMetadata metadata;
			metadata.Handle = AssetHandle();
			if (directoryPath.empty() || directoryPath == ".")
				metadata.FilePath = filename;
			else
				metadata.FilePath = AssetManager::GetRelativePath(directoryPath + "/" + filename);
			metadata.IsDataLoaded = true;
			metadata.Type = T::GetStaticType();

			if (FileExists(metadata)) {
				bool foundAvailableFileName = false;
				int current = 1;

				while (!foundAvailableFileName) {
					std::string nextFilePath = directoryPath + "/" + metadata.FilePath.stem().string();
					if (current < 10)
						nextFilePath += " (0" + std::to_string(current) + ")";
					else
						nextFilePath += " (" + std::to_string(current) + ")";
					nextFilePath += metadata.FilePath.extension().string();

					if (!FileSystem::Exists(nextFilePath)) {
						foundAvailableFileName = true;
						metadata.FilePath = nextFilePath;
						break;
					}
					current++;
				}
			}

			s_AssetRegistry[metadata.FilePath.string()] = metadata;

			WriteRegistryToFile();

			Ref<T> asset = Ref<T>::Create(std::forward<Args>(args)...);
			asset->Handle = metadata.Handle;
			s_LoadedAssets[asset->Handle] = asset;
			
			AssetImporter::Serialize(metadata, asset);

			return asset;
		}

		template<typename T>
		static Ref<T> GetAsset(AssetHandle assetHandle) {
			if (IsMemoryAsset(assetHandle))
				return s_MemoryAssets[assetHandle].As<T>();

			auto& metadata = GetMetadataInternal(assetHandle);

			Ref<Asset> asset = nullptr;
			if (!metadata.IsDataLoaded) {
				metadata.IsDataLoaded = AssetImporter::TryLoadData(metadata, asset);
				if (!metadata.IsDataLoaded)
					return nullptr;

				s_LoadedAssets[assetHandle] = asset;
			}
			else {
				asset = s_LoadedAssets[assetHandle];
			}
			return asset.As<T>();
		}

		template<typename T>
		static Ref<T> GetAsset(const std::string& filepath) {
			return GetAsset<T>(GetAssetHandleFromFilePath(filepath));
		}

		static bool FileExists(AssetMetadata& metadata) {
			return FileSystem::Exists(metadata.FilePath);
		}

		static const std::unordered_map<AssetHandle, Ref<Asset>>& GetLoadedAssets() { return s_LoadedAssets; }

		template<typename T, typename... Args>
		static AssetHandle CreateMemoryOnlyAsset(Args... args) {
			static_assert(std::is_base_of<Asset, T>::value, "CreateMemoryOnlyAsset only works for types of Assets");

			Ref<T> asset = Ref<T>::Create(std::forward<Args>(args)...);
			asset->Handle = AssetHandle();

			s_MemoryAssets[asset->Handle] = asset;
			return asset->Handle;
		}

		static bool IsMemoryAsset(AssetHandle handle) {
			return s_MemoryAssets.find(handle) != s_MemoryAssets.end();
		}

		static void OnImGuiRender(bool& open);
	private:
		static void LoadAssetRegistry();
		static void ProcessDirectory(const std::filesystem::path& path);
		static void ReloadAssets();
		static void WriteRegistryToFile();

		static AssetMetadata& GetMetadataInternal(AssetHandle handle);

		static void OnFileSystemChanged(const Core::Event::FileSystemChangedEvent& event);
		static void OnAssetRenamed(AssetHandle assetHandle, const std::filesystem::path& newFilePath);
		static void OnAssetMoved(AssetHandle assetHandle, const std::filesystem::path& dstPath);
		static void OnAssetDeleted(AssetHandle assetHandle);


		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		static std::unordered_map<AssetHandle, Ref<Asset>> s_MemoryAssets;

		inline static AssetRegistry s_AssetRegistry;
	};
}