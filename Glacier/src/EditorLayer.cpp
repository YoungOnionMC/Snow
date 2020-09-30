#include "EditorLayer.h"

#include <imgui.h>

void EditorLayer::OnAttach() {

}

void EditorLayer::OnDetach() {

}

void EditorLayer::OnUpdate() {
    Snow::Render::Renderer2D::SubmitQuad({0.0f, 0.0f}, {1.0f, 1.0f});
    Snow::Render::Renderer2D::PresentBatch();
}

void EditorLayer::OnImGuiRender() {

    ImGui::Begin("Test stuff");
    ImGui::Text("Client side code");
    ImGui::End();

}