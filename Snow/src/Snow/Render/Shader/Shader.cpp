#include <spch.h>
#include "Snow/Render/Shader/Shader.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLShader.h"
#include "Snow/Platform/Vulkan/VulkanShader.h"

namespace Snow {
    namespace Render {
        Ref<Shader> Shader::Create(ShaderType type, const std::string& path) {
            switch(Renderer::GetRenderAPI()) {
            case RenderAPIType::None:   return nullptr;
            case RenderAPIType::OpenGL: return Ref<OpenGLShader>::Create(type, path);
            case RenderAPIType::Vulkan: return Ref<VulkanShader>::Create(type, path);
            }

            return nullptr;
        }
        
    }
}