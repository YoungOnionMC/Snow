#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Asset/Asset.h"
#include "Snow/Asset/AssetMetadata.h"

#include "Snow/Render/Texture.h"

namespace Snow {

#define MAX_INPUT_BUFFER_LEN 128

	enum class ContentBrowserAction {
		None					= 0,
		Refresh					= BIT(0),
		Selected				= BIT(1),
		ClearSelections			= BIT(2),
		Hovered					= BIT(3),
		Renamed					= BIT(4),
		NavigateTo				= BIT(5),
		SelectToHere			= BIT(6),
		Moved					= BIT(7),
		ShowInExplorer			= BIT(8),
		OpenExternal			= BIT(9),
		Reload					= BIT(10),
		Copy					= BIT(11),
		Duplicate				= BIT(12),
		OpenDeleteDialoge		= BIT(13),
	};

	struct CBAResult {
		uint16_t field = 0;

		void Set(ContentBrowserAction action, bool value) {
			if (value) {
				field |= (uint16_t)action;
			}
			else {
				field &= ~(uint16_t)action;
			}
		}

		bool IsSet(ContentBrowserAction action) const { return (uint16_t)action & field; }
	};

	class ContentBrowserItem : public RefCounted {
	public:
		enum class ItemType : uint16_t {
			Directory, Asset
		};

		ContentBrowserItem(ItemType type, AssetHandle id, const std::string& name, const Ref<Render::Texture2D>& texture);

		void OnRenderBegin();
		CBAResult OnRender(float size, bool displayAssetType);
		void OnRenderEnd();

		virtual void Delete() {};
		virtual bool Move(const std::filesystem::path& path) { return false; }

		AssetHandle GetID() const { return m_ID; }
		ItemType GetType() const { return m_Type; }
		const std::string& GetName() const { return m_Name; }
		Ref<Render::Texture2D> GetIcon() const { return m_Icon; }

		bool IsSelected() const { return m_IsSelected; }
		void SetSelected(bool selected) { m_IsSelected = selected; }

		void Rename(const std::string& name);
	protected:
		ItemType m_Type;
		std::string m_Name;
		AssetHandle m_ID;
		Ref<Render::Texture2D> m_Icon;

		bool m_IsSelected = false;
		bool m_IsDragging = false;

	private:
		virtual void OnRenamed(const std::string& name) { m_Name = name; };
		virtual void Activate(CBAResult& result) = 0;
		virtual void UpdateDrop(CBAResult& result) {}

		void OnContextMenuOpen(CBAResult& result);

		friend class ContentBrowserPanel;
	};

	struct DirectoryInfo : public RefCounted {
		AssetHandle Handle;
		Ref<DirectoryInfo> Parent;

		std::filesystem::path FilePath;

		std::vector<AssetHandle> Assets;
		std::unordered_map<AssetHandle, Ref<DirectoryInfo>> SubDirectories;
	};

	class ContentBrowserDirectory : public ContentBrowserItem {
	public:
		ContentBrowserDirectory(const Ref<DirectoryInfo>& directoryInfo, const Ref<Render::Texture2D>& icon);
		virtual ~ContentBrowserDirectory();

		Ref<DirectoryInfo>& GetDirectoryInfo() { return m_DirectoryInfo; }

		virtual void Delete() override;
		virtual bool Move(const std::filesystem::path& path) override;
	private:
		virtual void OnRenamed(const std::string& name) override;
		virtual void Activate(CBAResult& result) override;
		virtual void UpdateDrop(CBAResult& result) override;

		Ref<DirectoryInfo> m_DirectoryInfo;
	};

	class ContentBrowserAsset : public ContentBrowserItem {
	public:
		ContentBrowserAsset(const AssetMetadata& assetMetadata, const Ref<Render::Texture2D>& icon);
		virtual ~ContentBrowserAsset();

		const AssetMetadata& GetAssetInfo() const { return m_AssetMetadata; }

		virtual void Delete() override;
		virtual bool Move(const std::filesystem::path& path) override;
	private:
		virtual void OnRenamed(const std::string& name) override;
		virtual void Activate(CBAResult& result) override;

		AssetMetadata m_AssetMetadata;
	};

	struct ContentBrowserItemList {
		static constexpr size_t InvalidItem = std::numeric_limits<size_t>::max();

		std::vector<Ref<ContentBrowserItem>> Items;

		std::vector<Ref<ContentBrowserItem>>::iterator begin() { return Items.begin(); }
		std::vector<Ref<ContentBrowserItem>>::iterator end() { return Items.end(); }
		std::vector<Ref<ContentBrowserItem>>::const_iterator begin() const { return Items.begin(); }
		std::vector<Ref<ContentBrowserItem>>::const_iterator end() const { return Items.end(); }

		Ref<ContentBrowserItem>& operator[](size_t index) { return Items[index]; }
		const Ref<ContentBrowserItem>& operator[](size_t index) const { return Items[index]; }

		void Clear() {
			Items.clear();
		}

		void erase(AssetHandle handle) {
			size_t index = FindItem(handle);
			if (index == InvalidItem)
				return;

			auto it = Items.begin() + index;
			Items.erase(it);
		}

		size_t FindItem(AssetHandle handle) {
			for (size_t i = 0; i < Items.size(); i++) {
				if (Items[i]->GetID() == handle)
					return i;
			}

			return InvalidItem;
		}
	};
}