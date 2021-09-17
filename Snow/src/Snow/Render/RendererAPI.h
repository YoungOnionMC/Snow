#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Render/Buffer.h"

#include "Snow/Render/RenderCommandBuffer.h"
#include "Snow/Render/Pipeline.h"
#include "Snow/Render/Image.h"
#include "Snow/Render/Material.h"
#include "Snow/Render/Mesh.h"
#include "Snow/Render/StorageBufferSet.h"
#include "Snow/Render/UniformBufferSet.h"
#include "Snow/Render/Shader.h"

#include <glm/glm.hpp>

namespace Snow {
    namespace Render {

        enum class RendererAPIType {
            None = 0,
            OpenGL, DirectX, Vulkan
        };

        class RendererAPI {
        public:

            virtual void Init() = 0;
            virtual void Shutdown() = 0;

            virtual void BeginFrame() = 0;
            virtual void EndFrame() = 0;

            virtual void BeginRenderPass(Ref<RenderCommandBuffer> commandBuffer, const Ref<RenderPass>& renderPass, bool clear) = 0;
            virtual void EndRenderPass(Ref<RenderCommandBuffer> commandBuffer) = 0;

            virtual void ClearImage(Ref<RenderCommandBuffer> commandBuffer, Ref<Image2D> image) = 0;

            virtual void SubmitFullscreenQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<Material> material) = 0;
            virtual void SubmitFullscreenQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material) = 0;
            
            virtual void RenderMesh(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Mesh> mesh, Ref<MaterialTable> materialTable, const glm::mat4& transform) = 0;
            virtual void RenderQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4& transform) = 0;

            virtual void RenderGeometry(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, const glm::mat4& transform, uint32_t indexCount) = 0;
            //virtual void SetViewport(uint32_t width, uint32_t height) = 0;
            //virtual void SetClearColor(const glm::vec4& color) = 0;
            //virtual void Clear() = 0;

            //virtual void SetBlending(bool blend) = 0;
            //virtual void SetDepthTesting(bool depthTest) = 0;

            //virtual void DispatchComputeShader(Ref<Shader>& computeShader, uint32_t xGroup, uint32_t yGroup, uint32_t zGroup) = 0;

            //virtual void SetImagePacking(uint32_t size) = 0;

            //virtual void DrawIndexed(uint32_t count, PrimitiveType type) = 0;

            //virtual void SwapBuffers() = 0;

            //virtual void BeginCommandBuffer() = 0;
            //virtual void EndCommandBuffer() = 0;

            //static Core::Scope<RenderAPI> Create();

            static RendererAPIType Current() { return s_CurrentRendererAPI; }
            static void SetAPI(RendererAPIType api);
        private:
            inline static RendererAPIType s_CurrentRendererAPI = RendererAPIType::Vulkan;
        };
        /*
        class RenderCommand {
        public:
            static void Init() {
                s_RenderAPI = RenderAPI::Create();
                s_RenderAPI->Init();
            }

            static void BeginCommandBuffer() {
                s_RenderAPI->BeginCommandBuffer();
            }

            static void EndCommandBuffer() {
                s_RenderAPI->EndCommandBuffer();
            }

            static void SetClearColor(const glm::vec4& color) {
                s_RenderAPI->SetClearColor(color);
            }

            static void Clear() {
                s_RenderAPI->Clear();
            }

            static void DrawIndexed(uint32_t count, PrimitiveType type) {
                s_RenderAPI->DrawIndexed(count, type);
            }

            static void SetViewport(int width, int height) {
                s_RenderAPI->SetViewport(width, height);
            }

            static void SetBlending(bool blend) {
                s_RenderAPI->SetBlending(blend);
            }

            static void SetDepthTesting(bool depthTest) {
                s_RenderAPI->SetDepthTesting(depthTest);
            }

            static void SetImagePacking(uint32_t size) {
                s_RenderAPI->SetImagePacking(size);
            }

            static void DispatchComputeShader(Ref<Shader>& computeShader, uint32_t xGroup = 1, uint32_t yGroup = 1, uint32_t zGroup = 1) {
                s_RenderAPI->DispatchComputeShader(computeShader, xGroup, yGroup, zGroup);
            }

            static void SwapBuffers() {
                s_RenderAPI->SwapBuffers();
            }
        private:
            static Core::Scope<RenderAPI> s_RenderAPI;
        };
        */
    }
}