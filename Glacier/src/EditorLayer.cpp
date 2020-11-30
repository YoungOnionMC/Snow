#include "EditorLayer.h"

#include <imgui.h>

#include "Snow/Utils/FileDialogs.h"

namespace Snow {

    void EditorLayer::OnAttach() {

        Render::FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        fbSpec.Attachments = { Render::FramebufferTextureFormat::RGBA16F };
        fbSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

        m_Framebuffer = Render::Framebuffer::Create(fbSpec);
        Render::RenderPassSpecification compRenderPassSpec;
        compRenderPassSpec.TargetFramebuffer = m_Framebuffer;
        m_CompRenderPass = Render::RenderPass::Create(compRenderPassSpec);

        m_ActiveScene = Snow::Ref<Snow::Scene>::Create();


        m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
        m_CameraEntity.AddComponent<CameraComponent>();

        m_Square1 = m_ActiveScene->CreateEntity("Square");
        m_Square1.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.5f, 1.0f});

        m_SceneHierarchyPanel = SceneHierarchyPanel(m_ActiveScene);
        //m_SceneHierarchyPanel.SetScene(m_ActiveScene);
    }

    void EditorLayer::OnDetach() {

    }

    void EditorLayer::OnUpdate() {

        if (Render::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0 && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        Render::Renderer::BeginRenderPass(m_CompRenderPass);
        m_ActiveScene->OnUpdate();
        Render::Renderer::EndRenderPass();


        //Snow::Render::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0, 1.0, 0.0, 1.0 });
        //Snow::Render::Renderer2D::DrawQuad({ -0.2f, -0.8f }, { 0.5f, 0.7f }, { 1.0, 0.0, 0.0, 1.0 });
        //Snow::Render::Renderer2D::PresentBatch();
    }

    void EditorLayer::OnImGuiRender() {

        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Dockspace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                    NewScene();

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    OpenScene();

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                    SaveSceneAs();

                if (ImGui::MenuItem("Exit")) Core::Application::Get().SetRunning(false);

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();

        //ImGui::Begin("Stats");

        //auto stats = Render::Renderer2D::GetStats();
        //ImGui::Text("Renderer2D Stats:");
        //ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        //ImGui::Text("Quads: %d", stats.QuadCount);
        //ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        //ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        //
        //ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::End();

    }

    void EditorLayer::NewScene() {
        m_ActiveScene = Ref<Scene>::Create();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetScene(m_ActiveScene);
    }

    void EditorLayer::OpenScene() {
        std::optional<std::string> filepath = Utils::FileDialogs::OpenFile("Snow Scene (*.snow)\0.snow\0");
        if (filepath) {
            m_ActiveScene = Ref<Scene>::Create();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetScene(m_ActiveScene);
        }
    }

    void EditorLayer::SaveSceneAs() {
        std::optional<std::string> filepath = Utils::FileDialogs::SaveFile("Snow Scene (*.snow)\0.snow\0");
        if (filepath) {
            m_ActiveScene = Ref<Scene>::Create();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetScene(m_ActiveScene);


        }
        SNOW_CORE_TRACE("You did it");
    }
}