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

        void OnScenePlay();
        void OnSceneStop();

        void OnImGuiRender();

        void OnEvent(Core::Event::Event& e) override;

    private:
        bool OnKeyPressed(Core::Event::KeyPressedEvent& e);

        void NewScene();
        void OpenScene();
        void SaveSceneAs();

        Ref<Snow::Scene> m_RuntimeScene, m_EditorScene;

        enum class SceneState {
            Editor = 0, Play = 1, Pause = 2
        };
        SceneState m_SceneState = SceneState::Editor;

        Render::EditorCamera m_EditorCamera;
        Entity m_CameraEntity;
        Entity m_Square1;


        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        bool m_ReloadScriptOnPlay = false;

        bool m_Running = false;

        Ref<Render::API::Texture2D> m_PlayButtonTex;

        SceneHierarchyPanel m_SceneHierarchyPanel;

        static int m_ImGuizmoSelection;
    };
}