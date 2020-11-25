#include <spch.h>
#include "Snow/Render/Pipeline.h"

#include "Snow/Platform/OpenGL/OpenGLPipeline.h"
#include "Snow/Platform/Vulkan/VulkanPipeline.h"

#include "Snow/Render/Renderer.h"

namespace Snow {
    namespace Render {
        Ref<Pipeline> Pipeline::Create(const PipelineSpecification& spec) {
            switch(Renderer::GetRenderAPI()) {
            case RenderAPIType::None:   return nullptr;
            case RenderAPIType::OpenGL: return Ref<OpenGLPipeline>::Create(spec);
            case RenderAPIType::Vulkan: return Ref<VulkanPipeline>::Create(spec);
            }

            return nullptr;
        }
    }
}