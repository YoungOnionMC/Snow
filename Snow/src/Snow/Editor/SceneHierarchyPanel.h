#pragma once

#include <Snow/Core/Ref.h>
#include <Snow/Scene/Scene.h>
#include <Snow/Scene/Entity.h>

#include <Snow/Render/Texture.h>

namespace Snow {
    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        void SetScene(const Ref<Scene>& scene);

        void OnImGuiRender(bool window = true);

        Entity GetSelectedEntity() { return m_SelectionContext; }

    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

        void DrawEnvironment();

        //void DrawIconedImage(const std::string& label, Ref<Render::Texture2D>& texture);

        Ref<Scene> m_SceneContext;
        Entity m_SelectionContext = {};

        Ref<Render::Texture2D> m_CheckerboardTexture;

        
    };
}