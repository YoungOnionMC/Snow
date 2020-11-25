#include <spch.h>
#include "Snow/Render/SwapChain.h"

#include "Snow/Render/RenderContext.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLSwapChain.h"
//#include "Snow/Platform/Vulkan/VulkanSwapChain.h"

namespace Snow {
    namespace Render {
        SwapChain* SwapChain::Create(const SwapChainSpecification& spec) {
            switch(Renderer::GetRenderAPI()) {
            case RenderAPIType::None: return nullptr;
            case RenderAPIType::OpenGL: return new OpenGLSwapChain(spec);
            }

            return nullptr;
        }
    }
}
