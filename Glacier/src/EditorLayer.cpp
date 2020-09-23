#include "EditorLayer.h"

#include <imgui.h>

void EditorLayer::OnAttach() {

}

void EditorLayer::OnDetach() {

}

void EditorLayer::OnUpdate() {

}

void EditorLayer::OnImGuiRender() {

    ImGui::Begin("Test stuff");
    ImGui::Text("Client side code");
    ImGui::End();

}