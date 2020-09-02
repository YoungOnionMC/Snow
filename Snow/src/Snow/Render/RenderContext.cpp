#include <spch.h>

#include "Snow/Render/RenderContext.h"

#include "Snow/Platform/OpenGL/OpenGLContext.h"

namespace Snow {
    namespace Render {
        RenderAPI ContextSpecification::s_RenderAPI = RenderAPI::None;
        bool Context::s_Created = false;

        Context* Context::Create(const ContextSpecification& spec) {
            if(s_Created) {
                SNOW_CORE_ERROR("Graphics Context already created");
            }

            switch(spec.s_RenderAPI) {
            case RenderAPI::None:   return nullptr;
            case RenderAPI::OpenGL: return new OpenGLContext(spec);
            }
            SNOW_CORE_ERROR("Context must not be null");
        }
    }
}