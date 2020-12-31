#include <spch.h>
#include "Snow/Platform/DirectX11/DirectXImGuiLayer.h"

#include <imgui.h>

#include <examples/imgui_impl_dx11.h>
#include <examples/imgui_impl_dx11.cpp>

#include "Snow/Core/Application.h"

#include "Snow/Platform/DirectX11/DirectXContext.h"

#include <ImGuizmo.h>

namespace Snow {
	DirectX11ImGuiLayer::DirectX11ImGuiLayer() {
		m_Name = "DirectXImGuiLayer";
	}
#if defined(SNOW_WINDOW_WIN32)
    LRESULT CALLBACK WindowProc(HWND wnd, UINT mess, WPARAM wParam, LPARAM lParam) {
        return ImGui_ImplWin32_WndProcHandler(wnd, mess, wParam, lParam);
    }
#endif

	void DirectX11ImGuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsClassic();
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        Ref<Core::Window> window = Core::Application::Get().GetWindow();

#if defined(SNOW_WINDOW_WIN32)
        ImGui_ImplWin32_Init((HWND)window->GetWindowHandle());
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetWindowHandle(), true);
#endif
        //ImGui_ImplWin32_Init(glfwGetWin32Window((GLFWwindow*)window->GetWindowHandle()));
        

        auto dxDevice = DirectX11RenderContext::Get()->GetDevice();
        ImGui_ImplDX11_Init(dxDevice->GetDevice().Get(), dxDevice->GetDeviceContext());
        
	}

    void DirectX11ImGuiLayer::OnDetach() {
#if defined(SNOW_WINDOW_WIN32)
        ImGui_ImplWin32_Shutdown();
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_Shutdown();
#endif
        

        ImGui_ImplDX11_Shutdown();
        ImGui::DestroyContext();
    }

    void DirectX11ImGuiLayer::BeginImGuiFrame() {
        ImGui_ImplDX11_NewFrame();
#if defined(SNOW_WINDOW_WIN32)
        ImGui_ImplWin32_NewFrame();
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_NewFrame();
#endif

        ImGui::NewFrame();

        ImGuizmo::BeginFrame();
    }

    void DirectX11ImGuiLayer::EndImGuiFrame() {
        ImGuiIO& io = ImGui::GetIO();

        Ref<Core::Window> window = Core::Application::Get().GetWindow();
        io.DisplaySize = ImVec2(window->GetWidth(), window->GetHeight());

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
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