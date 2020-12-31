#include <spch.h>
#include "Snow/ImGui/ImGuiLayer.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLImGuiLayer.h"
//#include "Snow/Platform/Vulkan/VulkanImGuiLayer.h"

#if defined(SNOW_PLATFORM_WINDOWS)
#include "Snow/Platform/DirectX11/DirectXImGuiLayer.h"
#endif



#if defined(SNOW_WINDOW_GLFW)


#include <examples/imgui_impl_glfw.cpp>

#elif defined(SNOW_WINDOW_WIN32)
#include <windows.h>
#include <examples/imgui_impl_win32.cpp>
#endif

namespace Snow {
    ImGuiLayer* ImGuiLayer::Create() {
        switch(Render::Renderer::GetRenderAPI()) {
        case Render::RenderAPIType::OpenGL: return new OpenGLImGuiLayer();
        //case Render::RenderAPIType::Vulkan: return new VulkanImGuiLayer();
#if defined(SNOW_PLATFORM_WINDOWS)
        case Render::RenderAPIType::DirectX:    return new DirectX11ImGuiLayer();
#endif
        }
    }
}