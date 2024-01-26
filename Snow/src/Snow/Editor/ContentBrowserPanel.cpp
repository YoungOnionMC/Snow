#include <spch.h>

#include "Snow/Editor/ContentBrowserPanel.h"
#include "Snow/Asset/AssetManager.h"
#include "Snow/Scene/Scene.h"
#include "Snow/Utils/FileDialogs.h"
#include "Snow/Core/Input.h"


#include "Snow/ImGui/TreeNode.h"
#include "Snow/ImGui/ImGui.h"
#include "Snow/ImGui/ImGuiUtilities.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Snow {
	namespace UI {
		static bool TreeNode(const std::string& id, const std::string& label, ImGuiTreeNodeFlags flags = 0, const Ref<Render::Texture2D>& icon = nullptr) {
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;
			return UI::TreeNodeWithIcon(icon, window->GetID(id.c_str()), flags, label.c_str(), NULL);

			
		}
	}

	ContentBrowserPanel* ContentBrowserPanel::s_Instance;

	static float s_Padding = 2.0f;
	static float s_ThumbnailSize = 128.0f;

	void ContentBrowserPanel::CreateIconMap() {
		// Scenes
		m_IconSet[".scene"] = Render::Texture2D::Create("Resources/Editor/scene.png");

		// Images
		m_IconSet[".jpeg"] = Render::Texture2D::Create("Resources/Editor/scene.png");
		m_IconSet[".jpg"] = Render::Texture2D::Create("Resources/Editor/scene.png");
		m_IconSet[".png"] = Render::Texture2D::Create("Resources/Editor/scene.png");
		m_IconSet[".tga"] = Render::Texture2D::Create("Resources/Editor/scene.png");

		m_IconSet[".hdr"] = Render::Texture2D::Create("Resources/Editor/scene.png");

		// Meshes
		m_IconSet[".mesh"] = Render::Texture2D::Create("Resources/Editor/scene.png");

		m_IconSet[".fbx"] = Render::Texture2D::Create("Resources/Editor/scene.png");
		m_IconSet[".gltf"] = Render::Texture2D::Create("Resources/Editor/scene.png");
		m_IconSet[".glb"] = Render::Texture2D::Create("Resources/Editor/scene.png");
		m_IconSet[".obj"] = Render::Texture2D::Create("Resources/Editor/scene.png");

		// Materials
		m_IconSet[".mat"] = Render::Texture2D::Create("Resources/Editor/scene.png");

		// Audio
		m_IconSet[".wav"] = Render::Texture2D::Create("Resources/Editor/scene.png");
		m_IconSet[".ogg"] = Render::Texture2D::Create("Resources/Editor/scene.png");
		m_IconSet[".mp3"] = Render::Texture2D::Create("Resources/Editor/scene.png");

		// Scripting
		m_IconSet[".cs"] = Render::Texture2D::Create("Resources/Editor/scene.png");

		// Prefab
		m_IconSet[".prefab"] = Render::Texture2D::Create("Resources/Editor/scene.png");
	}

	ContentBrowserPanel::ContentBrowserPanel(Ref<Project> project) :
		m_Project(project) {
		s_Instance = this;

		AssetManager::SetAssetChangeCallback(SNOW_BIND_EVENT_FN(ContentBrowserPanel::OnFileSystemChanged));

		Render::TextureProperties textureProps;
		textureProps.SamplerWrap = Render::TextureWrap::Clamp;

		m_FolderIcon = Render::Texture2D::Create("Resources/Editor/folder.png", textureProps);

		m_RefreshIcon = Render::Texture2D::Create("Resources/Editor/refresh.png");
		m_BackButtonIcon = Render::Texture2D::Create("Resources/Editor/backbutton.png");
		m_ForwardButtonIcon = Render::Texture2D::Create("Resources/Editor/forwardbutton.png");

		CreateIconMap();

		AssetHandle baseDirectoryHandle = ProcessDirectory(project->GetAssetDirectory().string(), nullptr);
		m_BaseDirectory = m_Directories[baseDirectoryHandle];
		ChangeDirectory(m_BaseDirectory);
	}

	AssetHandle ContentBrowserPanel::ProcessDirectory(const std::filesystem::path& directoryPath, const Ref<DirectoryInfo>& parent) {
		const auto& directory = GetDirectory(directoryPath);
		if (directory)
			return directory->Handle;

		Ref<DirectoryInfo> info = Ref<DirectoryInfo>::Create();
		info->Handle = AssetHandle();
		info->Parent = parent;

		if (directoryPath == m_Project->GetAssetDirectory())
			info->FilePath = "";
		else
			info->FilePath = std::filesystem::relative(directoryPath, m_Project->GetAssetDirectory());

		for (auto entry : std::filesystem::directory_iterator(directoryPath)) {
			if (entry.is_directory()) {
				AssetHandle subDirHandle = ProcessDirectory(entry.path(), info);
				info->SubDirectories[subDirHandle] = m_Directories[subDirHandle];
				continue;
			}

			auto metadata = AssetManager::GetMetadata(std::filesystem::relative(entry.path(), m_Project->GetAssetDirectory()));
			if (!metadata.IsValid()) {
				AssetType type = AssetManager::GetAssetTypeFromPath(entry.path());
				if (type == AssetType::None)
					continue;

				metadata.Handle = AssetManager::ImportAsset(entry.path());
			}

			if (!metadata.IsValid())
				continue;

			info->Assets.push_back(metadata.Handle);
		}
		m_Directories[info->Handle] = info;
		return info->Handle;
	}

	Ref<DirectoryInfo> ContentBrowserPanel::GetDirectory(const std::filesystem::path& path) const {
		if (path.string() == "" || path.string() == ".")
			return m_BaseDirectory;

		for (const auto& [handle, directory] : m_Directories) {
			if (directory->FilePath == path)
				return directory;
		}

		return nullptr;
	}

	void ContentBrowserPanel::ChangeDirectory(Ref<DirectoryInfo>& directory) {
		if (!directory)
			return;

		m_Items.Items.clear();

		for (auto& [subDirHandle, subDir] : directory->SubDirectories)
			m_Items.Items.push_back(Ref<ContentBrowserDirectory>::Create(subDir, m_FolderIcon));

		std::vector<AssetHandle> invalidAssets;
		for (auto assetHandle : directory->Assets) {
			AssetMetadata metadata = AssetManager::GetMetadata(assetHandle);
			if (metadata.IsValid()) {
				m_Items.Items.push_back(Ref<ContentBrowserAsset>::Create(metadata, 
					m_IconSet.find(metadata.FilePath.extension().string()) != m_IconSet.end() ? m_IconSet[metadata.FilePath.extension().string()] : m_FileIcon));
			}
		}

		m_PreviousDirectory = directory;
		m_CurrentDirectory = directory;
	}

	void ContentBrowserPanel::OnEvent(Core::Event::Event& event) {
		Core::Event::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<Core::Event::KeyPressedEvent>([this](Core::Event::KeyPressedEvent& e) {return OnKeyPressedEvent(e); });
	}

	void ContentBrowserPanel::DrawDirectoryHierarchy(Ref<DirectoryInfo>& directory) {
		std::string name = directory->FilePath.filename().string();
		std::string id = name + "_TreeNode";
		bool prevState = ImGui::TreeNodeBehaviorIsOpen(ImGui::GetID(id.c_str()));

		auto* window = ImGui::GetCurrentWindow();
		window->DC.CurrLineSize.y = 20.0f;
		window->DC.CurrLineTextBaseOffset = 3.0f;

		const ImRect itemRect = { window->WorkRect.Min.x, window->DC.CursorPos.y, window->WorkRect.Max.x, window->DC.CursorPos.y + window->DC.CurrLineSize.y };

		const bool isItemClicked = [&itemRect, &id] {
			if (ImGui::ItemHoverable(itemRect, ImGui::GetID(id.c_str())))
				return ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Left);
			return false;
		}();

		auto fillWithColor = [&](const ImColor& color) {
			const ImU32 bgColor = ImGui::ColorConvertFloat4ToU32(color);
			ImGui::GetWindowDrawList()->AddRectFilled(itemRect.Min, itemRect.Max, bgColor);
		};

		const bool isWindowFocused = ImGui::IsWindowFocused();

		auto checkIfAnyDescSelected = [&](Ref<DirectoryInfo>& directory, auto isAnyDescendantSelected) -> bool {
			if (directory->Handle == m_CurrentDirectory->Handle)
				return true;

			if (!directory->SubDirectories.empty()) {
				for (auto& [chandle, cDir] : directory->SubDirectories) {
					if (isAnyDescendantSelected(cDir, isAnyDescendantSelected))
						return true;
				}
			}
			return false;
		};

		const bool isAnyDescendantSelected = checkIfAnyDescSelected(directory, checkIfAnyDescSelected);
		const bool isActiveDirectory = directory->Handle == m_CurrentDirectory->Handle;

		ImGuiTreeNodeFlags flags = (isActiveDirectory ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_SpanFullWidth;
		if (isActiveDirectory || isItemClicked) {
			if (isWindowFocused)
				fillWithColor(Colors::Theme::selection);
			else {
				const ImColor col = UI::ColorWithMultipliedValue(Colors::Theme::selection, 0.8f);
				fillWithColor(UI::ColorWithMultipliedSaturation(col, 0.7f));
			}

			ImGui::PushStyleColor(ImGuiCol_Text, Colors::Theme::backgroundDark);
		}
		else if (isAnyDescendantSelected)
			fillWithColor(Colors::Theme::selectionMuted);

		bool open = UI::TreeNode(id, name, flags, m_FolderIcon);

		if (isActiveDirectory || isItemClicked)
			ImGui::PopStyleColor();

		if (open) {
			for (auto& [handle, child] : directory->SubDirectories)
				DrawDirectoryHierarchy(child);
		}

		if (open != prevState && !isActiveDirectory) {
			if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.01))
				ChangeDirectory(directory);
		}

		if (open)
			ImGui::TreePop();
	}

	static std::mutex s_LockMutex;

	void ContentBrowserPanel::DrawItems() {
		m_AnyItemHovered = false;
		bool openDeleteDialog = false;

		std::lock_guard<std::mutex> lock(s_LockMutex);

		for (auto& item : m_Items) {
			item->OnRenderBegin();

			auto result = item->OnRender(s_ThumbnailSize, true);

			if (result.IsSet(ContentBrowserAction::ClearSelections))
				ClearSelections();

			if (result.IsSet(ContentBrowserAction::Selected) && !m_SelectionStack.IsSelected(item->GetID())) {
				m_SelectionStack.Select(item->GetID());
				item->SetSelected(true);
			}

			if (result.IsSet(ContentBrowserAction::SelectToHere) && m_SelectionStack.SelectionCount() == 2) {

			}

			if (result.IsSet(ContentBrowserAction::OpenDeleteDialoge))
				openDeleteDialog = true;

			if (result.IsSet(ContentBrowserAction::ShowInExplorer)) {
				if (item->GetType() == ContentBrowserItem::ItemType::Directory)
					FileSystem::ShowFileInExplorer(m_Project->GetAssetDirectory() / m_CurrentDirectory->FilePath / item->GetName());
				else
					FileSystem::ShowFileInExplorer(AssetManager::GetFileSystemPath(AssetManager::GetMetadata(item->GetID())));
			}

			if (result.IsSet(ContentBrowserAction::OpenExternal)) {
				if (item->GetType() == ContentBrowserItem::ItemType::Directory)
					FileSystem::OpenExternally(m_Project->GetAssetDirectory() / m_CurrentDirectory->FilePath / item->GetName());
				else
					FileSystem::OpenExternally(AssetManager::GetFileSystemPath(AssetManager::GetMetadata(item->GetID())));
			}


			if (result.IsSet(ContentBrowserAction::Hovered))
				m_AnyItemHovered = true;

			item->OnRenderEnd();

			if (result.IsSet(ContentBrowserAction::Renamed)) {
				break;
			}

			if (result.IsSet(ContentBrowserAction::NavigateTo)) {
				ChangeDirectory(item.As<ContentBrowserDirectory>()->GetDirectoryInfo());
				break;
			}

			if (result.IsSet(ContentBrowserAction::Refresh)) {
				ChangeDirectory(m_CurrentDirectory);
				break;
			}

			

			ImGui::NextColumn();
		}

		if (openDeleteDialog) {
			ImGui::OpenPopup("Delete");
			openDeleteDialog = false;
		}
	}

	void ContentBrowserPanel::DrawTopBar(float height) {
		ImGui::BeginChild("##top_bar", ImVec2(0, height));
		//ImGui::horizontal
		{
			const float edgeOffset = 4.0f;

			auto contentBrowserButton = [height](const char* labelID, const Ref<Texture2D>& icon) {
				const ImU32 buttonCol = Colors::Theme::background;
				const ImU32 buttonColP = UI::ColorWithMultipliedValue(Colors::Theme::background, 0.8f);

				const float iconSize = std::min(24.0f, height);
				const float iconPadding = 3.0f;
				const bool clicked = ImGui::Button(labelID, ImVec2(iconSize, iconSize));
				UI::DrawButtonImage(icon, Colors::Theme::textDark, Colors::Theme::textDark, Colors::Theme::textDark, UI::RectExpanded(UI::GetItemRect(), -iconPadding, -iconPadding));

				return clicked;
			};

			if (contentBrowserButton("##back", m_BackButtonIcon)) {
				m_NextDirectory = m_CurrentDirectory;
				m_PreviousDirectory = m_CurrentDirectory->Parent;
				ChangeDirectory(m_PreviousDirectory);
			}
			ImGui::SameLine();

			if (contentBrowserButton("##forward", m_ForwardButtonIcon)) {
				ChangeDirectory(m_NextDirectory);
			}
			ImGui::SameLine();

			if (contentBrowserButton("##refresh", m_RefreshIcon)) {
				Refresh();
			}
		}
		ImGui::EndChild();
	}

	void ContentBrowserPanel::OnFileSystemChanged(const Core::Event::FileSystemChangedEvent& e) {
		Refresh(); 
	}

	

	void ContentBrowserPanel::Refresh() {
		std::lock_guard<std::mutex> lock(s_LockMutex);

		m_Directories.clear();

		Ref<DirectoryInfo> currentDirectory = m_CurrentDirectory;
		AssetHandle baseDirectoryHandle = ProcessDirectory(m_Project->GetAssetDirectory().string(), nullptr);
		m_BaseDirectory = m_Directories[baseDirectoryHandle];
		m_CurrentDirectory = GetDirectory(currentDirectory->FilePath);

		if (!m_CurrentDirectory)
			m_CurrentDirectory = m_BaseDirectory;

		ChangeDirectory(m_CurrentDirectory);
	}

	void ContentBrowserPanel::UpdateInput() {

		if (!m_ContentBrowserHovered)
			return;
		
		if ((!m_AnyItemHovered && ImGui::IsMouseDown(ImGuiMouseButton_Left)) || Core::Input::IsKeyPressed(KeyCode::Escape))
			ClearSelections();

		if (Core::Input::IsKeyPressed(KeyCode::Delete) && m_SelectionStack.SelectionCount() > 0)
			ImGui::OpenPopup("Delete");

		if (Core::Input::IsKeyPressed(KeyCode::F5))
			Refresh();
	}

	void ContentBrowserPanel::ClearSelections() {
		for (auto& item : m_Items) {
			item->SetSelected(false);

			
		}
		m_SelectionStack.Clear();
	}

	void ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser", NULL, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

		m_ContentBrowserHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
		m_ContentBrowserFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

		{
			ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;

			UI::PushID();
			if (ImGui::BeginTable(UI::GenerateID(), 2, tableFlags, ImVec2(0.0f, 0.0f))) {
				ImGui::TableSetupColumn("Outliner", 0, 300.0f);
				ImGui::TableSetupColumn("Directory Structure", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);

				ImGui::BeginChild("##folders");
				{
					if (ImGui::CollapsingHeader("Content", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
						UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
						UI::ScopedColorStack itemBG(ImGuiCol_Header, IM_COL32_DISABLE, ImGuiCol_HeaderActive, IM_COL32_DISABLE);

						//if (m_BaseDirectory == nullptr)
						//	return;
						for (auto& [handle, directory] : m_BaseDirectory->SubDirectories)
							DrawDirectoryHierarchy(directory);
					}
				}
				ImGui::EndChild();

				ImGui::TableSetColumnIndex(1);

				const float topBarHeight = 25.0f;
				const float bottomBarHeight = 50.0f;

				ImGui::BeginChild("##directory_structure", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - topBarHeight - bottomBarHeight));
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
					DrawTopBar(topBarHeight);
					ImGui::PopStyleVar();

					ImGui::BeginChild("Scrolling");
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.3f));

						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 4.0f));
						if (ImGui::BeginPopupContextWindow(0, 1, false)) {
							if (ImGui::BeginMenu("New")) {
								if (ImGui::MenuItem("Folder")) {
									bool created = FileSystem::CreateDirectory(Project::GetAssetDirectory() / m_CurrentDirectory->FilePath / "New Folder");
									if (created) {
										Refresh();
										const auto& directoryInfo = GetDirectory(m_CurrentDirectory->FilePath / "New Folder");
										size_t index = m_Items.FindItem(directoryInfo->Handle);
										//if (index != ContentBrowserItemList::InvalidItem)
										//	m_Items[index]->StartRenaming();
									}
								}

								if (ImGui::MenuItem("Scene"))
									CreateAsset<Scene>("New Scene.scene");

								//if(ImGui::MenuItem("Shader"))
								//	CreateAsset<Render::Shader>("")

								if (ImGui::MenuItem("Material"))
									CreateAsset<MaterialAsset>("New Material.mat");

								ImGui::EndMenu();
							}
							if (ImGui::MenuItem("Import")) {
								std::string filepath = Snow::Utils::FileDialogs::OpenFile().value();
								if (!filepath.empty())
									FileSystem::MoveFile(filepath, m_CurrentDirectory->FilePath);
							}

							if (ImGui::MenuItem("Refresh"))
								Refresh();

							ImGui::Separator();

							if (ImGui::MenuItem("Show in Explorer")) {
								FileSystem::ShowFileInExplorer(Project::GetAssetDirectory() / m_CurrentDirectory->FilePath);
							}
							ImGui::EndPopup();

						}
						ImGui::PopStyleVar();

						const float paddingOutline = 2.0f;
						const float scrollBarOffset = 20.0f + ImGui::GetStyle().ScrollbarSize;
						float panelWidth = ImGui::GetContentRegionAvail().x - scrollBarOffset;
						float cellSize = s_ThumbnailSize + s_Padding + paddingOutline;
						int columnCount = (int)(panelWidth / cellSize);
						if (columnCount < 1) columnCount = 1;

						{
							const float rowSpace = 12.0f;
							UI::ScopedStyle(ImGuiStyleVar_ItemSpacing, ImVec2{ paddingOutline, rowSpace });
							ImGui::Columns(columnCount, 0, false);

							UI::ScopedStyle border(ImGuiStyleVar_FrameBorderSize, 0.0f);
							UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));


							DrawItems();
						}

						if (ImGui::IsWindowFocused() && !ImGui::IsMouseDragging(ImGuiMouseButton_Left))
							UpdateInput();

						ImGui::PopStyleColor(2);
					}
					ImGui::EndChild();
				}
				ImGui::EndChild();

				if (ImGui::BeginDragDropTarget()) {

				}

				ImGui::EndTable();
			}
			UI::PopID();
		}
		ImGui::End();
	}
}