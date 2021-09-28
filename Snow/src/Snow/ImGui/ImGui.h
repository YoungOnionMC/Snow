#pragma once

#include "Snow/Render/Image.h"
#include "Snow/Render/Texture.h"

#include <imgui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>


namespace Snow {
	namespace UI {

		static int s_UIContextID = 0;
		static uint32_t s_Counter = 0;
		static char s_IDBuffer[16];

		static const char* GenerateID() {
			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			return &s_IDBuffer[0];
		}

		static bool IsMouseEnabled() {
			return ImGui::GetIO().ConfigFlags & ~ImGuiConfigFlags_NoMouse;
		}

		static void SetMouseEnabled(const bool enable) {
			if(enable)
				ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			else
				ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		}

		static void PushID() {
			ImGui::PushID(s_UIContextID++);
			s_Counter = 0;
		}

		static void PopID() {
			ImGui::PopID();
			s_UIContextID--;
		}

		static void BeginPropertyGrid() {
			PushID();
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
			ImGui::Columns(2);
		}

		static void EndPropertyGrid() {
			ImGui::Columns(1);
			ImGui::PopStyleVar(2);
			ImGui::PopID();
		}

		static bool BeginTreeNode(const char* name, bool defaultOpen = true) {
			ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
			if(defaultOpen)
				treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

			return ImGui::TreeNodeEx(name, treeNodeFlags);
		}

		static void EndTreeNode() {
			ImGui::TreePop();
		}

		static bool PropertyGridHeader(const std::string& name, bool openByDefault = true) {
			ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_AllowItemOverlap |
				ImGuiTreeNodeFlags_FramePadding;

			if (openByDefault)
				treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

			bool open = false;
			const float framePaddingX = 6.0f;
			const float framePaddingY = 6.0f;

			ImGui::PushID(name.c_str());
			open = ImGui::TreeNodeEx("##dummy_id", treeNodeFlags);// , Utils::ToUpper(name).c_str());
			ImGui::PopID();

			return open;
		}

		static void Separator() {
			ImGui::Separator();
		}

