#pragma once

#include <Snow/Core/Ref.h>
#include <Snow/Scene/Scene.h>
#include <Snow/Scene/Entity.h>

namespace Snow {
    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        void SetScene(const Ref<Scene>& scene);

        void OnImGuiRender();

    private:
        void DrawComponents(const Entity& entity);

        Ref<Scene> m_SceneContext;
        Entity m_SelectionContext = {};

    };
}