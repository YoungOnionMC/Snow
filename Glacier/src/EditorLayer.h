#pragma once

#include <Snow.h>
#include "Panels/SceneHierarchyPanel.h"

#include <glm/glm.hpp>

#include <Snow/Core/Input.h>

#include "Snow/Core/Event/KeyEvent.h"

namespace Snow {

    class EditorLayer : public Core::Layer {
    public:
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;

        void OnImGuiRender();

        void OnEvent(Core::Event::Event& e) override;

    private:
        bool OnKeyPressed(Core::Event::KeyPressedEvent& e);

        void NewScene();
        void OpenScene();
        void SaveSceneAs();

        Ref<Snow::Scene> m_ActiveScene;

        Render::EditorCamera m_EditorCamera;
        Entity m_CameraEntity;
        Entity m_Square1;

        Ref<Render::Framebuffer> m_Framebuffer;
        Ref<Render::RenderPass> m_CompRenderPass;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

        bool m_Running = false;

        SceneHierarchyPanel m_SceneHierarchyPanel;

        static int m_ImGuizmoSelection;
    };
}