#include "EditorLayer.h"


#include "Snow/Utils/FileDialogs.h"
#include "Snow/Scene/SceneSerializer.h"
#include "Snow/Render/SceneRenderer.h"

#include "Snow/Math/Mat4.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

namespace Snow {

    int EditorLayer::m_ImGuizmoSelection = -1;

    void EditorLayer::OnAttach() {

        Render::FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        fbSpec.AttachmentList = { Render::FramebufferTextureFormat::RGBA8 };
        fbSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
        fbSpec.SwapChainTarget = true;

        m_Framebuffer = Render::Framebuffer::Create(fbSpec);
        Render::RenderPassSpecification compRenderPassSpec;
        compRenderPassSpec.TargetFramebuffer = m_Framebuffer;
        m_CompRenderPass = Render::RenderPass::Create(compRenderPassSpec);

        m_ActiveScene = Snow::Ref<Snow::Scene>::Create();

        m_EditorCamera = Render::EditorCamera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);


        //m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
        //m_CameraEntity.AddComponent<CameraComponent>();

        m_Square1 = m_ActiveScene->CreateEntity("Square");
        m_Square1.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.5f, 1.0f});

        m_SceneHierarchyPanel = SceneHierarchyPanel(m_ActiveScene);
        //m_SceneHierarchyPanel.SetScene(m_ActiveScene);

        ImGuizmo::SetOrthographic(false);
    }

    void EditorLayer::OnDetach() {

    }

    void EditorLayer::OnUpdate() {

        if (Render::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0 && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        
        m_ActiveScene->OnUpdateEditor(m_EditorCamera);
        if(m_ViewportFocused)
            m_EditorCamera.OnUpdate();
        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender() {
        m_Framebuffer->Bind();
#if 1
        static bool dockspaceOpen = false;
        static bool opt_fullscreen_persistant = false;
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
        //ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
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

                if (ImGui::MenuItem("Exit")) Core::Application::Get().Close();

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

        void* textureID = Render::SceneRenderer::GetFinalColorAttachment();
        ImGui::Image((textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


        // Gizmos
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_ImGuizmoSelection != -1) {
            ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            const glm::mat4& cameraProj = m_EditorCamera.GetProjection();
            glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

            auto& transformComp = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = transformComp.GetTransform();

            bool snap = Core::Input::IsKeyPressed(Key::LeftAlt);
            float snapValue = 0.5f;
            if (m_ImGuizmoSelection == ImGuizmo::OPERATION::ROTATE)
                snapValue = 22.5f;

            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj), 
                (ImGuizmo::OPERATION)m_ImGuizmoSelection, ImGuizmo::LOCAL, 
                glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);
            
            if (ImGuizmo::IsUsing()) {
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRot = rotation - transformComp.Rotation;
                transformComp.Translation = translation;
                transformComp.Rotation += deltaRot;
                transformComp.Scale = scale;
            }
        }

        //ImGui::End();
        ImGui::PopStyleVar();
        ImGui::End();
#endif

        //ImGui::ShowDemoWindow();
        m_Framebuffer->Unbind();
    }

    void EditorLayer::NewScene() {
        m_ActiveScene = Ref<Scene>::Create();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        
        m_SceneHierarchyPanel.SetScene(m_ActiveScene);
    }

    void EditorLayer::OpenScene() {
        std::optional<std::string> filepath = Utils::FileDialogs::OpenFile("Snow Scene (*.snow)\0*.snow\0");
        if (filepath) {
            m_ActiveScene = Ref<Scene>::Create();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetScene(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);
            serializer.DeserializeText(*filepath);
        }
    }

    void EditorLayer::SaveSceneAs() {
        std::optional<std::string> filepath = Utils::FileDialogs::SaveFile("Snow Scene (*.snow)\0*.snow\0");
        if (filepath) {
            SceneSerializer serializer(m_ActiveScene);
            serializer.SerializeText(*filepath);
        }
    }

    void EditorLayer::OnEvent(Core::Event::Event& e) {

        Core::Event::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Core::Event::KeyPressedEvent>(SNOW_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnKeyPressed(Core::Event::KeyPressedEvent& e) {
        switch (e.GetKeyCode()) {
        case KeyCode::N: {
            if(Core::Input::IsKeyPressed(KeyCode::LeftControl))
                NewScene();
            break;
        }
        case KeyCode::S: {
            if (Core::Input::IsKeyPressed(KeyCode::LeftControl))
                SaveSceneAs();
            break;
        }
        case KeyCode::O: {
            if (Core::Input::IsKeyPressed(KeyCode::LeftControl))
                OpenScene();
            break;
        }

        case KeyCode::Q: {
            m_ImGuizmoSelection = -1;
            break;
        }
        case KeyCode::W: {
            m_ImGuizmoSelection = ImGuizmo::OPERATION::TRANSLATE;
            break;
        }
        case KeyCode::E: {
            m_ImGuizmoSelection = ImGuizmo::OPERATION::ROTATE;
            break;
        }
        case KeyCode::R: {
            m_ImGuizmoSelection = ImGuizmo::OPERATION::SCALE;
            break;
        }
        }

        return false;
    }
}