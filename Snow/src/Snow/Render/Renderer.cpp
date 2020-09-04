#include <spch.h>


#include "Snow/Render/Renderer.h"
#include "Snow/Core/Application.h"

namespace Snow {
    namespace Render {
        Context* Renderer::s_Context = nullptr;

        void Renderer::Init() {
            SNOW_CORE_INFO("Initializing Renderer");
            ContextSpecification contextSpec;
            contextSpec.s_RenderAPI = RenderAPI::OpenGL;
            contextSpec.WindowHandle = Core::Application::Get().GetWindow()->GetWindowHandle();

            s_Context = Context::Create(contextSpec);
        }
    }
}