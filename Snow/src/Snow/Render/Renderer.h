#pragma once

#include "Snow/Core/Application.h"

#include "Snow/Render/RenderContext.h"
#include "Snow/Render/RenderCommandQueue.h"
#include "Snow/Render/RenderCommandBuffer.h"
#include "Snow/Render/RendererAPI.h"
#include "Snow/Render/Material.h"
#include "Snow/Render/MaterialAsset.h"
#include "Snow/Render/Mesh.h"
#include "Snow/Render/Image.h"
#include "Snow/Render/ComputePipeline.h"
#include "Snow/Render/Pipeline.h"
#include "Snow/Render/TextRenderer.h"
#include "Snow/Render/ShaderLibrary.h"

#include "Snow/Render/RenderCapabilities.h"

#include "Snow/Core/Event/Event.h"
#include "Snow/Core/Window.h"

namespace Snow {
    namespace Render {

        struct RendererConfig {
            uint32_t FramesInFlight = 2;

            bool ComputeEnvironmentMaps = false;

            uint32_t EnvironmentMapResolution = 1024;
            uint32_t IrradianceMapComputeSamples = 512;
        };
        
        class Renderer {
        public:
            typedef void(*RenderCommandFn)(void*);

            
            static Ref<Context> GetContext() { return Core::Application::Get().GetWindow()->GetRenderContext(); }

            static void Init();
            static void Shutdown();

            static RenderCapabilities& GetCapabilities();
            static Ref<ShaderLibrary> GetShaderLibrary();

            template<typename FuncT>
            static void Submit(FuncT&& func) {
                auto renderCmd = [](void* ptr) {
                    auto pFunc = (FuncT*)ptr;
                    (*pFunc)();

                    pFunc->~FuncT();
                };
                auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
                new (storageBuffer) FuncT(std::forward<FuncT>(func));
            }

            template<typename FuncT>
            static void SubmitResourceFree(FuncT&& func) {
                auto renderCmd = [](void* ptr) {
                    auto pFunc = (FuncT*)ptr;
                    (*pFunc)();
                    pFunc->~FuncT();
                };

                Submit([renderCmd, func]() {
                    const uint32_t index = Renderer::GetCurrentFrameIndex();
                    auto storageBuffer = GetRenderResourceReleaseQueue(index).Allocate(renderCmd, sizeof(func));
                    new (storageBuffer) FuncT(std::forward<FuncT>((FuncT&&)func));
                });
            }

            static void WaitAndRender();

            static void BeginFrame();
            static void EndFrame();

            static void BeginRenderPass(Ref<RenderCommandBuffer> commandBuffer, Ref<RenderPass> renderPass, bool clear = false);
            static void EndRenderPass(Ref<RenderCommandBuffer> commandBuffer);

            static void ClearImage(Ref<RenderCommandBuffer> commandBuffer, Ref<Image2D> image);

            static void RenderMesh(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Mesh> mesh, Ref<MaterialTable> materialTable, const glm::mat4& transform);
            static void RenderQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4& transform);

            static void RenderGeometry(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, const glm::mat4& transform, uint32_t indexCount = 0);

            static void SubmitFullscreenQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<Material> material);
            static void SubmitFullscreenQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material);

            static Ref<Texture2D> GetWhiteTexture();
            static Ref<Texture2D> GetBlackTexture();
            static Ref<Texture2D> GetBRDFLutTexture();

            static void RegisterShaderDependency(Ref<Shader> shader, Ref<ComputePipeline> computePipeline);
            static void RegisterShaderDependency(Ref<Shader> shader, Ref<Pipeline> pipeline);
            static void RegisterShaderDependency(Ref<Shader> shader, Ref<Material> material);
            static void OnShaderReloaded(size_t hash);

            //static void SwapBuffers();

           // static void SetRenderAPI(RendererAPIType api) { s_RenderAPI = api; }
           // static RendererAPIType GetRenderAPI() { return s_RenderAPI; }

            // Render Core stuff
            

            // Needed common functions, objects to get from the Main Renderer
            static Renderer* Get() { return s_Instance; }
            static Ref<FontManager> GetFontManager();

            static uint32_t GetCurrentFrameIndex();

            static RendererConfig& GetConfig();

            static RenderCommandQueue& GetRenderResourceReleaseQueue(uint32_t index);
        private:
            static RenderCommandQueue& GetRenderCommandQueue();

            static Renderer* s_Instance;
        };
    }
}