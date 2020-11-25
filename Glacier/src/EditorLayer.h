#pragma once

#include <Snow.h>
#include "Panels/SceneHierarchyPanel.h"

namespace Snow {
    class EditorLayer : public Core::Layer {
    public:
        void OnAttach();
        void OnDetach();
        void OnUpdate();

        void OnImGuiRender();


    private:

        Ref<Snow::Scene> m_ActiveScene;
        Entity m_CameraEntity;
        Entity m_Square1;

        Ref<Render::Framebuffer> m_Framebuffer;
        Ref<Render::RenderPass> m_CompRenderPass;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        Math::Vector2f m_ViewportSize = { 0.0f, 0.0f };

        SceneHierarchyPanel m_SceneHierarchyPanel;

    };
}