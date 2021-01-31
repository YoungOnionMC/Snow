#include "ImGuiUI.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Snow {
	namespace UI {
		static int s_UIContextID = 0;
		static uint32_t s_Counter = 0;
		static char s_IDBuffer[16];

		static void PushID() {
			ImGui::PushID(s_UIContextID++);
			s_Counter = 0;
		}

		static void PopID() {
			ImGui::PopID();
			s_UIContextID--;
		}

		void BeginGrid(uint32_t columns) {
			PushID();
			ImGui::Columns(columns);
		}

		void EndGrid() {
			ImGui::Columns(1);
			PopID();
		}

		bool DrawStringControl(const char* label, std::string& value, bool error) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			char buffer[256];
			strcpy_s(buffer, sizeof(buffer), value.c_str());

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			itoa(s_Counter++, s_IDBuffer + 2, 16);

			if (error)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
			if (ImGui::InputText(s_IDBuffer, buffer, sizeof(buffer))) {
				value = buffer;
				modified = true;
			}
			if (error)
				ImGui::PopStyleColor();
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		bool DrawStringControl(const char* label, const char* value) {
			bool modified = false;
			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			itoa(s_Counter++, s_IDBuffer + 2, 16);
			if (ImGui::InputText(s_IDBuffer, (char*)value, 256, ImGuiInputTextFlags_ReadOnly))
				modified = true;

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		bool DrawBoolControl(const char* label, bool* value) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			itoa(s_Counter++, s_IDBuffer + 2, 16);
			if (ImGui::Checkbox(s_IDBuffer, value))
				modified = true;

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		bool DrawCharControl(const char* label, char* value) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			itoa(s_Counter++, s_IDBuffer + 2, 16);
			if (ImGui::DragInt(s_IDBuffer, (int*)value))
				modified = true;

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		bool DrawIntControl(const char* label, int* value) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			itoa(s_Counter++, s_IDBuffer + 2, 16);
			if (ImGui::DragInt(s_IDBuffer, value))
				modified = true;

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		bool DrawFloatControl(const char* label, float* value) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			itoa(s_Counter++, s_IDBuffer + 2, 16);
			if (ImGui::DragFloat(s_IDBuffer, value, 0.1f))
				modified = true;

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		bool DrawFloat2Control(const char* label, glm::vec2& values, float delta) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			itoa(s_Counter++, s_IDBuffer + 2, 16);
			if (ImGui::DragFloat2(s_IDBuffer, glm::value_ptr(values), delta))
				modified = true;

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		bool DrawFloat3Control(const char* label, glm::vec3& values, float delta) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			itoa(s_Counter++, s_IDBuffer + 2, 16);
			if (ImGui::DragFloat3(s_IDBuffer, glm::value_ptr(values), delta))
				modified = true;

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		bool DrawFloat4Control(const char* label, glm::vec4& values, float delta) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			itoa(s_Counter++, s_IDBuffer + 2, 16);
			if (ImGui::DragFloat4(s_IDBuffer, glm::value_ptr(values), delta))
				modified = true;

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		bool DrawColorControl(const char* label, glm::vec4& values) {
			bool modified = false;

			ImGui::Text(label);
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);

			s_IDBuffer[0] = '#';
			s_IDBuffer[1] = '#';
			memset(s_IDBuffer + 2, 0, 14);
			itoa(s_Counter++, s_IDBuffer + 2, 16);
			if (ImGui::ColorEdit4(s_IDBuffer, glm::value_ptr(values)))
				modified = true;

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			return modified;
		}

		void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth) {
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			ImGui::PushID(label.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();

			ImGui::Columns(1);

			ImGui::PopID();
		}
	}
}