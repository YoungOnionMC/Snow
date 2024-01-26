#include "RayTracingLayer.h"

using namespace Snow;

void RayTracingLayer::OnAttach() {
    //m_Camera.OnUpdate(ts);
    m_Camera.OnResize(m_ViewportSize.x, m_ViewportSize.y);
    m_Renderer.OnRender(m_Scene, m_Camera);
    //m_Renderer.OnResize(m_ViewportSize.x, m_ViewportSize.y);
	////Render::TextureProperties textureProps;
	//textureProps.SamplerFilter = TextureFilter::Linear;
}

void RayTracingLayer::OnDetach() {
}

void RayTracingLayer::OnUpdate(Timestep ts) {
    m_Camera.OnUpdate(ts);
    m_Renderer.OnRender(m_Scene, m_Camera);
}

void RayTracingLayer::OnResize(uint32_t width, uint32_t height) {
    m_Camera.OnResize(width, height);
    m_Renderer.OnResize(width, height);
}

void RayTracingLayer::OnEvent(Core::Event::Event& e) {
}

void RayTracingLayer::OnImGuiRender() {
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

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

    {
        ImGui::Begin("Viewport");

       // m_ViewportFocused = ImGui::IsWindowFocused();
        //m_ViewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        OnResize(m_ViewportSize.x, m_ViewportSize.y);
        auto image = m_Renderer.GetOutputImage();
        //if(image)
        UI::Image(image, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


        ImGui::End();
    }
    ImGui::PopStyleVar();
    ImGui::End();

    ImGui::Begin("Scene");
    ImGui::DragInt("Bounces", &m_Renderer.GetBounces(), 1.0f, 1, 10);
    for (size_t i = 0; i < m_Scene.Spheres.size(); i++) {
        ImGui::PushID(i);

        RayRenderer::Sphere& sphere = m_Scene.Spheres[i];

        ImGui::DragFloat3("Position", glm::value_ptr(sphere.Pos), 0.1f);
        ImGui::DragFloat("Radius", &sphere.radius, 0.1f);
        ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.Color));
        ImGui::Separator();

        ImGui::PopID();
    }
    ImGui::End();
}