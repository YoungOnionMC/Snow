#include <spch.h>
#include "Snow/Render/RenderCommand.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLRenderCommand.h"
#include "Snow/Platform/Vulkan/VulkanRenderCommand.h"

namespace Snow {
    namespace Render {
        RenderCommand* RenderCommand::Create() {
            switch(Renderer::GetRenderAPI()){
                case RenderAPI::None:   return nullptr;
                case RenderAPI::OpenGL: return new OpenGLRenderCommand();
                case RenderAPI::Vulkan: return new VulkanRenderCommand();
            }
            
            return nullptr;
        }
    }
}