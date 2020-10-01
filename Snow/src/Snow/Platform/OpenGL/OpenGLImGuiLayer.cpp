#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLImGuiLayer.h"

#include "Snow/Core/Application.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_opengl3.cpp>

#if defined(SNOW_WINDOW_GLFW)
    #include <GLFW/glfw3.h>
    #include <examples/imgui_impl_glfw.h>
    #include <examples/imgui_impl_glfw.cpp>
#elif defined(SNOW_WINDOW_WIN32)
    #include <windows.h>
    #include <examples/imgui_impl_win32.h>
    #include <examples/imgui_impl_win32.cpp>
#endif

namespace Snow {
    OpenGLImGuiLayer::OpenGLImGuiLayer() {
        m_Name = "OpenGLImGuiLayer";
    }

    void OpenGLImGuiLayer::OnAttach() {
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
        ImGui_ImplWin32_Init((HWND)window->GetWindowHandle());
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetWindowHandle(), true);
#endif
        ImGui_ImplOpenGL3_Init("#version 430");
    }

    void OpenGLImGuiLayer::OnDetach() {
#if defined(SNOW_WINDOW_WIN32)
        ImGui_ImplWin32_Shutdown();
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_Shutdown();
#endif

        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }

    void OpenGLImGuiLayer::BeginImGuiFrame() {
        ImGui_ImplOpenGL3_NewFrame();
#if defined(SNOW_WINDOW_WIN32)
        ImGui_ImplWin32_NewFrame();
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_NewFrame();
#endif
        ImGui::NewFrame();
    }

    void OpenGLImGuiLayer::EndImGuiFrame() {
        ImGuiIO& io = ImGui::GetIO();

        Ref<Core::Window> window = Core::Application::Get().GetWindow();
        io.DisplaySize = ImVec2(window->GetWidth(), window->GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
#if defined(SNOW_WINDOW_GLFW)
            GLFWwindow* currentContext = glfwGetCurrentContext();
#endif
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
#if defined(SNOW_WINDOW_GLFW)
            glfwMakeContextCurrent(currentContext);   
#endif
        }
    }
}