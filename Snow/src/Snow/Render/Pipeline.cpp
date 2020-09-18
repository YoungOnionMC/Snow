#include <spch.h>
#include "Snow/Render/Pipeline.h"

#include "Snow/Platform/OpenGL/OpenGLPipeline.h"

#include "Snow/Render/Renderer.h"

namespace Snow {
    namespace Render {
        Ref<Pipeline> Pipeline::Create(const PipelineSpecification& spec) {
            switch(Renderer::GetRenderAPI()) {
            case RenderAPI::None:   return nullptr;
            case RenderAPI::OpenGL: return Ref<OpenGLPipeline>::Create(spec);
            }
        }
    }
}