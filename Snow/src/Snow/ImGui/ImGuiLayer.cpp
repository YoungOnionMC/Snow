#include <spch.h>
#include "Snow/ImGui/ImGuiLayer.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLImGuiLayer.h"
#include "Snow/Platform/Vulkan/VulkanImGuiLayer.h"

namespace Snow {
    ImGuiLayer* ImGuiLayer::Create() {
        switch(Render::Renderer::GetRenderAPI()) {
        case Render::RenderAPI::OpenGL: return new OpenGLImGuiLayer();
        case Render::RenderAPI::Vulkan: return new VulkanImGuiLayer();
        }
    }
}