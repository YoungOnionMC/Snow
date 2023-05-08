#include "RayTracingLayer.h"

using namespace Snow;

void RayTracingLayer::OnAttach() {
	

	Render::TextureProperties textureProps;
	textureProps.SamplerFilter = TextureFilter::Linear;
}

void RayTracingLayer::OnDetach() {
}

static glm::vec4 ConvertUIntToVec4(uint32_t num) {
    uint8_t a = num >> 24 & 0xff;
    uint8_t b = num >> 16 & 0xff;
    uint8_t g = num >>  8 & 0xff;
    uint8_t r = num       & 0xff;
    return glm::vec4(r, g, b, a);
}

static uint32_t ConvertVec4ToUInt(glm::vec4 num) {
    uint8_t r = (uint8_t)(num.r * 255.0f);
    uint8_t g = (uint8_t)(num.g * 255.0f);
    uint8_t b = (uint8_t)(num.b * 255.0f);
    uint8_t a = (uint8_t)(num.a * 255.0f);
    return (a << 24) | (b << 16) | (g << 8) | (r);
}



void RayTracingLayer::OnUpdate(Timestep ts) {
    m_Camera.OnUpdate(ts);
    OnRender(m_Camera);
}

void RayTracingLayer::OnRender(RayCamera& camera) {
    OnResize(m_ViewportSize.x, m_ViewportSize.y);
    camera.OnResize(m_ViewportSize.x, m_ViewportSize.y);

    Math::Ray ray;
    ray.Origin = camera.GetPosition();
    //Buffer imageData = Buffer(m_ViewportSize.x * m_ViewportSize.y * 4);

    for (int y = 0; y < m_OutputImage->GetHeight(); y++) {
        for (int x = 0; x < m_OutputImage->GetWidth(); x++) {
            ray.Direction = camera.getRayDirections()[x + y * m_OutputImage->GetWidth()];
            glm::vec4 color = TraceRay(ray);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            m_ImageData[x + y * m_OutputImage->GetWidth()] = ConvertVec4ToUInt(color);
        }
    }

    m_OutputImage->SetData(m_ImageData.data());
}

glm::vec4 RayTracingLayer::TraceRay(Math::Ray ray) {
    float radius = 0.5f;

    float a = glm::dot(ray.Direction, ray.Direction);

    float b = 2.0f * glm::dot(ray.Origin, ray.Direction);

    float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

    float disc = b * b - 4.0f * a * c;

    if (disc < 0.0f)
        return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    float t0 = (-b + glm::sqrt(disc)) / (2.0f * a);
    float t1 = (-b - glm::sqrt(disc)) / (2.0f * a);

    glm::vec3 hit0 = ray.Origin + ray.Direction * t0;
    glm::vec3 hit1 = ray.Origin + ray.Direction * t1;

    glm::vec3 normalized = glm::normalize(hit1);

    glm::vec3 lightDir = glm::normalize(glm::vec3(1, -1, -1));

    float dot = glm::max(glm::dot(normalized, -lightDir), 0.0f);

    glm::vec3 sphereColor = glm::vec3(0.2f, 0.6f, 0.7f);
    sphereColor *= dot;

    return glm::vec4(sphereColor, 1.0f);
}

void RayTracingLayer::OnResize(uint32_t width, uint32_t height) {
    
    if (m_OutputImage) {
 

        m_OutputImage->Resize(width, height);
        //m_OutputImage->Invalidate();
    }
    else {
        Render::ImageSpecification imageSpec;
        imageSpec.Format = ImageFormat::RGBA;
        imageSpec.Width = width, imageSpec.Height = height;
        m_OutputImage = Render::Image2D::Create(imageSpec);
        m_OutputImage->Invalidate();
    }

    
    m_ImageData.resize(width * height);
    //SNOW_CORE_TRACE(m_ImageData.size());
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