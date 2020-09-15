#include <spch.h>
#include "Snow/Render/RenderCommand.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLRenderCommand.h"

namespace Snow {
    namespace Render {
        RenderCommand* RenderCommand::Create() {
            switch(Renderer::GetRenderAPI()){
                case RenderAPI::None:   return nullptr;
                case RenderAPI::OpenGL: return new OpenGLRenderCommand();
            }
        }
    }
}