#pragma once

#include "Snow/Render/ComputePipeline.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"
#include "Snow/Platform/Vulkan/VulkanShader.h"

namespace Snow {
	using namespace Render;
	class VulkanComputePipeline : public Render::ComputePipeline {
		
	public:
		VulkanComputePipeline(Ref<Shader> computeShader);

		virtual void Begin(Ref<RenderCommandBuffer> commandBuffer = nullptr) override;
		virtual void End() override;

		void Dispatch(VkDescriptorSet descriptorSet,  uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

		void Execute(VkDescriptorSet* descriptorSets, uint32_t descriptorSetCount, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

		void SetPushConstants(const void* data, uint32_t size);

		void CreatePipeline();

		virtual Ref<Shader> GetShader() override { return m_Shader; }
		VkCommandBuffer GetCommandBuffer() { return m_ActiveCommandBuffer; }
	private:
		void RTCreatePipeline();

		VkPipeline m_VulkanPipeline = nullptr;
		VkPipelineLayout m_PipelineLayout = nullptr;
		VkPipelineCache m_PipelineCache = nullptr;

		VkCommandBuffer m_ActiveCommandBuffer = nullptr;

		bool m_UsingGraphicsQueue = false;

		Ref<VulkanShader> m_Shader;
	};
}