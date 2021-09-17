#include <spch.h>
#include "Snow/Render/RenderCommand.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLRenderCommand.h"
//#include "Snow/Platform/Vulkan/VulkanRenderCommand.h"

//#include "Snow/Platform/DirectX11/DirectXRenderCommand.h"

namespace Snow {
    namespace Render {
        Core::Scope<RenderAPI> RenderCommand::s_RenderAPI = nullptr;

        Core::Scope<RenderAPI> RenderAPI::Create() {
            switch (Renderer::GetRenderAPI()) {
            case RenderAPIType::None:   return nullptr;
            case RenderAPIType::OpenGL: return Core::CreateScope<OpenGLRenderCommand>();
            //case RenderAPIType::Vulkan: return Core::CreateScope<VulkanRenderCommand>();
            //case RenderAPIType::DirectX:    return Core::CreateScope<DirectX11RenderCommand>();
            }
        }
    }
}