#include <spch.h>

#include "Snow/Editor/ContentBrowser/ContentBrowserItem.h"
#include "Snow/Editor/ContentBrowserPanel.h"

#include "Snow/Asset/AssetManager.h"

#include "Snow/ImGui/ImGui.h"
#include "Snow/ImGui/ImGuiUtilities.h"

#include "Snow/Core/Input.h"

namespace Snow {
	static char s_RenameBuffer[MAX_INPUT_BUFFER_LEN];

	ContentBrowserItem::ContentBrowserItem(ItemType type, AssetHandle id, const std::string& name, const Ref<Render::Texture2D>& texture) : 
		m_Type(type), m_ID(id), m_Name(name), m_Icon(texture) {

	}

	void ContentBrowserItem::OnRenderBegin() {
		ImGui::PushID(&m_ID);
		ImGui::BeginGroup();
	}

	void ContentBrowserItem::OnRenderEnd() {
		ImGui::EndGroup();
		ImGui::PopID();
	}

	CBAResult ContentBrowserItem::OnRender(float size, bool displayAssetType) {

		CBAResult result;

		const float edgeOffset = 4.0f;

		const float textLineHeight = ImGui::GetTextLineHeightWithSpacing() * 2.0f + edgeOffset * 2.0f;
		const float infoPanelHeight = std::max(displayAssetType ? size * 0.5f : textLineHeight, textLineHeight);

		const ImVec2 topLeft = ImGui::GetCursorScreenPos();
		const ImVec2 bottomRight = { topLeft.x + size, topLeft.y + size + infoPanelHeight };
		const ImVec2 infoTopLeft = { topLeft.x, topLeft.y + size };
		const ImVec2 thumbnailBottomRight = { topLeft.x + size, topLeft.y + size };

		auto drawShadow = [](const ImVec2& topLeft, const ImVec2& bottomRight, bool directory) {
			auto* drawList = ImGui::GetWindowDrawList();
			const ImRect itemRect = UI::RectOffset(ImRect(topLeft, bottomRight), 1.0f, 1.0f);
			drawList->AddRect(itemRect.Min, itemRect.Max, Colors::Theme::muted, 6.0f, directory ? 0 : ImDrawFlags_RoundCornersBottom, 2.0f);
		};

		if (m_Type != ItemType::Directory) {
			auto* drawList = ImGui::GetWindowDrawList();

			drawShadow(topLeft, bottomRight, false);

			drawList->AddRectFilled(topLeft, thumbnailBottomRight, Colors::Theme::background);
			drawList->AddRectFilled(infoTopLeft, bottomRight, Colors::Theme::muted, 6.0f, ImDrawFlags_RoundCornersBottom);
		}
		else if (ImGui::ItemHoverable(ImRect(topLeft, bottomRight), ImGui::GetID(&m_ID)) || m_IsSelected) {
			drawShadow(topLeft, bottomRight, true);

			auto* drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(topLeft, bottomRight, Colors::Theme::highlight, 6.0f);
		}

		ImGui::InvisibleButton("##invisibleButton", ImVec2{ size, size });
		UI::DrawButtonImage(m_Icon, IM_COL32(255, 255, 255, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 255, 255, 255), UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f));

		auto renaming = [&] {
			ImGui::SetKeyboardFocusHere();
			if (ImGui::InputText("##rename", s_RenameBuffer, MAX_INPUT_BUFFER_LEN, ImGuiInputTextFlags_EnterReturnsTrue)) {
				Rename(s_RenameBuffer);

				result.Set(ContentBrowserAction::Renamed, true);
			}
		};

