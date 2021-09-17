#include <spch.h>
#include "Snow/Render/Shader.h"

#include "Snow/Render/Renderer.h"

//#include "Snow/Platform/OpenGL/OpenGLShader.h"
#include "Snow/Platform/Vulkan/VulkanShader.h"

#if defined(SNOW_PLATFORM_WINDOWS)
    //#include "Snow/Platform/DirectX11/DirectXShader.h"
#endif

namespace Snow {
    namespace Render {
        Ref<Shader> Shader::Create(const std::initializer_list<ShaderModule>& shaderModules) {
            switch(Render::RendererAPI::Current()) {
            case RendererAPIType::None:   return nullptr;
           // case RendererAPIType::OpenGL: return Ref<OpenGLShader>::Create(shaderModules);
            case RendererAPIType::Vulkan: return Ref<VulkanShader>::Create(shaderModules);
#if defined(SNOW_PLATFORM_WINDOWS)
            //case RenderAPIType::DirectX:    return Ref<DirectX11Shader>::Create(shaderModules);
#endif
            }

            return nullptr;
        }

        ShaderType Shader::ShaderTypeFromString(const std::string& type) {
            if (type == "vertex")
                return ShaderType::Vertex;
            if (type == "fragment" || type == "pixel")
                return ShaderType::Pixel;
            if (type == "compute")
                return ShaderType::Compute;
        }

        std::string Shader::ShaderTypeToString(ShaderType type) {
            switch (type) {
            case ShaderType::Vertex:    return "vertex";
            case ShaderType::Pixel:    return "pixel";
            case ShaderType::Compute:    return "compute";
            case ShaderType::Geometry:    return "geometry";
            }
        }
        
    }
}