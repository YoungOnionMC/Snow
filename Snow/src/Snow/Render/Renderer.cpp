#include <spch.h>


#include "Snow/Render/Renderer.h"
#include "Snow/Core/Application.h"

#include "Snow/Render/Renderer2D.h"
#include "Snow/Render/SceneRenderer.h"

#include "Snow/Render/Shader/ShaderLibrary.h"

namespace Snow {
    namespace Render {

        

        RenderAPIType Renderer::s_RenderAPI = RenderAPIType::None;

        struct RendererData {
            Ref<RenderPass> m_ActiveRenderPass;

            Ref<ShaderLibrary> m_ShaderLibrary;
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

            s_Data.m_ShaderLibrary = Ref<ShaderLibrary>::Create();

            /*
            Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/glsl/QuadBatchRenderVert.glsl");
            Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/glsl/QuadBatchRenderFrag.glsl");
            Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/glsl/LineBatchRenderVert.glsl");
            Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/glsl/LineBatchRenderFrag.glsl");
            Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/glsl/PBRVert.glsl");
            Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/glsl/PBRFrag.glsl");
            */

            
            Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/hlsl/QuadBatchRenderVert.hlsl");
            Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/hlsl/QuadBatchRenderFrag.hlsl");
            Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/hlsl/LineBatchRenderVert.hlsl");
            Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/hlsl/LineBatchRenderFrag.hlsl");
            /*
            Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/hlsl/PBRVert.hlsl");
            Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/hlsl/PBRFrag.hlsl");
            */
            RenderCommand::Init();

            Renderer2D::Init();
            SceneRenderer::Init();
        }

        Ref<ShaderLibrary> Renderer::GetShaderLibrary() {
            return s_Data.m_ShaderLibrary;
        }

        void Renderer::BeginRenderPass(Ref<RenderPass> renderPass, bool clear) {
            s_Data.m_ActiveRenderPass = renderPass;

            renderPass->GetSpecification().TargetFramebuffer->Bind();
            renderPass->BeginPass();
        }

        void Renderer::EndRenderPass() {
            if (!s_Data.m_ActiveRenderPass) SNOW_CORE_ERROR("No active render pass!");
            s_Data.m_ActiveRenderPass->GetSpecification().TargetFramebuffer->Unbind();
            s_Data.m_ActiveRenderPass->EndPass();
            s_Data.m_ActiveRenderPass = nullptr;
        }
    }
}