		if (m_Type == ItemType::Directory) {
			//ImGui::BeginVertical()
			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (size - edgeOffset * 3.0f));
			const float textWidth = std::min(ImGui::CalcTextSize(m_Name.c_str()).x, size);
			if (false) {
				ImGui::SetNextItemWidth(size - edgeOffset * 3.0f);
				renaming();
			}
			else {
				ImGui::SetNextItemWidth(textWidth);
				ImGui::Text(m_Name.c_str());
			}
			ImGui::PopTextWrapPos();
		}
		else {
			
			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (size - edgeOffset * 2.0f));
			const float textWidth = std::min(ImGui::CalcTextSize(m_Name.c_str()).x, size);
			if (false) {
				ImGui::SetNextItemWidth(size - edgeOffset * 3.0f);
				renaming();
			}
			else {
				ImGui::Text(m_Name.c_str());
			}
			ImGui::PopTextWrapPos();

			if (displayAssetType) {
				{
					const AssetMetadata& metadata = AssetManager::GetMetadata(m_ID);
					const std::string& type = Utils::AssetTypeToString(metadata.Type);
					UI::ScopedColor textColor(ImGuiCol_Text, Colors::Theme::textDark);
					ImGui::TextUnformatted(type.c_str());
				}
			}
		}

		if (true) {
			//if (Core::Input::IsKeyPressed(KeyCode::F2))
				//StartRenaming();
		}

		if (m_IsSelected || ImGui::IsItemHovered()) {
			ImRect itemRect = UI::GetItemRect();
			auto* drawList = ImGui::GetWindowDrawList();
			if (m_IsSelected) {
				const bool mouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsItemHovered();
				ImColor colTransition = UI::ColorWithMultipliedValue(Colors::Theme::selection, 0.8f);

				drawList->AddRect(itemRect.Min, itemRect.Max, 
					mouseDown ? colTransition : Colors::Theme::selection, 6.0f, 
					m_Type == ItemType::Directory ? 0 : ImDrawFlags_RoundCornersBottom, 1.0f);
			}
			else {
				if (m_Type == ItemType::Asset) {
					drawList->AddRect(itemRect.Min, itemRect.Max,
						Colors::Theme::muted, 6.0f,
						ImDrawFlags_RoundCornersBottom, 1.0f);
				}
			}
		}

		UpdateDrop(result);

		bool dragging = false;
		if (dragging = ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
			m_IsDragging = true;

			const auto& selectionStack = ContentBrowserPanel::Get().GetSelectionStack();
			if (!selectionStack.IsSelected(m_ID))
				result.Set(ContentBrowserAction::ClearSelections, true);

			auto& items = ContentBrowserPanel::Get().GetItems();

			if (selectionStack.SelectionCount() > 0) {
				for (const auto& selectedItems : selectionStack) {
					size_t index = items.FindItem(selectedItems);
					if (index == ContentBrowserItemList::InvalidItem)
						continue;

					const auto& item = items[index];
					UI::Image(item->GetIcon(), ImVec2{ 20,20 });
					ImGui::SameLine();
					const auto& name = item->GetName();
					ImGui::TextUnformatted(name.c_str());
				}
				ImGui::SetDragDropPayload("assetPayload", selectionStack.SelectionData(), sizeof(AssetHandle) * selectionStack.SelectionCount());
			}

			result.Set(ContentBrowserAction::Selected, true);
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered()) {
			result.Set(ContentBrowserAction::Hovered, true);

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				Activate(result);
			}
			else {
				const auto& selectionStack = ContentBrowserPanel::Get().GetSelectionStack();
				bool action = selectionStack.SelectionCount() > 1 ? ImGui::IsMouseReleased(ImGuiMouseButton_Left) : ImGui::IsMouseClicked(ImGuiMouseButton_Left);
				bool skipDrag = m_IsDragging && selectionStack.IsSelected(m_ID);
				if (action && !skipDrag) {
					result.Set(ContentBrowserAction::Selected, true);

					if (!Core::Input::IsKeyPressed(KeyCode::LeftControl) && !Core::Input::IsKeyPressed(KeyCode::LeftShift))
						result.Set(ContentBrowserAction::ClearSelections, true);
					if (Core::Input::IsKeyPressed(KeyCode::LeftShift))
						result.Set(ContentBrowserAction::SelectToHere, true);
				}
			}
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4.0f, 4.0f });
		if (ImGui::BeginPopupContextItem("CBIContextMenu")) {
			result.Set(ContentBrowserAction::Selected, true);

			if (!Core::Input::IsKeyPressed(KeyCode::LeftControl) && !Core::Input::IsKeyPressed(KeyCode::LeftShift))
				result.Set(ContentBrowserAction::ClearSelections, true);
			if (Core::Input::IsKeyPressed(KeyCode::LeftShift))
				result.Set(ContentBrowserAction::SelectToHere, true);

			OnContextMenuOpen(result);
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();

		m_IsDragging = dragging;

		return result;
	}

	void ContentBrowserItem::Rename(const std::string& name) {
		OnRenamed(name);
	}

	void ContentBrowserItem::OnContextMenuOpen(CBAResult& result) {
		if (ImGui::MenuItem("Reload"))
			result.Set(ContentBrowserAction::Reload, true);

		if (ImGui::MenuItem("Rename"))
			result.Set(ContentBrowserAction::Renamed, true);

		if (ImGui::MenuItem("Copy"))
			result.Set(ContentBrowserAction::Copy, true);

		if (ImGui::MenuItem("Delete"))
			result.Set(ContentBrowserAction::OpenDeleteDialoge, true);

		ImGui::Separator();

		if (ImGui::MenuItem("Show In Explorer"))
			result.Set(ContentBrowserAction::ShowInExplorer, true);

		if (ImGui::MenuItem("Open External"))
			result.Set(ContentBrowserAction::OpenExternal, true);
	}

	ContentBrowserDirectory::ContentBrowserDirectory(const Ref<DirectoryInfo>& directoryInfo, const Ref<Render::Texture2D>& icon) :
		ContentBrowserItem(ContentBrowserItem::ItemType::Directory, directoryInfo->Handle, directoryInfo->FilePath.filename().string(), icon), m_DirectoryInfo(directoryInfo) {

	}

	ContentBrowserDirectory::~ContentBrowserDirectory() {}

	void ContentBrowserDirectory::Delete() {
		bool deleted = FileSystem::DeleteFile(Project::GetActive()->GetAssetDirectory() / m_DirectoryInfo->FilePath);
		if (!deleted) {
			SNOW_CORE_ERROR("Failed to delete folder {0}", m_DirectoryInfo->FilePath);
			return;
		}

		for (auto asset : m_DirectoryInfo->Assets)
			AssetManager::OnAssetDeleted(asset);
	}

	bool ContentBrowserDirectory::Move(const std::filesystem::path& path) {
		bool moved = FileSystem::MoveFile(Project::GetActive()->GetAssetDirectory() / m_DirectoryInfo->FilePath, Project::GetActive()->GetAssetDirectory() / path);

		return moved;

		//if (!moved)
		//	return false;

		//return true;
	}

	void ContentBrowserDirectory::Activate(CBAResult& result) {
		result.Set(ContentBrowserAction::NavigateTo, true);
	}

	void ContentBrowserDirectory::OnRenamed(const std::string& name) {
		if (FileSystem::Exists(Project::GetActive()->GetAssetDirectory() / m_DirectoryInfo->FilePath.parent_path() / name)) {
			SNOW_CORE_ERROR("A Directory with that name already exists");
			return;
		}

		FileSystem::Rename(Project::GetActive()->GetAssetDirectory() / m_DirectoryInfo->FilePath, Project::GetActive()->GetAssetDirectory() / m_DirectoryInfo->FilePath.parent_path() / name);
	}

	void ContentBrowserDirectory::UpdateDrop(CBAResult& result) {
		if (IsSelected())
			return;

		if (ImGui::BeginDragDropTarget()) {
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("assetPayload");

			if (payload) {
				auto& items = ContentBrowserPanel::Get().GetItems();
				uint32_t count = payload->DataSize / sizeof(AssetHandle);

				for (uint32_t i = 0; i < count; i++) {
					AssetHandle handle = *(((AssetHandle*)payload->Data) + i);
					size_t index = items.FindItem(handle);
					if (index != ContentBrowserItemList::InvalidItem) {
						if (items[index]->Move(m_DirectoryInfo->FilePath)) {
							result.Set(ContentBrowserAction::Refresh, true);
							items.erase(handle);
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	ContentBrowserAsset::ContentBrowserAsset(const AssetMetadata& assetMetadata, const Ref<Render::Texture2D>& icon) :
		ContentBrowserItem(ContentBrowserItem::ItemType::Asset, assetMetadata.Handle, assetMetadata.FilePath.stem().string(), icon), m_AssetMetadata(assetMetadata) {

	}

	ContentBrowserAsset::~ContentBrowserAsset() {}

	void ContentBrowserAsset::Delete() {
		auto path = AssetManager::GetFileSystemPath(m_AssetMetadata);
		bool deleted = FileSystem::DeleteFile(path);
		if (!deleted) {
			SNOW_CORE_ERROR("Failed to delete file {0}", m_AssetMetadata.FilePath);
			return;
		}

		auto& currentDirectory = ContentBrowserPanel::Get().GetDirectory(m_AssetMetadata.FilePath.parent_path());
		currentDirectory->Assets.erase(std::remove(currentDirectory->Assets.begin(), currentDirectory->Assets.end(), m_AssetMetadata.Handle), currentDirectory->Assets.end());

		AssetManager::OnAssetDeleted(m_AssetMetadata.Handle);
	}

	bool ContentBrowserAsset::Move(const std::filesystem::path& path) {
		auto origPath = AssetManager::GetFileSystemPath(m_AssetMetadata);
		bool moved = FileSystem::MoveFile(origPath, Project::GetActive()->GetAssetDirectory() / path);

		if (!moved) {
			SNOW_CORE_ERROR("Couldn't move file from {0} to {1}", m_AssetMetadata.FilePath, path);
			return false;
		}

		AssetManager::OnAssetMoved(m_AssetMetadata.Handle, path);
		return true;
	}

	void ContentBrowserAsset::OnRenamed(const std::string& name) {
		auto filePath = AssetManager::GetFileSystemPath(m_AssetMetadata);
		std::filesystem::path newPath = fmt::format("{0}\\{1}{2}", filePath.parent_path().string(), name, filePath.extension().string());

		if(!FileSystem::Rename(filePath, newPath))
			SNOW_CORE_ERROR("A file with that name already exists");
	}

	void ContentBrowserAsset::Activate(CBAResult& result) {

	}
}