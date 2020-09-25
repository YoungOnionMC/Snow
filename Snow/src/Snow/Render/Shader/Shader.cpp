#include <spch.h>
#include "Snow/Render/Shader/Shader.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLShader.h"

namespace Snow {
    namespace Render {
        Ref<Shader> Shader::Create(ShaderType type, const std::string& path) {
            switch(Renderer::GetRenderAPI()) {
            case RenderAPI::None:   return nullptr;
            case RenderAPI::OpenGL: return Ref<OpenGLShader>::Create(type, path);
            }

            return nullptr;
        }
        
    }
}