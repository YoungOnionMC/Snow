#include "RayTracingLayer.h"

using namespace Snow;

void RayTracingLayer::OnAttach() {
	

	Render::TextureProperties textureProps;
	textureProps.SamplerFilter = TextureFilter::Linear;
}

void RayTracingLayer::OnDetach() {
}

void RayTracingLayer::OnUpdate(Timestep ts) {
    OnResize(m_ViewportSize.x, m_ViewportSize.y);

    Buffer imageData = Buffer(m_ViewportSize.x * m_ViewportSize.y * 4);

    for (int ix = 0; ix < m_ViewportSize.x; ix++) {
        for (int y = 0; y < m_ViewportSize.y; y++) {
            int position = ix + y * m_ViewportSize.x;
            m_ImageData[position] = 0xFFFF00FF;
        }
    }

    m_OutputImage->SetData(m_ImageData);
}

void RayTracingLayer::OnResize(uint32_t width, uint32_t height) {
    
    if (m_OutputImage) {
        //m_OutputImage->Invalidate();
    }
    else {
        Render::ImageSpecification imageSpec;
        imageSpec.Format = ImageFormat::RGBA;
        imageSpec.Width = width, imageSpec.Height = height;
        m_OutputImage = Render::Image2D::Create(imageSpec);
        m_OutputImage->Invalidate();
    }

    m_ImageData = new uint32_t[width * height];
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
        
        UI::Image(m_OutputImage, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


        ImGui::End();
    }
    ImGui::PopStyleVar();
    ImGui::End();
}