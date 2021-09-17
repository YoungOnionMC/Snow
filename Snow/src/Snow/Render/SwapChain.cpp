#include <spch.h>
#include "Snow/Render/SwapChain.h"

#include "Snow/Render/RenderContext.h"

#include "Snow/Render/Renderer.h"

//#include "Snow/Platform/OpenGL/OpenGLSwapChain.h"
#include "Snow/Platform/Vulkan/VulkanSwapChain.h"

namespace Snow {
    namespace Render {
        SwapChain* SwapChain::CreateSwapChain(const SwapChainSpecification& spec) {
            switch(Render::RendererAPI::Current()) {
            case RendererAPIType::None: return nullptr;
            //case RendererAPIType::OpenGL: return new OpenGLSwapChain(spec);
            case RendererAPIType::Vulkan: return new VulkanSwapChain();
            }

            return nullptr;
        }
    }
}
