#include <spch.h>


#include "Snow/Render/Renderer.h"
#include "Snow/Core/Application.h"

#include "Snow/Render/RendererAPI.h"
#include "Snow/Render/SceneRenderer.h"
#include "Snow/Render/Renderer2D.h"
#include "Snow/Render/TextRenderer.h"

#include "Snow/Render/ShaderLibrary.h"

#include "Snow/Platform/Vulkan/VulkanRenderer.h"

namespace Snow {
    namespace Render {

        

        static RendererAPI* s_RendererAPI = nullptr;

        void RendererAPI::SetAPI(RendererAPIType api) {
            s_CurrentRendererAPI = api;
        }

        struct RendererData {
            Ref<ShaderLibrary> m_ShaderLibrary;
            Ref<FontManager> m_FontManager;

            Ref<Texture2D> WhiteTexture;
            Ref<Texture2D> BlackTexture;
            Ref<Texture2D> BRDFLutTexture;
            Ref<TextureCube> BlackCubeTexture;

            Ref<Environment> EmptyEnvironment;
        };

        static RendererData* s_Data = nullptr;

        static RenderCommandQueue* s_CommandQueue = nullptr;
        static std::vector<RenderCommandQueue*> s_ResourceFreeQueue;

        static std::unordered_map<size_t, Ref<Pipeline>> s_PipelineCache;

        struct ShaderDependencies {
            std::vector<Ref<ComputePipeline>> ComputePipelines;
            std::vector<Ref<Pipeline>> Pipelines;
            std::vector<Ref<Material>> Materials;
        };

        static std::unordered_map<size_t, ShaderDependencies> s_ShaderDependencies;

        void Renderer::RegisterShaderDependency(Ref<Shader> shader, Ref<ComputePipeline> computePipeline) {
            s_ShaderDependencies[shader->GetHash()].ComputePipelines.push_back(computePipeline);
        }

        void Renderer::RegisterShaderDependency(Ref<Shader> shader, Ref<Pipeline> pipeline) {
            s_ShaderDependencies[shader->GetHash()].Pipelines.push_back(pipeline);
        }

        void Renderer::RegisterShaderDependency(Ref<Shader> shader, Ref<Material> material) {
            s_ShaderDependencies[shader->GetHash()].Materials.push_back(material);
        }

        void Renderer::OnShaderReloaded(size_t hash) {
            if (s_ShaderDependencies.find(hash) != s_ShaderDependencies.end()) {
                auto& dependencies = s_ShaderDependencies.at(hash);
                for (auto& pipeline : dependencies.Pipelines) {
                    pipeline->Invalidate();
                }
                for (auto& pipeline : dependencies.ComputePipelines) {
                    
                    //pipeline->Invalidate();
                }
                for (auto& material : dependencies.Materials) {
                    material->Invalidate();
                }
            }
        }

        static RendererAPI* InitRendererAPI() {
            switch (RendererAPI::Current()) {
                case RendererAPIType::Vulkan: return new VulkanRenderer();
            }
            SNOW_CORE_ASSERT(false, "unknown rendererAPI");
            return nullptr;
        }

