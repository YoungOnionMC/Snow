#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

namespace Snow {
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene::Scene>& scene) {
        SetScene(scene);
    }

    void SceneHierarchyPanel::SetScene(const Ref<Scene::Scene>& scene) {
        m_SceneContext = scene;
    }

    void SceneHierarchyPanel::OnImGuiRender() {
        ImGui::Begin("Scene Hierarchy");

        if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        ImGui::End();

        ImGui::Begin("Properties");
        if(m_SelectionContext)
            DrawComponents(m_SelectionContext);

        ImGui::End();
    }
}