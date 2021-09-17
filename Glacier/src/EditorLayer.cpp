#include "EditorLayer.h"


#include "Snow/Utils/FileDialogs.h"
#include "Snow/Scene/SceneSerializer.h"
#include "Snow/Render/SceneRenderer.h"
#include "Snow/Script/ScriptEngine.h"

#include "UI/ImGuiUI.h" 

#include "Snow/Math/Mat4.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

namespace Snow {

    int EditorLayer::m_ImGuizmoSelection = -1;

    void EditorLayer::OnAttach() {
        m_EditorScene = Ref<Scene>::Create();
        m_SceneRenderer = Ref<Render::SceneRenderer>::Create(m_EditorScene, Render::SceneRendererSpecification{ false });

        m_EditorCamera = Editor::EditorCamera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 1000.0f));
        //m_EditorCamera = *(Editor::EditorCamera*)&Render::Camera(glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f));
        m_PlayButtonTex = Render::Texture2D::Create("assets/textures/Windows.png");

        //m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
        //m_CameraEntity.AddComponent<CameraComponent>();

        m_Square1 = m_EditorScene->CreateEntity("Square");

        m_SceneHierarchyPanel = SceneHierarchyPanel(m_EditorScene);
        Script::ScriptEngine::SetSceneContext(m_EditorScene);
        //m_SceneHierarchyPanel.SetScene(m_ActiveScene);

        ImGuizmo::SetOrthographic(false);

#if 0
        class PlayerController : public ScriptableEntity {
        public:
            virtual void OnCreate() override {

            }

            virtual void OnDestroy() override {}

            virtual void OnUpdate(Timestep ts) override {
                auto RigidBody = GetComponent<RigidBody2DComponent>().RigidBody;

                
                if (Core::Input::IsKeyPressed(Key::Space))
                    RigidBody->ApplyForceToCenter({ 0.0, 10.0f }, true);
                if (Core::Input::IsKeyPressed(Key::D))
                    RigidBody->ApplyForceToCenter({ 1.0f, 0.0f }, true);
                else if(Core::Input::IsKeyPressed(Key::A))
                    RigidBody->ApplyForceToCenter({ -1.0f, 0.0f }, true);
            }
        };

        m_Square1.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