        void Renderer::Init() {
            SNOW_CORE_INFO("Initializing Renderer");
            s_Data = new RendererData();
            s_CommandQueue = new RenderCommandQueue();

            s_ResourceFreeQueue.resize(GetConfig().FramesInFlight);
            for (auto& queue : s_ResourceFreeQueue)
                queue = new RenderCommandQueue();

            GetConfig().FramesInFlight = std::min(GetConfig().FramesInFlight, Core::Application::Get().GetWindow()->GetSwapChain()->GetImageCount());

            s_RendererAPI = InitRendererAPI();


            s_Data->m_ShaderLibrary = Ref<ShaderLibrary>::Create();

            Renderer::GetShaderLibrary()->Load({ { ShaderType::None, "assets/shaders/glsl/SceneComposite.glsl" } });
            Renderer::GetShaderLibrary()->Load({ { ShaderType::None, "assets/shaders/glsl/Texture2D.glsl" } });
            Renderer::GetShaderLibrary()->Load({ { ShaderType::None, "assets/shaders/glsl/Grid.glsl" } });
            Renderer::GetShaderLibrary()->Load({ { ShaderType::None, "assets/shaders/glsl/PBR.glsl" } });
            Renderer::GetShaderLibrary()->Load({ { ShaderType::None, "assets/shaders/glsl/Renderer2D-Quad.glsl" } });
            Renderer::GetShaderLibrary()->Load({ { ShaderType::None, "assets/shaders/glsl/Renderer2D-Circle.glsl" } });
            Renderer::GetShaderLibrary()->Load({ { ShaderType::None, "assets/shaders/glsl/Renderer2D-Line.glsl" } });
            Renderer::GetShaderLibrary()->Load({ { ShaderType::Compute, "assets/shaders/glsl/PreethamSky.glsl" } });
            Renderer::GetShaderLibrary()->Load({ { ShaderType::None, "assets/shaders/glsl/PreDepth.glsl" } });
            Renderer::GetShaderLibrary()->Load({ { ShaderType::None, "assets/shaders/glsl/ShadowMap.glsl" } });
            Renderer::GetShaderLibrary()->Load({ { ShaderType::None, "assets/shaders/glsl/Skybox.glsl" } });

            Renderer::WaitAndRender();
            /*
            if (s_RendererAPI->Current() == RendererAPIType::OpenGL || s_RendererAPI->Current() == RendererAPIType::Vulkan) {
                Renderer::GetShaderLibrary()->Load({
                    { ShaderType::Vertex, "assets/shaders/glsl/QuadBatchRender.vert.glsl" },
                    { ShaderType::Pixel, "assets/shaders/glsl/QuadBatchRender.frag.glsl" } });

                Renderer::GetShaderLibrary()->Load({
                    { ShaderType::Vertex, "assets/shaders/glsl/SceneComposite.vert.glsl" },
                    { ShaderType::Pixel, "assets/shaders/glsl/SceneComposite.frag.glsl" } });

                Renderer::GetShaderLibrary()->Load({
                    { ShaderType::Vertex, "assets/shaders/glsl/LineBatchRender.vert.glsl" },
                    { ShaderType::Pixel, "assets/shaders/glsl/LineBatchRender.frag.glsl" } });

                Renderer::GetShaderLibrary()->Load({
                    { ShaderType::Vertex, "assets/shaders/glsl/PBR.vert.glsl" },
                    { ShaderType::Pixel, "assets/shaders/glsl/PBR.frag.glsl" } });

                //Renderer::GetShaderLibrary()->Load({
                //    { ShaderType::None, "assets/shaders/glsl/PBR.glsl" } });
            }
            else if (s_RendererAPI->Current() == RendererAPIType::OpenGL) {
                //Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/hlsl/QuadBatchRenderVert.hlsl");
                //Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/hlsl/QuadBatchRenderFrag.hlsl");
                //Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/hlsl/SceneCompositeVert.hlsl");
                //Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/hlsl/SceneCompositeFrag.hlsl");
                //Renderer::GetShaderLibrary()->Load(ShaderType::Vertex, "assets/shaders/hlsl/LineBatchRenderVert.hlsl");
                //Renderer::GetShaderLibrary()->Load(ShaderType::Pixel, "assets/shaders/hlsl/LineBatchRenderFrag.hlsl");
            }
            */

            // Compile Shaders
            Renderer::WaitAndRender();

            uint32_t whiteTextureData = 0xffffffff;
            s_Data->WhiteTexture = Texture2D::Create(ImageFormat::RGBA, 1, 1, &whiteTextureData);
            s_Data->WhiteTexture->GetImage()->GetSpecification().DebugName = "White Texture";

            uint32_t blackTextureData = 0xff000000;
            s_Data->BlackTexture = Texture2D::Create(ImageFormat::RGBA, 1, 1, &blackTextureData);
            s_Data->BlackTexture->GetImage()->GetSpecification().DebugName = "Black Texture";

            {
                TextureProperties props;
                props.SamplerWrap = TextureWrap::Clamp;
                s_Data->BRDFLutTexture = Texture2D::Create("assets/textures/BRDF_LUT.tga", props);
                s_Data->BRDFLutTexture->GetImage()->GetSpecification().DebugName = "BRDF LUT";
            }

            uint32_t blackCubeTextureData[6] = { 0xffff0000, 0xffffff00, 0xff00ff00, 0xff00ffff, 0xff0000ff, 0xffff00ff };
            s_Data->BlackCubeTexture = TextureCube::Create(ImageFormat::RGBA, 1, 1, &blackCubeTextureData);

            s_Data->EmptyEnvironment = Ref<Environment>::Create(s_Data->BlackCubeTexture, s_Data->BlackCubeTexture);
            //Renderer2D::Init();
            //RenderCommand::Init();

            //
            //SceneRenderer::Init();

            //TextRenderer::Init();
            //Renderer::GetFontManager()->Load("assets/fonts/comic.ttf");

            s_RendererAPI->Init();
        }

        void Renderer::Shutdown() {
            //Renderer2D::Shutdown();

            s_ShaderDependencies.clear();
            s_RendererAPI->Shutdown();

            delete s_Data;
            delete s_CommandQueue;
        }