		static bool Property(const char* label, std::string& value) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			char buffer[256];
			strcpy_s<256>(buffer, value.c_str());

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::InputText(s_IDBuffer, buffer, 256)) {
				value = buffer;
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static void Property(const char* label, const std::string& value) {
			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			itoa(s_Counter++, s_IDBuffer + 2, 14);

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			ImGui::InputText(s_IDBuffer, (char*)value.c_str(), value.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopStyleVar();

			ImGui::PopItemWidth();
			ImGui::NextColumn();

		}

		static void Property(const char* label, const char* value) {
			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			ImGui::InputText(s_IDBuffer, (char*)value, 256, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopStyleVar();

			ImGui::PopItemWidth();
			ImGui::NextColumn();

		}

		static bool Property(const char* label, char* value, size_t length) {
			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			bool changed = ImGui::InputText(s_IDBuffer, value, length);

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return changed;
		}

		static bool Property(const char* label, bool& value) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::Checkbox(s_IDBuffer, &value)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool Property(const char* label, char& value) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::DragInt(s_IDBuffer, (int*)&value, 1.0f, -256, 255)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool Property(const char* label, int& value) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::DragInt(s_IDBuffer, &value)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool Property(const char* label, int& value, int min, int max) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::SliderInt(s_IDBuffer, &value, min, max)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool Property(const char* label, float& value, float min, float max) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::SliderFloat(s_IDBuffer, &value, min, max)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool Property(const char* label, glm::vec2& value, float min, float max) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::SliderFloat2(s_IDBuffer, glm::value_ptr(value), min, max)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool Property(const char* label, glm::vec3& value, float min, float max) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::SliderFloat3(s_IDBuffer, glm::value_ptr(value), min, max)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool Property(const char* label, glm::vec4& value, float min, float max) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::SliderFloat4(s_IDBuffer, glm::value_ptr(value), min, max)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool Property(const char* label, float& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::DragFloat(s_IDBuffer, &value, delta, min, max)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool Property(const char* label, glm::vec2& value, float delta = 0.1f) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::DragFloat2(s_IDBuffer, glm::value_ptr(value), delta)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool Property(const char* label, glm::vec3& value, float delta = 0.1f) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::DragFloat3(s_IDBuffer, glm::value_ptr(value), delta)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool Property(const char* label, glm::vec4& value, float delta = 0.1f) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::DragFloat4(s_IDBuffer, glm::value_ptr(value), delta)) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		enum class PropertyAssetReferenceError {
			None = 0, InvalidMetadata = 1
		};

		static AssetHandle s_PropertyAssetReferenceAssetHandle;

		struct PropertyAssetReferenceSettings {
			bool AdvanceToNextColumn = true;
			bool NoItemSpacing = false;
			float WidthOffset = 0.0f;
		};

		template<typename T, typename Fn>
		static bool PropertyAssetReferenceTarget(const char* label, const char* assetName, Ref<T> source, Fn&& targetFunc, const PropertyAssetReferenceSettings& settings = PropertyAssetReferenceSettings()) {
			bool modified = true;

			ImGui::Text(label);
			ImGui::NextColumn();

			ImGui::PushItemWidth(-1);
			if (settings.NoItemSpacing)
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });

			ImVec2 originalButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
			ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
			float width = ImGui::GetContentRegionAvail().x - settings.WidthOffset;
			UI::PushID();

			float lineHeight = 28.0f;

			if (source) {
				if (!source->IsFlagSet(AssetFlag::Missing)) {
					if (assetName)
						ImGui::Button((char*)assetName, { width, lineHeight });
					else {
						assetName = "testAssetname";
						ImGui::Button((char*)assetName, { width, lineHeight });
					}
				}
				else
					ImGui::Button("Missing", { width, lineHeight });
			}
			else {
				ImGui::Button("Null", { width, lineHeight });
			}
			UI::PopID();
			ImGui::GetStyle().ButtonTextAlign = originalButtonTextAlign;

			if (ImGui::BeginDragDropTarget()) {
				auto data = ImGui::AcceptDragDropPayload("assetPayload");

				if (data) {
					AssetHandle assetHandle = *(AssetHandle*)data->Data;
					s_PropertyAssetReferenceAssetHandle = assetHandle;
					//Ref<Asset> asset = AssetManager::GetAsset<Asset>(assetHandle);
					//if (asset && asset->GetAssetType() == T::GetStaticType()) {
					//	targetFunc(asset.As<T>());
					//	modified = true;
					//}
				}
			}

			ImGui::PopItemWidth();
			if (settings.AdvanceToNextColumn)
				ImGui::NextColumn();
			if (settings.NoItemSpacing)
				ImGui::PopStyleVar();

			return modified;
		}

		static bool PropertyColor(const char* label, glm::vec3& value) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::ColorEdit3(s_IDBuffer, glm::value_ptr(value))) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static bool PropertyColor(const char* label, glm::vec4& value) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);


			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::ColorEdit4(s_IDBuffer, glm::value_ptr(value))) {
				modified = true;
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		static int s_CheckboxCount = 0;

		static void BeginCheckboxGroup(const char* label) {
			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);
		}

		static void EndCheckboxGroup() {
			ImGui::PopItemWidth();
			ImGui::NextColumn();
			s_CheckboxCount = 0;
		}

		static bool PropertyCheckboxGroup(const char* label, bool& value) {
			bool modified = false;

			if (++s_CheckboxCount > 1)
				ImGui::SameLine();

			ImGui::Text(label);
			ImGui::SameLine();

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			sprintf_s(s_IDBuffer + 2, 14, "%o", s_Counter++);

			if (ImGui::Checkbox(s_IDBuffer, &value))
				modified = true;

			return modified;
		}

		static bool Button(const char* label, const ImVec2& size = ImVec2(0, 0)) {
			bool result = ImGui::Button(label, size);
			ImGui::NextColumn();
			return result;
		}
		

		void Image(const Ref<Render::Image2D>& image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
		void Image(const Ref<Render::Image2D>& image, uint32_t layer, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
		void ImageMip(const Ref<Render::Image2D>& image, uint32_t mip, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
		void Image(const Ref<Render::Texture2D>& texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
		bool ImageButton(const Ref<Render::Image2D>& image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& background_col = ImVec4(1, 1, 1, 1), const ImVec4& tint_col = ImVec4(0, 0, 0, 0));
		bool ImageButton(const char* stringID, const Ref<Render::Image2D>& image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& background_col = ImVec4(1, 1, 1, 1), const ImVec4& tint_col = ImVec4(0, 0, 0, 0));
		bool ImageButton(const Ref<Render::Texture2D>& texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& background_col = ImVec4(1, 1, 1, 1), const ImVec4& tint_col = ImVec4(0, 0, 0, 0));
		bool ImageButton(const char* stringID, const Ref<Render::Texture2D>& texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& background_col = ImVec4(1, 1, 1, 1), const ImVec4& tint_col = ImVec4(0, 0, 0, 0));
	}
}