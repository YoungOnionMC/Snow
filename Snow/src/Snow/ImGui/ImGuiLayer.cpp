#include <spch.h>
#include "Snow/ImGui/ImGuiLayer.h"

#define SNOW_WINDOW_GLFW

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#if defined(SNOW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#include <examples/imgui_impl_glfw.h>
#endif

//#include <examples/imgui_impl_vulkan.h>
//#include <examples/imgui_impl_vulkan.cpp>
//#include "Snow/Platform/Vulkan/VulkanContext.h"

#include "Snow/Core/Application.h"

#include "Snow/Render/Renderer.h"

namespace Snow {

    ImGuiLayer::ImGuiLayer() :
        Layer("ImGuiLayer") {

    }

    ImGuiLayer::~ImGuiLayer() {
        
    }

    void ImGuiLayer::OnAttach() {
        SNOW_CORE_TRACE("Attaching Layer {0}", m_Name);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsClassic();
        ImGuiStyle& style = ImGui::GetStyle();
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        Ref<Core::Window> window = Core::Application::Get().GetWindow();

#if defined(SNOW_WINDOW_WIN32)

#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetWindowHandle(), true);
#endif

        SNOW_CORE_ERROR("{0}", Render::Renderer::GetRenderAPI());

        switch(Render::Renderer::GetRenderAPI()) {
        case Render::RenderAPI::OpenGL: {
            SNOW_CORE_TRACE("Using OpenGL with ImGui");
            ImGui_ImplOpenGL3_Init("#version 430");
            break;
        }
        case Render::RenderAPI::Vulkan: {
            //ImGui_ImplVulkan_InitInfo vulkanInitInfo = {};
            //Render::VulkanContext* vkContext = static_cast<Render::VulkanContext*>(Render::Renderer::GetContext());
            //Ref<Render::VulkanDevice> vkDevice = vkContext->GetDevice();
            //Render::VulkanSwapChain vkSwapChain = vkContext->GetSwapChain();
            //vulkanInitInfo.Instance = vkContext->GetVulkanInstance();
            //vulkanInitInfo.PhysicalDevice = vkDevice->GetPhysicalDevice();
            //vulkanInitInfo.Device = vkDevice->GetDevice();
            //vulkanInitInfo.MinImageCount = vkSwapChain.GetMinimumImageCount();
            //vulkanInitInfo.ImageCount = vkSwapChain.GetImageCount();
            //vulkanInitInfo.CheckVkResultFn = VulkanErrorCheck;
            //ImGui_ImplVulkan_Init(&vulkanInitInfo, vkSwapChain.GetRenderPass());
        }
        }
    }

    void ImGuiLayer::OnDetach() {
#if defined(SNOW_WINDOW_WIN32)
        ImGui_ImplWin32_Shutdown();
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_Shutdown();
#endif

        switch(Render::Renderer::GetRenderAPI()) {
        case Render::RenderAPI::OpenGL: ImGui_ImplOpenGL3_Shutdown(); break;
        }
        ImGui::DestroyContext();
        
    }

    void ImGuiLayer::Begin() {
        switch(Render::Renderer::GetRenderAPI()) {
        case Render::RenderAPI::OpenGL: ImGui_ImplOpenGL3_NewFrame(); break;

        }
#if defined(SNOW_WINDOW_WIN32)
        ImGui_ImplWin32_NewFrame();
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_NewFrame();
#endif
        ImGui::NewFrame();
    }

    void ImGuiLayer::End() {
        ImGuiIO& io = ImGui::GetIO();
        //Core::Window window
        Ref<Core::Window> window = Core::Application::Get().GetWindow();
        io.DisplaySize = ImVec2(window->GetWidth(), window->GetHeight());

        ImGui::Render();
        switch(Render::Renderer::GetRenderAPI()) {
        case Render::RenderAPI::OpenGL: ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); break;
        }

        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
#if defined(SNOW_WINDOW_GLFW)
            GLFWwindow* currentContext = glfwGetCurrentContext();
#endif

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

#if defined(SNOW_WINDOW_GLFW)
            switch(Render::Renderer::GetRenderAPI()) {
                case Render::RenderAPI::OpenGL: {
                    glfwMakeContextCurrent(currentContext);
                }
            }

#elif defined(SNOW_WINDOW_WIN32)

#endif
        }
    }

    void ImGuiLayer::OnUpdate() {
        
    }

}