        Ref<ShaderLibrary> Renderer::GetShaderLibrary() {
            return s_Data->m_ShaderLibrary;
        }

        Ref<FontManager> Renderer::GetFontManager() {
            return s_Data->m_FontManager;
        }

        void Renderer::WaitAndRender() {
            s_CommandQueue->Execute();
        }

        void Renderer::BeginFrame() {
            s_RendererAPI->BeginFrame();
        }

        void Renderer::EndFrame() {
            s_RendererAPI->EndFrame();
        }

        void Renderer::BeginRenderPass(Ref<RenderCommandBuffer> commandBuffer, Ref<RenderPass> renderPass, bool clear) {
            s_RendererAPI->BeginRenderPass(commandBuffer, renderPass, clear);
        }

        void Renderer::EndRenderPass(Ref<RenderCommandBuffer> commandBuffer) {
            s_RendererAPI->EndRenderPass(commandBuffer);
        }

        void Renderer::ClearImage(Ref<RenderCommandBuffer> commandBuffer, Ref<Image2D> image) {
            s_RendererAPI->ClearImage(commandBuffer, image);
        }

        void Renderer::RenderMesh(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Mesh> mesh, Ref<MaterialTable> materialTable, const glm::mat4& transform) {
            s_RendererAPI->RenderMesh(commandBuffer, pipeline, uniformBufferSet, storageBufferSet, mesh, materialTable, transform);
        }

        void Renderer::RenderMeshWithMaterial(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Mesh> mesh, const glm::mat4& transform, Ref<Material> material, Buffer additionalUniforms) {
            s_RendererAPI->RenderMeshWithMaterial(commandBuffer, pipeline, uniformBufferSet, storageBufferSet, mesh, transform, material, additionalUniforms);
        }

        void Renderer::RenderQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4& transform) {
            s_RendererAPI->RenderQuad(commandBuffer, pipeline, uniformBufferSet, storageBufferSet, material, transform);
        }

        void Renderer::RenderGeometry(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, const glm::mat4& transform, uint32_t indexCount) {
            s_RendererAPI->RenderGeometry(commandBuffer, pipeline, uniformBufferSet, storageBufferSet, material, vertexBuffer, indexBuffer, transform, indexCount);
        }

        void Renderer::SubmitFullscreenQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<Material> material) {
            s_RendererAPI->SubmitFullscreenQuad(commandBuffer, pipeline, uniformBufferSet, material);
        }

        void Renderer::SubmitFullscreenQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material) {
            s_RendererAPI->SubmitFullscreenQuad(commandBuffer, pipeline, uniformBufferSet, storageBufferSet, material); 
        }

        void Renderer::DispatchComputeShader(Ref<RenderCommandBuffer> commandBuffer, Ref<ComputePipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::ivec3& workGroups) {
            s_RendererAPI->DispatchComputeShader(commandBuffer, pipeline, uniformBufferSet, storageBufferSet, material, workGroups);
        }

        void Renderer::SetSceneEnvironment(Ref<SceneRenderer> sceneRenderer, Ref<Environment> environment, Ref<Image2D> shadow) {
            s_RendererAPI->SetSceneEnvironment(sceneRenderer, environment, shadow);
        }

        Ref<TextureCube> Renderer::CreatePreethamSky(float turbidity, float azimuth, float inclination) {
            return s_RendererAPI->CreatePreethamSky(turbidity, azimuth, inclination);
        }

        Ref<Texture2D> Renderer::GetWhiteTexture() {
            return s_Data->WhiteTexture;
        }

        Ref<Texture2D> Renderer::GetBlackTexture() {
            return s_Data->BlackTexture;
        }

        Ref<Texture2D> Renderer::GetBRDFLutTexture() {
            return s_Data->BRDFLutTexture;
        }

        Ref<TextureCube> Renderer::GetBlackCubeTexture() {
            return s_Data->BlackCubeTexture;
        }

        Ref<Environment> Renderer::GetEmptyEnvironment() {
            return s_Data->EmptyEnvironment;
        }

        RenderCommandQueue& Renderer::GetRenderCommandQueue() {
            return *s_CommandQueue;
        }
        RenderCommandQueue& Renderer::GetRenderResourceReleaseQueue(uint32_t index) {
            return *s_ResourceFreeQueue[index];
        }

        uint32_t Renderer::GetCurrentFrameIndex() {
            return Core::Application::Get().GetWindow()->GetSwapChain()->GetCurrentBufferIndex();
        }

        RendererConfig& Renderer::GetConfig() {
            static RendererConfig config;
            return config;
        }
        
    }
}