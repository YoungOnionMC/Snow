#pragma once

#include <Snow.h>
#include "Snow/Editor/SceneHierarchyPanel.h"
#include "Snow/Editor/ContentBrowserPanel.h"

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

        void CreateProject(std::filesystem::path projectPath);
        void OpenProject();
        void OpenProject(const std::string& filepath);
        void SaveProject();
        void CloseProject(bool unloadProject = true);

        void NewScene();
        void OpenScene();
        void OpenScene(const std::string& filepath);
        void OpenScene(const AssetMetadata& metadata);
        void SaveScene();
        void SaveSceneAs();

        void OnRender();

    private:
        bool OnKeyPressed(Core::Event::KeyPressedEvent& e);

        bool m_ShowCreateProjectPopup = false;

        Ref<Snow::Scene> m_RuntimeScene, m_EditorScene;
        Ref<Render::SceneRenderer> m_SceneRenderer;

        Ref<Render::Renderer2D> m_Renderer2D;

        std::string m_SceneFilePath;

        enum class SceneState {
            Editor = 0, Play = 1, Pause = 2
        };
        SceneState m_SceneState = SceneState::Editor;

        Editor::EditorCamera m_EditorCamera;
        Entity m_CameraEntity;
        Entity m_Square1;


        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        bool m_ReloadScriptOnPlay = false;

        bool m_Running = false;

        Ref<Render::Texture2D> m_PlayButtonTex;

        SceneHierarchyPanel m_SceneHierarchyPanel;
        Scope<ContentBrowserPanel> m_ContentBrowserPanel;

        static int m_ImGuizmoSelection;
    };
}