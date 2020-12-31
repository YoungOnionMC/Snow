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
        Ref<Shader> Shader::Create(ShaderType type, const std::string& path) {
            switch(Renderer::GetRenderAPI()) {
            case RenderAPIType::None:   return nullptr;
            case RenderAPIType::OpenGL: return Ref<OpenGLShader>::Create(type, path);
            case RenderAPIType::Vulkan: return Ref<VulkanShader>::Create(type, path);
#if defined(SNOW_PLATFORM_WINDOWS)
            case RenderAPIType::DirectX:    return Ref<DirectX11Shader>::Create(type, path);
#endif
            }

            return nullptr;
        }
        
    }
}