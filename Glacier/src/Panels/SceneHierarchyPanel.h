#pragma once

#include <Snow/Scene/Scene.h>

namespace Snow {
    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene::Scene>& scene);

        void SetScene(const Ref<Scene::Scene>& scene);

        void OnImGuiRender();

    private:
        void DrawComponents();

        Ref<Scene::Scene> m_SceneContext;
        Entity m_SelectionContext;

    };
}