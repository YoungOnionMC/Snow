#include <spch.h>

#include "Snow/Render/RenderContext.h"

#include "Snow/Platform/OpenGL/OpenGLContext.h"
#include "Snow/Platform/Vulkan/VulkanContext.h"

#if defined SNOW_PLATFORM_WINDOWS
    #include "Snow/Platform/DirectX11/DirectXContext.h"
#endif

namespace Snow {
    namespace Render {
        RenderAPIType ContextSpecification::s_RenderAPIType = RenderAPIType::None;
        bool Context::s_Created = false;

        Context* Context::Create(const ContextSpecification& spec) {
            if(s_Created) {
                SNOW_CORE_ERROR("Graphics Context already created");
            }

            switch(spec.s_RenderAPIType) {
            case RenderAPIType::None:   return nullptr;
            case RenderAPIType::OpenGL: return new OpenGLContext(spec);
            case RenderAPIType::Vulkan: return new VulkanContext(spec);
#ifdef SNOW_PLATFORM_WINDOWS
            case RenderAPIType::DirectX:    return new DirectX11RenderContext(spec);
#endif
            }
            s_Created = true;
            SNOW_CORE_ERROR("Context must not be null");
            return nullptr;
        }
    }
}