#endif
    }

    void EditorLayer::OnDetach() {

    }

    void EditorLayer::OnScenePlay() {

        m_SceneState = SceneState::Play;
        if (m_ReloadScriptOnPlay)
            Script::ScriptEngine::ReloadAssembly("assets/scripts/ExampleApp.dll");

        m_RuntimeScene = Ref<Scene>::Create();
        m_EditorScene->CopyScene(m_RuntimeScene);

        m_RuntimeScene->OnRuntimeStart();
        m_SceneHierarchyPanel.SetScene(m_RuntimeScene);
    }

    void EditorLayer::OnSceneStop() {
        m_RuntimeScene->OnRuntimeStop();
        m_SceneState = SceneState::Editor;

        m_RuntimeScene = nullptr;

        
        Script::ScriptEngine::SetSceneContext(m_EditorScene);
        m_SceneHierarchyPanel.SetScene(m_EditorScene);
    }

    void EditorLayer::OnUpdate(Timestep ts) {
        switch (m_SceneState) {
        case SceneState::Editor: {
            /*
            if (Render::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
                m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0 && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
                m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
                m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
                m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            }
            */

            //if (m_ViewportFocused)
                m_EditorCamera.OnUpdate(ts);

            m_EditorScene->OnUpdate(ts);
            m_EditorScene->OnRenderEditor(m_SceneRenderer, ts, m_EditorCamera);

            //m_Framebuffer->Unbind();

            break;
        }
        case SceneState::Play: {
            if (m_ViewportFocused)
                m_EditorCamera.OnUpdate(ts);

            m_RuntimeScene->OnUpdate(ts);
            m_RuntimeScene->OnRenderRuntime(m_SceneRenderer, ts);
            break;
        }
        case SceneState::Pause: {
            if (m_ViewportFocused)
                m_EditorCamera.OnUpdate(ts);

            m_RuntimeScene->OnRenderRuntime(m_SceneRenderer, ts);
            break;
        }
        }

        
    }

    void EditorLayer::OnImGuiRender() {
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
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
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
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

            if (ImGui::BeginMenu("Scripting")) {
                if (ImGui::MenuItem("Reload C# Assembly"))
                    Script::ScriptEngine::ReloadAssembly("assets/scripts/ExampleScriptApp.dll");

                ImGui::MenuItem("reload assembly on play", nullptr, &m_ReloadScriptOnPlay);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();

        ImGui::Begin("Shaders");
        if (ImGui::TreeNode("Shaders")) {
            auto& shaders = Render::Renderer::GetShaderLibrary()->Get();
            for (auto& [name, shader] : shaders) {
                if (ImGui::TreeNode(shader->GetName().c_str())) {
                    std::string buttonName = "Reload##" + shader->GetName();
                    if (ImGui::Button(buttonName.c_str()))
                        shader->Reload();
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
        ImGui::Begin("Toolbar");
        if (m_SceneState == SceneState::Editor) {
            UI::BeginPropertyGrid();
            //if (ImGui::ImageButton((ImTextureID)(m_PlayButtonTex->GetImage()->GetRendererID()), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(0.9f, 0.9f, 0.9f, 1.0f))) {
            //    OnScenePlay();
            //}
            ImGui::NextColumn();
            ImGui::Text("Play Scene");
            UI::EndPropertyGrid();
        }
        else if (m_SceneState == SceneState::Play) {
            UI::BeginPropertyGrid();
            //if (ImGui::ImageButton((ImTextureID)(m_PlayButtonTex->GetRendererID()), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(0.9f, 0.9f, 0.9f, 1.0f))) {
            //    OnSceneStop();
            //}
            ImGui::NextColumn();
            ImGui::Text("Switch to Editor Scene");
            UI::EndPropertyGrid();
        }

        ImGui::SameLine();

        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        

        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        //Render::SceneRenderer::OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        if (m_RuntimeScene)
            m_RuntimeScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_EditorCamera.SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), (float)m_ViewportSize.x, (float)m_ViewportSize.y, 0.1f, 1000.0f));
        m_EditorCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

        Ref<Snow::Render::Image2D> sceneImage = m_SceneRenderer->GetFinalPassImage();
        UI::Image(sceneImage, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        //ImGui::Image(reinterpret_cast<void*>(imageID->), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


        // Gizmos
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_ImGuizmoSelection != -1 && (m_ViewportFocused) && !m_Running && (m_SceneState == SceneState::Editor)) {
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

        ImGui::End();
        ImGui::PopStyleVar();

        m_SceneRenderer->OnImGuiRender();

        Script::ScriptEngine::OnImGuiRender();
        ImGui::End();
    }

    void EditorLayer::NewScene() {

        m_EditorScene = Ref<Scene>::Create();
        m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        
        m_SceneHierarchyPanel.SetScene(m_EditorScene);
        Script::ScriptEngine::SetSceneContext(m_EditorScene);
    }

    void EditorLayer::OpenScene() {
        std::optional<std::string> filepath = Utils::FileDialogs::OpenFile("Snow Scene (*.snow)\0*.snow\0");
        if (filepath) {
            m_EditorScene = Ref<Scene>::Create();
            //m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorScene->OnViewportResize((uint32_t)1280, (uint32_t)720);
            m_SceneHierarchyPanel.SetScene(m_EditorScene);
            Script::ScriptEngine::SetSceneContext(m_EditorScene);
            //Script::ScriptEngine::LoadSnowRuntimeAssembly("assets/scripts/ExamplescriptApp.dll");


            SceneSerializer serializer(m_EditorScene);
            serializer.DeserializeText(*filepath);
        }
    }

    void EditorLayer::SaveSceneAs() {
        std::optional<std::string> filepath = Utils::FileDialogs::SaveFile("Snow Scene (*.snow)\0*.snow\0");
        if (filepath) {
            SceneSerializer serializer(m_EditorScene);
            serializer.SerializeText(*filepath);
        }
    }

    void EditorLayer::OnEvent(Core::Event::Event& e) {

        Core::Event::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Core::Event::KeyPressedEvent>(SNOW_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnKeyPressed(Core::Event::KeyPressedEvent& e) {
        switch (e.GetKeyCode()) {
        case KeyCode::LeftBracket: {
            m_Running = true;
            break;
        }
        case KeyCode::RightBracket: {
            m_Running = false;
            break;
        }
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