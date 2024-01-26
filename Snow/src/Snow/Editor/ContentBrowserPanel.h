#pragma once

#include "Snow/Project/Project.h"

#include "Snow/Render/Texture.h"

#include "Snow/Editor/ContentBrowser/ContentBrowserItem.h"
#include "Snow/Asset/AssetManager.h"

#include "Snow/Core/Event/KeyEvent.h"

namespace Snow {
	class SelectionStack {
	public:
		void Select(AssetHandle handle) {
			m_Selections.push_back(handle);
		}

		void Deselect(AssetHandle handle) {
			for (auto it = m_Selections.begin(); it != m_Selections.end(); it++) {
				if (handle == *it) {
					m_Selections.erase(it);
					break;
				}
			}
		}

		bool IsSelected(AssetHandle handle) const {
			for (const auto& selectedHandle : m_Selections) {
				if (selectedHandle == handle)
					return true;
			}
			return false;
		}

		void Clear() {
			m_Selections.clear();
		}

		size_t SelectionCount() const { return m_Selections.size(); }
		const AssetHandle* SelectionData() const { return m_Selections.data(); }

		std::vector<AssetHandle>::iterator begin() { return m_Selections.begin(); }
		std::vector<AssetHandle>::iterator end() { return m_Selections.end(); }
		std::vector<AssetHandle>::const_iterator begin() const { return m_Selections.begin(); }
		std::vector<AssetHandle>::const_iterator end() const { return m_Selections.end(); }

	private:
		std::vector<AssetHandle> m_Selections;
	};

	class ContentBrowserPanel : public RefCounted {
	public:
		ContentBrowserPanel() = default;
		ContentBrowserPanel(Ref<Project> project);

		void OnImGuiRender();

		Ref<DirectoryInfo> GetDirectory(const std::filesystem::path& filepath) const;

		void OnEvent(Core::Event::Event& e);

		static ContentBrowserPanel& Get() { return *s_Instance; }

		ContentBrowserItemList& GetItems() { return m_Items; }
		const SelectionStack& GetSelectionStack() const { return m_SelectionStack; }
	private:
		AssetHandle ProcessDirectory(const std::filesystem::path& path, const Ref<DirectoryInfo>& parent);

		void ChangeDirectory(Ref<DirectoryInfo>& directory);

		void DrawDirectoryHierarchy(Ref<DirectoryInfo>& directory);
		void DrawTopBar(float height);
		void DrawBottomBar();

		void DrawItems();

		void Refresh();

		void ClearSelections();

		void UpdateInput();
		bool OnKeyPressedEvent(Core::Event::KeyPressedEvent& event) { return false; }

		void OnFileSystemChanged(const Core::Event::FileSystemChangedEvent& e);

		template<typename T, typename... Args>
		Ref<T> CreateAsset(const std::string& filename, Args&&... args) {
			FileSystem::SkipNextFileSystemChange();
			Ref<T> asset = AssetManager::CreateNewAsset<T>(filename, (m_Project->GetAssetDirectory() / m_CurrentDirectory->FilePath).string(), std::forward<Args>(args)...);
			if (!asset)
				return nullptr;

			m_CurrentDirectory->Assets.push_back(asset->Handle);
			ChangeDirectory(m_CurrentDirectory);

			return asset;
		}

		void CreateIconMap();


		Ref<Project> m_Project;

		//std::filesystem::path m_CurrentDirectory;

		Ref<DirectoryInfo> m_BaseDirectory;
		Ref<DirectoryInfo> m_CurrentDirectory;
		Ref<DirectoryInfo> m_NextDirectory, m_PreviousDirectory;

		ContentBrowserItemList m_Items;
		SelectionStack m_SelectionStack;

		Ref<Render::Texture2D> m_FileIcon;
		Ref<Render::Texture2D> m_FolderIcon;
		Ref<Render::Texture2D> m_RefreshIcon;

		std::map<std::string, Ref<Render::Texture2D>> m_IconSet;

		Ref<Render::Texture2D> m_BackButtonIcon;
		Ref<Render::Texture2D> m_ForwardButtonIcon;


		std::unordered_map<AssetHandle, Ref<DirectoryInfo>> m_Directories;

		bool m_ContentBrowserHovered, m_ContentBrowserFocused;
		bool m_AnyItemHovered;

		static ContentBrowserPanel* s_Instance;
	};
}