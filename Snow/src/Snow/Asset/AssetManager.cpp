#include <spch.h>
#include "Snow/Asset/AssetManager.h"
#include "Snow/Asset/AssetExtensions.h"

#include "Snow/Project/Project.h"

#include "Snow/Utils/StringUtils.h"

#include "Snow/ImGui/ImGui.h"

#include <yaml-cpp/yaml.h>


namespace Snow {

	AssetManager::AssetChangeCallbackFn AssetManager::s_AssetChangeCallback;

	void AssetManager::Init() {
		AssetImporter::Init();

		LoadAssetRegistry();
		FileSystem::SetChangeCallback(AssetManager::OnFileSystemChanged);
		ReloadAssets();
	}

	void AssetManager::Shutdown() {
		WriteRegistryToFile();

		s_MemoryAssets.clear();
		s_AssetRegistry.Clear();
		s_LoadedAssets.clear();
	}

	void AssetManager::SetAssetChangeCallback(const AssetChangeCallbackFn& callback) {
		s_AssetChangeCallback = callback;
	}

	void AssetManager::OnFileSystemChanged(const Core::Event::FileSystemChangedEvent& event) {
		using namespace Core::Event;
		if (event.GetEventType() == EventType::FileModified) {
			AssetHandle handle = GetAssetHandleFromFilePath(event.GetFilePath());
			const auto& metadata = GetMetadata(handle);
			if (metadata.IsValid() && metadata.IsDataLoaded)
				ReloadData(handle);
		}
		else if (event.GetEventType() == EventType::FileRenamed) {
			FileRenamedEvent* ev = std::move((FileRenamedEvent*)&event);
			AssetType previousType = GetAssetTypeFromPath(std::filesystem::path(ev->GetOldName()));
			AssetType newType = GetAssetTypeFromPath(ev->GetFilePath());

			if (previousType == AssetType::None && newType != AssetType::None)
				ImportAsset(ev->GetFilePath());
			else
				OnAssetRenamed(GetAssetHandleFromFilePath(ev->GetFilePath().parent_path() / ev->GetOldName()), ev->GetFilePath());

		}
		else if (event.GetEventType() == EventType::FileDeleted) {
			OnAssetDeleted(GetAssetHandleFromFilePath(event.GetFilePath()));
		}

		s_AssetChangeCallback(event);
	}

	static AssetMetadata s_NullMetadata;
	AssetMetadata& AssetManager::GetMetadataInternal(AssetHandle handle) {
		for (auto& [filepath, metadata] : s_AssetRegistry) {
			if (metadata.Handle == handle)
				return metadata;
		}
		return s_NullMetadata;
	}

	const AssetMetadata& AssetManager::GetMetadata(AssetHandle handle) {
		return GetMetadataInternal(handle);
	}

	const AssetMetadata& AssetManager::GetMetadata(const std::filesystem::path& path) {
		if (s_AssetRegistry.Contains(path))
			return s_AssetRegistry[path];

		return s_NullMetadata;
	}

	std::filesystem::path AssetManager::GetRelativePath(const std::filesystem::path& path) {
		std::string temp = path.string();
		if (temp.find(Project::GetActive()->GetAssetDirectory().string()) != std::string::npos)
			return std::filesystem::relative(path, Project::GetActive()->GetAssetDirectory());
		return path;
	}

	AssetHandle AssetManager::GetAssetHandleFromFilePath(const std::filesystem::path& path) {
		return s_AssetRegistry.Contains(path) ? s_AssetRegistry[path].Handle : 0;
	}

	void AssetManager::OnAssetRenamed(AssetHandle handle, const std::filesystem::path& newPath) {
		AssetMetadata metadata = GetMetadataInternal(handle);
		if (!metadata.IsValid())
			return;

		s_AssetRegistry.Remove(metadata.FilePath);
		metadata.FilePath = newPath;
		s_AssetRegistry[metadata.FilePath] = metadata;
		WriteRegistryToFile();
	}

	void AssetManager::OnAssetMoved(AssetHandle handle, const std::filesystem::path& newPath) {
		AssetMetadata metadata = GetMetadataInternal(handle);
		if (!metadata.IsValid())
			return;

		s_AssetRegistry.Remove(metadata.FilePath);
		metadata.FilePath = newPath / metadata.FilePath.filename();
		s_AssetRegistry[metadata.FilePath] = metadata;
		WriteRegistryToFile();
	}

	void AssetManager::OnAssetDeleted(AssetHandle handle) {
		AssetMetadata metadata = GetMetadataInternal(handle);
		if (!metadata.IsValid())
			return;

		s_AssetRegistry.Remove(metadata.FilePath);
		s_LoadedAssets.erase(handle);
		WriteRegistryToFile();
	}

	AssetType AssetManager::GetAssetTypeFromExtension(const std::string& extension) {
		std::string ext = Utils::String::ToLower(extension);
		if (s_AssetExtensionMap.find(ext) == s_AssetExtensionMap.end())
			return AssetType::None;

		return s_AssetExtensionMap[ext.c_str()];
	}

	AssetType AssetManager::GetAssetTypeFromPath(const std::filesystem::path& path) {
		return GetAssetTypeFromExtension(path.extension().string());
	}

