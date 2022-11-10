#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Render/Buffer.h"

#include "Snow/Render/RenderCommandBuffer.h"
#include "Snow/Render/ComputePipeline.h"
#include "Snow/Render/Pipeline.h"
#include "Snow/Render/Image.h"
#include "Snow/Render/Material.h"
#include "Snow/Render/Mesh.h"
#include "Snow/Render/StorageBufferSet.h"
#include "Snow/Render/SceneRenderer.h"
#include "Snow/Render/UniformBufferSet.h"
#include "Snow/Render/Shader.h"

#include "Snow/Scene/SceneEnvironment.h"

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
            virtual void RenderMeshWithMaterial(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Mesh> mesh, const glm::mat4& transform, Ref<Material> material, Buffer additionalUniforms) = 0;
            virtual void RenderQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4& transform) = 0;

            virtual void RenderGeometry(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, const glm::mat4& transform, uint32_t indexCount) = 0;
            
            virtual void DispatchComputeShader(Ref<RenderCommandBuffer> commandBuffer, Ref<ComputePipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::ivec3& workGroups) = 0;
            
            virtual void SetSceneEnvironment(Ref<SceneRenderer> sceneRenderer, Ref<Environment> environment, Ref<Image2D> shadow) = 0;
            virtual Ref<TextureCube> CreatePreethamSky(float turbidity, float azimuth, float inclination) = 0;
            

            //virtual void SetBlending(bool blend) = 0;
            //virtual void SetDepthTesting(bool depthTest) = 0;

            //virtual void SetImagePacking(uint32_t size) = 0;


            static RendererAPIType Current() { return s_CurrentRendererAPI; }
            static void SetAPI(RendererAPIType api);
        private:
            inline static RendererAPIType s_CurrentRendererAPI = RendererAPIType::Vulkan;
        };
    }
}