#include <spch.h>
#include "Snow/ImGui/ImGuiLayer.h"

#include "Snow/Render/Renderer.h"

//#include "Snow/Platform/OpenGL/OpenGLImGuiLayer.h"
#include "Snow/Platform/Vulkan/VulkanImGuiLayer.h"

#if defined(SNOW_PLATFORM_WINDOWS)
//#include "Snow/Platform/DirectX11/DirectXImGuiLayer.h"
#endif

namespace Snow {
    ImGuiLayer* ImGuiLayer::Create() {
        switch(Render::RendererAPI::Current()) {
        //case Render::RendererAPIType::OpenGL: return new OpenGLImGuiLayer();
        case Render::RendererAPIType::Vulkan: return new VulkanImGuiLayer();
#if defined(SNOW_PLATFORM_WINDOWS)
        //case Render::RendererAPIType::DirectX:    return new DirectX11ImGuiLayer();
#endif
        }
    }
}