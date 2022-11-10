#pragma once

#include "Snow/Render/RendererAPI.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"
#include "Snow/Platform/Vulkan/VulkanMaterial.h"

namespace Snow {
    using namespace Render;
    class VulkanRenderer : public Render::RendererAPI {
       
    public:
        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void BeginFrame() override;
        virtual void EndFrame() override;

        virtual void BeginRenderPass(Ref<RenderCommandBuffer> commandBuffer, const Ref<RenderPass>& renderPass, bool clear) override;
        virtual void EndRenderPass(Ref<RenderCommandBuffer> commandBuffer) override;

        virtual void ClearImage(Ref<RenderCommandBuffer> commandBuffer, Ref<Image2D> image) override;

        virtual void SubmitFullscreenQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<Material> material) override;
        virtual void SubmitFullscreenQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material) override;

        virtual void RenderMesh(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Mesh>, Ref<MaterialTable> materialTable, const glm::mat4& transform) override;
        virtual void RenderMeshWithMaterial(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Mesh>, const glm::mat4& transform, Ref<Material> material, Buffer additionalUniforms) override;
        virtual void RenderQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4& transform) override;

        virtual void RenderGeometry(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, const glm::mat4& transform, uint32_t indexCount) override;

        virtual void DispatchComputeShader(Ref<RenderCommandBuffer> commandBuffer, Ref<ComputePipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::ivec3& workGroups) override;

        virtual void SetSceneEnvironment(Ref<SceneRenderer> sceneRenderer, Ref<Environment> environment, Ref<Image2D> shadow) override;
        virtual Ref<TextureCube> CreatePreethamSky(float turbidity, float azimuth, float inclination) override;

        static void RTUpdateMaterialForRendering(Ref<VulkanMaterial> material, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet);
        //virtual void DrawIndexed(uint32_t indexCount, PrimitiveType type) override {}

        //virtual void SetViewport(uint32_t width, uint32_t height) override;

        //virtual void BeginCommandBuffer() override;
        //virtual void EndCommandBuffer() override;

        //void SetImagePacking(uint32_t size) override {}

        //virtual void SwapBuffers() override;

        static VkDescriptorSet RTAllocateDescriptorSet(VkDescriptorSetAllocateInfo& allocInfo);
    private:
        VkCommandBuffer m_DrawCommandBuffer;
    };

    namespace Render::Utils {
        void InsertImageMemoryBarrier(VkCommandBuffer cmdBuffer,
            VkImage image,
            VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
            VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
            VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
            VkImageSubresourceRange subresourceRange);

        void SetImageLayout(VkCommandBuffer cmdbuffer, VkImage image,
            VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
            VkImageSubresourceRange subresourceRange,
            VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

        void SetImageLayout(VkCommandBuffer cmdbuffer, VkImage image,
            VkImageAspectFlags aspectMask,
            VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
            VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

        inline VkShaderStageFlagBits ShaderTypeToVKShaderStage(Render::ShaderType type) {
            switch (type) {
            case Render::ShaderType::Vertex:	return VK_SHADER_STAGE_VERTEX_BIT;
            case Render::ShaderType::Pixel:		return VK_SHADER_STAGE_FRAGMENT_BIT;
            case Render::ShaderType::Geometry:	return VK_SHADER_STAGE_GEOMETRY_BIT;
            case Render::ShaderType::Compute:	return VK_SHADER_STAGE_COMPUTE_BIT;
            }
        }
    }
}