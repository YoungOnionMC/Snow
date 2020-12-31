#pragma once

#include <Snow/Core/Ref.h>
#include <Snow/Scene/Scene.h>
#include <Snow/Scene/Entity.h>

#include <Snow/Render/API/Texture.h>

namespace Snow {
    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        void SetScene(const Ref<Scene>& scene);

        void OnImGuiRender();

        Entity GetSelectedEntity() { return m_SelectionContext; }

    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

        void DrawEnvironment();

        void DrawIconedImage(const std::string& label, Ref<Render::API::Texture2D>& texture);

        Ref<Scene> m_SceneContext;
        Entity m_SelectionContext = {};

        Ref<Render::API::Texture2D> m_CheckerboardTexture;

        
    };
}