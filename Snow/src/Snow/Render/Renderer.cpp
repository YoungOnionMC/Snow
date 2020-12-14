#include <spch.h>


#include "Snow/Render/Renderer.h"
#include "Snow/Core/Application.h"

#include "Snow/Render/Renderer2D.h"

namespace Snow {
    namespace Render {

        

        RenderAPIType Renderer::s_RenderAPI = RenderAPIType::None;

        struct RendererData {
            Ref<RenderPass> m_ActiveRenderPass;
        };

        static RendererData s_Data;

        Renderer* Renderer::s_Instance = nullptr;
        Context* Renderer::s_Context = nullptr;

        void Renderer::Init() {
            SNOW_CORE_INFO("Initializing Renderer");
            ContextSpecification contextSpec;
            contextSpec.s_RenderAPIType = GetRenderAPI();
            contextSpec.WindowHandle = Core::Application::Get().GetWindow()->GetWindowHandle();

            
            s_Context = Context::Create(contextSpec);

            RenderCommand::Init();

            Renderer2D::Init();
        }

        void Renderer::BeginRenderPass(Ref<RenderPass> renderPass, bool clear) {
            s_Data.m_ActiveRenderPass = renderPass;

            renderPass->GetSpecification().TargetFramebuffer->Bind();
            if (clear) {
                const glm::vec4 clearColor = renderPass->GetSpecification().TargetFramebuffer->GetSpecification().ClearColor;
                RenderCommand::SetClearColor(clearColor);
                RenderCommand::Clear();
            }
        }

        void Renderer::EndRenderPass() {
            if (!s_Data.m_ActiveRenderPass) SNOW_CORE_ERROR("No active render pass!");
            s_Data.m_ActiveRenderPass->GetSpecification().TargetFramebuffer->Unbind();
            s_Data.m_ActiveRenderPass = nullptr;
        }
    }
}