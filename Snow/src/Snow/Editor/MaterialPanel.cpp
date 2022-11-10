#include <spch.h>

#include "Snow/Editor/MaterialPanel.h"

#include "Snow/ImGui/ImGui.h"

namespace Snow {
	MaterialPanel::MaterialPanel(Ref<Render::Material>& material) :
		m_Material(material) {

	}

	void MaterialPanel::OnImGuiRender() {
		ImGui::Begin("Material Editor");

		//ImGuiPopupFlags_MouseButtonRight
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::BeginMenu("Texture Node")) {
				auto textureNode = m_TextureNodes.emplace_back();
				
				
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}