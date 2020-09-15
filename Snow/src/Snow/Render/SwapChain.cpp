#include <spch.h>
#include "Snow/Render/SwapChain.h"

#include "Snow/Render/RenderContext.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLSwapChain.h"

namespace Snow {
    namespace Render {
        SwapChain* SwapChain::Create(const SwapChainSpecification& spec) {
        switch(Renderer::GetRenderAPI()) {
        case RenderAPI::None: return nullptr;
        case RenderAPI::OpenGL: return new OpenGLSwapChain(spec);
        }
        }
    }
}
