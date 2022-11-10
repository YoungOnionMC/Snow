#include <spch.h>

#include "Snow/Platform/Vulkan/VulkanComputePipeline.h"
#include "Snow/Platform/Vulkan/VulkanRenderer.h"
#include "Snow/Platform/Vulkan/VulkanRenderCommandBuffer.h"
#include "Snow/Platform/Vulkan/VulkanShader.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"

#include "Snow/Render/Renderer.h"

namespace Snow {
	static VkFence s_ComputeFence = nullptr;

	using namespace Render;
	VulkanComputePipeline::VulkanComputePipeline(Ref<Shader> computeShader) {
		SNOW_CORE_ASSERT(computeShader->GetModuleCount() == 1);
		SNOW_CORE_ASSERT(computeShader->GetModuleTypes()[0] == Render::ShaderType::Compute, "Shader is not a ComputeShader");
		m_Shader = computeShader.As<VulkanShader>();

		Ref<VulkanComputePipeline> instance = this;
		Renderer::Submit([instance]() mutable {
			instance->RTCreatePipeline();
		});
		Renderer::RegisterShaderDependency(computeShader, this);
	}

	void VulkanComputePipeline::CreatePipeline() {
		Renderer::Submit([instance = Ref(this)]() mutable {
			instance->RTCreatePipeline();
		});
	}

	void VulkanComputePipeline::RTCreatePipeline() {

		auto vkDevice = VulkanContext::GetCurrentDevice();

		auto descriptorSetLayouts = m_Shader->GetAllDescriptorSetLayouts();

		VkPipelineLayoutCreateInfo pipelineLayoutCI = {};
		pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCI.setLayoutCount = descriptorSetLayouts.size();
		pipelineLayoutCI.pSetLayouts = descriptorSetLayouts.data();

		auto pushConstantRanges = m_Shader->GetPushConstantRanges();

		std::vector<VkPushConstantRange> vkPushConstantRanges;
		for (auto pcr : pushConstantRanges) {
			VkPushConstantRange pushconstantrange;
			pushconstantrange.stageFlags = pcr.ShaderStage;
			pushconstantrange.size = pcr.Size;
			pushconstantrange.offset = pcr.Offset;
			vkPushConstantRanges.push_back(pushconstantrange);
		}
		pipelineLayoutCI.pushConstantRangeCount = vkPushConstantRanges.size();
		pipelineLayoutCI.pPushConstantRanges = vkPushConstantRanges.data();

		VKCheckError(vkCreatePipelineLayout(vkDevice->GetVulkanDevice(), &pipelineLayoutCI, nullptr, &m_PipelineLayout));

		VkComputePipelineCreateInfo computePipelineCI = {};
		computePipelineCI.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		computePipelineCI.layout = m_PipelineLayout;
		computePipelineCI.flags = 0;

		VkPipelineShaderStageCreateInfo shaderStageCI = {};
		shaderStageCI.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageCI.stage = Render::Utils::ShaderTypeToVKShaderStage(m_Shader->GetShaderModuleType(0));
		SNOW_CORE_ASSERT(m_Shader->GetShaderModuleType(0) == Render::ShaderType::Compute, "Compute pipeline needs a compute shader!");
		shaderStageCI.module = m_Shader->GetVulkanShaderModule(0);
		shaderStageCI.pName = "main";
		
		computePipelineCI.stage = shaderStageCI;

		VkPipelineCacheCreateInfo pipelineCacheCI = {};
		pipelineCacheCI.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

		VKCheckError(vkCreatePipelineCache(vkDevice->GetVulkanDevice(), &pipelineCacheCI, nullptr, &m_PipelineCache));

		VKCheckError(vkCreateComputePipelines(vkDevice->GetVulkanDevice(), m_PipelineCache, 1, &computePipelineCI, nullptr, &m_VulkanPipeline));
	}

	void VulkanComputePipeline::Begin(Ref<RenderCommandBuffer> commandBuffer) {

		if (commandBuffer) {
			uint32_t frameIndex = Render::Renderer::GetCurrentFrameIndex();
			m_ActiveCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(frameIndex);
			m_UsingGraphicsQueue = true;
		}
		else {
			m_ActiveCommandBuffer = VulkanContext::GetCurrentDevice()->GetCommandBuffer(true, true);
			m_UsingGraphicsQueue = false;
		}

		vkCmdBindPipeline(m_ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_VulkanPipeline);
	}

	void VulkanComputePipeline::End() {
		auto vkDevice = VulkanContext::GetCurrentDevice();

		if (!m_UsingGraphicsQueue) {
			VkQueue computeQueue = vkDevice->GetComputeQueue();

			vkEndCommandBuffer(m_ActiveCommandBuffer);

			if (!s_ComputeFence) {
				VkFenceCreateInfo fenceCI = {};
				fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;
				VKCheckError(vkCreateFence(vkDevice->GetVulkanDevice(), &fenceCI, nullptr, &s_ComputeFence));
			}

			vkWaitForFences(vkDevice->GetVulkanDevice(), 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
			vkResetFences(vkDevice->GetVulkanDevice(), 1, &s_ComputeFence);

			VkSubmitInfo computeSubmitInfo = {};
			computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			computeSubmitInfo.commandBufferCount = 1;
			computeSubmitInfo.pCommandBuffers = &m_ActiveCommandBuffer;
			VKCheckError(vkQueueSubmit(computeQueue, 1, &computeSubmitInfo, s_ComputeFence));

			
			vkWaitForFences(vkDevice->GetVulkanDevice(), 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
		}
		m_ActiveCommandBuffer = nullptr;
	}

	void VulkanComputePipeline::SetPushConstants(const void* data, uint32_t size) {
		vkCmdPushConstants(m_ActiveCommandBuffer, m_PipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, size, data);
	}

	void VulkanComputePipeline::Dispatch(VkDescriptorSet descriptorSet, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {

		vkCmdBindDescriptorSets(m_ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_PipelineLayout, 0, 1, &descriptorSet, 0, 0);
		vkCmdDispatch(m_ActiveCommandBuffer, groupCountX, groupCountY, groupCountZ);
	}

	void VulkanComputePipeline::Execute(VkDescriptorSet* descriptorSets, uint32_t descriptorSetCount, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
		auto vkDevice = VulkanContext::GetCurrentDevice();

		VkQueue computeQueue = vkDevice->GetComputeQueue();

		VkCommandBuffer commandBuffer = vkDevice->GetCommandBuffer(true, true);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_VulkanPipeline);

		for (uint32_t i = 0; i < descriptorSetCount; i++) {
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_PipelineLayout, 0, 1, &descriptorSets[i], 0, 0);
			vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
		}

		vkEndCommandBuffer(commandBuffer);

		if (!s_ComputeFence) {
			VkFenceCreateInfo fenceCI = {};
			fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			VKCheckError(vkCreateFence(vkDevice->GetVulkanDevice(), &fenceCI, nullptr, &s_ComputeFence));
		}

		vkWaitForFences(vkDevice->GetVulkanDevice(), 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
		vkResetFences(vkDevice->GetVulkanDevice(), 1, &s_ComputeFence);

		VkSubmitInfo computeSubmitInfo = {};
		computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		computeSubmitInfo.commandBufferCount = 1;
		computeSubmitInfo.pCommandBuffers = &commandBuffer;
		VKCheckError(vkQueueSubmit(computeQueue, 1, &computeSubmitInfo, s_ComputeFence));


		vkWaitForFences(vkDevice->GetVulkanDevice(), 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);

	}
}