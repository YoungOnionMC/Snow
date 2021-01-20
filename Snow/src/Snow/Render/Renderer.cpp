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

            Ref<API::VertexBuffer> FullscreenQuadVertexBuffer;
            Ref<API::IndexBuffer> FullscreenQuadIndexBuffer;
            Ref<Pipeline> FullscreenQuadPipeline;
        };

        static RendererData s_Data;

        Renderer* Renderer::s_Instance = nullptr;
        Context* Renderer::s_Context = nullptr;

        static void CreateFullscreenData() {
            float x = -1;
            float y = -1;
            float width = 2, height = 2;
            struct QuadVertex {
                glm::vec3 Position;
                glm::vec2 TexCoord;
            };

            QuadVertex* data = new QuadVertex[4];

            data[0].Position = glm::vec3(x, y, 0.1f);
            data[0].TexCoord = glm::vec2(0, 0);

            data[1].Position = glm::vec3(x + width, y, 0.1f);
            data[1].TexCoord = glm::vec2(1, 0);

            data[2].Position = glm::vec3(x + width, y + height, 0.1f);
            data[2].TexCoord = glm::vec2(1, 1);

            data[3].Position = glm::vec3(x, y + height, 0.1f);
            data[3].TexCoord = glm::vec2(0, 1);

            s_Data.FullscreenQuadVertexBuffer = API::VertexBuffer::Create(data, 4 * sizeof(QuadVertex));
            uint32_t indices[6] = { 0, 1, 2, 2, 3, 0, };
            s_Data.FullscreenQuadIndexBuffer = API::IndexBuffer::Create(indices, 6 * sizeof(uint32_t));


            
        }

        void Renderer::Init() {
            SNOW_CORE_INFO("Initializing Renderer");
            ContextSpecification contextSpec;
            contextSpec.s_RenderAPIType = GetRenderAPI();
            contextSpec.WindowHandle = Core::Application::Get().GetWindow()->GetWindowHandle();

            
            s_Context = Context::Create(contextSpec);

            s_Data.m_ShaderLibrary = Ref<ShaderLibrary>::Create();

            if (s_RenderAPI == RenderAPIType::OpenGL) {
                Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/glsl/QuadBatchRenderVert.glsl");
                Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/glsl/QuadBatchRenderFrag.glsl");
                Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/glsl/SceneCompositeVert.glsl");
                Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/glsl/SceneCompositeFrag.glsl");
                Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/glsl/LineBatchRenderVert.glsl");
                Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/glsl/LineBatchRenderFrag.glsl");
                Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/glsl/PBRVert.glsl");
                Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/glsl/PBRFrag.glsl");
            }
            else if (s_RenderAPI == RenderAPIType::DirectX) {
                Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/hlsl/QuadBatchRenderVert.hlsl");
                Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/hlsl/QuadBatchRenderFrag.hlsl");
                Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/hlsl/LineBatchRenderVert.hlsl");
                Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/hlsl/LineBatchRenderFrag.hlsl");
            }
            /*
            Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/hlsl/PBRVert.hlsl");
            Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/hlsl/PBRFrag.hlsl");
            */
            RenderCommand::Init();

            Renderer2D::Init();
            SceneRenderer::Init();

            CreateFullscreenData();
            PipelineSpecification fullscreenPipelineSpec;
            fullscreenPipelineSpec.Type = PrimitiveType::Triangle;
            fullscreenPipelineSpec.Layout = {
                {AttribType::Float3, "a_Position"},
                {AttribType::Float2, "a_TexCoord"}
            };
            fullscreenPipelineSpec.Shaders = { GetShaderLibrary()->Get("SceneCompositeVert"), GetShaderLibrary()->Get("SceneCompositeFrag") };

            s_Data.FullscreenQuadPipeline = Pipeline::Create(fullscreenPipelineSpec);
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

        void Renderer::SubmitFullscreenQuad(Ref<MaterialInstance> material) {

            if (material) {
                material->Bind();
            }

            s_Data.FullscreenQuadVertexBuffer->Bind();
            s_Data.FullscreenQuadPipeline->Bind();
            s_Data.FullscreenQuadIndexBuffer->Bind();

            RenderCommand::DrawIndexed(6, s_Data.FullscreenQuadPipeline->GetSpecification().Type);

        }
        
    }
}