	void AssetManager::LoadAssetRegistry() {
		SNOW_CORE_INFO("[AssetManager] Loading AssetRegistry");

		const auto& assetRegistryPath = Project::GetAssetRegistryPath();
		if (!FileSystem::Exists(assetRegistryPath))
			return;

		std::ifstream stream(assetRegistryPath);
		SNOW_CORE_ASSERT(stream);
		std::stringstream sstream;
		sstream << stream.rdbuf();

		YAML::Node data = YAML::Load(sstream.str());
		auto handles = data["Assets"];
		if (!handles) {
			SNOW_CORE_ERROR("[AssetManager] Asset Registry does not have any handles");
			SNOW_CORE_VERIFY(false);
			return;
		}

		for (auto entry : handles) {
			std::string filepath = entry["FilePath"].as<std::string>();

			AssetMetadata metadata;
			metadata.Handle = entry["Handle"].as<uint64_t>();
			metadata.FilePath = filepath;
			metadata.Type = (AssetType)Utils::AssetTypeFromString(entry["Type"].as<std::string>());

			if (metadata.Type == AssetType::None)
				continue;

			if (!FileSystem::Exists(AssetManager::GetFileSystemPath(metadata))) {
				SNOW_CORE_TRACE("[AssetManager] Missing asset '{0}' detected in registry file, trying to locate...", metadata.FilePath);
			}

			s_AssetRegistry[metadata.FilePath] = metadata;
		}
		SNOW_CORE_INFO("[AssetManager] Loaded {0} asset entries", s_AssetRegistry.Count());
	}

	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& filePath) {
		std::filesystem::path path = GetRelativePath(filePath);

		if (s_AssetRegistry.Contains(path))
			return s_AssetRegistry[path].Handle;

		AssetType type = GetAssetTypeFromPath(path);
		if (type == AssetType::None)
			return 0;

		AssetMetadata metadata;
		metadata.Handle = AssetHandle();
		metadata.FilePath = path;
		metadata.Type = type;
		s_AssetRegistry[metadata.FilePath] = metadata;

		return metadata.Handle;
	}

	bool AssetManager::ReloadData(AssetHandle handle) {
		auto& metadata = GetMetadataInternal(handle);
		if (!metadata.IsDataLoaded) {
			SNOW_CORE_WARN("Trying to reload asset that was never loaded");

			Ref<Asset> asset;
			metadata.IsDataLoaded = AssetImporter::TryLoadData(metadata, asset);
			return metadata.IsDataLoaded;
		}

		SNOW_CORE_ASSERT(s_LoadedAssets.find(handle) != s_LoadedAssets.end());
		Ref<Asset>& asset = s_LoadedAssets.at(handle);
		metadata.IsDataLoaded = AssetImporter::TryLoadData(metadata, asset);
		return metadata.IsDataLoaded;
	}

	void AssetManager::ProcessDirectory(const std::filesystem::path& directoryPath) {
		for (auto entry : std::filesystem::directory_iterator(directoryPath)) {
			if (entry.is_directory())
				ProcessDirectory(entry.path());
			else
				ImportAsset(entry.path());
		}
	}

	void AssetManager::ReloadAssets() {
		ProcessDirectory(Project::GetAssetDirectory().string());
		WriteRegistryToFile();
	}

	void AssetManager::WriteRegistryToFile() {
		struct AssetRegistryEntry {
			std::string FilePath;
			AssetType Type;
		};

		std::map<UUID, AssetRegistryEntry> sortedMap;
		for (auto& [filepath, metadata] : s_AssetRegistry) {
			if (!FileSystem::Exists(GetFileSystemPath(metadata)))
				continue;

			std::string pathToSerialize = metadata.FilePath.string();

			std::replace(pathToSerialize.begin(), pathToSerialize.end(), '\\', '/');
			sortedMap[metadata.Handle] = { pathToSerialize, metadata.Type };
		}

		SNOW_CORE_INFO("[AssetManager] serializing asset registry with {0} entries", sortedMap.size());

		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Assets" << YAML::BeginSeq;
		for (auto& [handle, entry] : sortedMap) {
			out << YAML::BeginMap;
			out << YAML::Key << "Handle" << YAML::Value << handle;
			out << YAML::Key << "FilePath" << YAML::Value << entry.FilePath;
			out << YAML::Key << "Type" << YAML::Value << Utils::AssetTypeToString(entry.Type);
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		FileSystem::SkipNextFileSystemChange();

		const std::string& assetRegistryPath = Project::GetAssetRegistryPath().string();
		std::ofstream fout(assetRegistryPath);
		fout << out.c_str();
	}

	void AssetManager::OnImGuiRender(bool& open) {
		if (!open)
			return;

		ImGui::Begin("Asset Manager", &open);
		if (UI::BeginTreeNode("Registry")) {
			static char searchBuffer[256];
			ImGui::InputText("##regsearch", searchBuffer, 256);
			UI::BeginPropertyGrid();
			static float columnWidth = 0.0f;
			if (columnWidth == 0.0f) {
				ImVec2 textSize = ImGui::CalcTextSize("File Path");
				columnWidth = textSize.x * 2.0f;
				ImGui::SetColumnWidth(0, columnWidth);
			}
			for (const auto& [path, metadata] : s_AssetRegistry) {
				std::string handle = fmt::format("{0}", metadata.Handle);
				std::string filepath = metadata.FilePath.string();
				std::string type = Utils::AssetTypeToString(metadata.Type);
				if (searchBuffer[0] != 0) {
					std::string searchString = searchBuffer;
					
				}
				else {
					UI::Property("Handle", (const std::string&)handle);
					UI::Property("File Path", (const std::string&)filepath);
					UI::Property("Type", (const std::string&)type);
					UI::Separator();
				}
			}

			UI::EndPropertyGrid();
			UI::EndTreeNode();
		}
		ImGui::End();
	}

	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_LoadedAssets;
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_MemoryAssets;
}