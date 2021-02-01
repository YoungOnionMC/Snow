#include <spch.h>
#include "Snow/Render/Shader/Shader.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLShader.h"
#include "Snow/Platform/Vulkan/VulkanShader.h"

#if defined(SNOW_PLATFORM_WINDOWS)
    #include "Snow/Platform/DirectX11/DirectXShader.h"
#endif

namespace Snow {
    namespace Render {
        Ref<Shader> Shader::Create(const std::initializer_list<ShaderModule>& shaderModules) {
            switch(Renderer::GetRenderAPI()) {
            case RenderAPIType::None:   return nullptr;
            case RenderAPIType::OpenGL: return Ref<OpenGLShader>::Create(shaderModules);
            //case RenderAPIType::Vulkan: return Ref<VulkanShader>::Create(shaderModules);
#if defined(SNOW_PLATFORM_WINDOWS)
            //case RenderAPIType::DirectX:    return Ref<DirectX11Shader>::Create(shaderModules);
#endif
            }

            return nullptr;
        }
        
    }
}