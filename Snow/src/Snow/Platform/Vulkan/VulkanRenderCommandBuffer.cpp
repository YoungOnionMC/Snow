#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanRenderCommandBuffer.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"

#include "Snow/Core/Application.h"

namespace Snow {
	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(uint32_t count, const std::string& debugName) :
		m_DebugName(debugName) {
		auto device = VulkanContext::GetCurrentDevice();
		uint32_t framesInFlight = Renderer::GetConfig().FramesInFlight;

		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = device->GetQueueFamilyIndices().Graphics;
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VKCheckError(vkCreateCommandPool(device->GetVulkanDevice(), &cmdPoolInfo, nullptr, &m_CommandPool));

		VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = m_CommandPool;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		if (count == 0)
			count = Render::Renderer::GetConfig().FramesInFlight;
		commandBufferAllocateInfo.commandBufferCount = count;
		m_CommandBuffers.resize(count);
		VKCheckError(vkAllocateCommandBuffers(device->GetVulkanDevice(), &commandBufferAllocateInfo, m_CommandBuffers.data()));

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		m_WaitFences.resize(Render::Renderer::GetConfig().FramesInFlight);
		for (auto& fence : m_WaitFences)
			VKCheckError(vkCreateFence(device->GetVulkanDevice(), &fenceCreateInfo, nullptr, &fence));

		VkQueryPoolCreateInfo queryPoolCI = {};
		queryPoolCI.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
		queryPoolCI.pNext = nullptr;

		m_PipelineQueryCount = 7;
		queryPoolCI.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;
		queryPoolCI.queryCount = m_PipelineQueryCount;
		queryPoolCI.pipelineStatistics = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;

		m_PipelineStatsQueryPools.resize(framesInFlight);
		for (auto& pipelineStatPool : m_PipelineStatsQueryPools)
			VKCheckError(vkCreateQueryPool(device->GetVulkanDevice(), &queryPoolCI, nullptr, &pipelineStatPool));

		m_PipelineStatsQueryResults.resize(framesInFlight);
	}

	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(const std::string& debugName, bool swapchain) :
		m_OwnedBySwapChain(true), m_DebugName(debugName) {
		auto device = VulkanContext::GetCurrentDevice();
		uint32_t framesInFlight = Renderer::GetConfig().FramesInFlight;

		m_CommandBuffers.resize(framesInFlight);
		Ref<VulkanSwapChain> swapChain = Core::Application::Get().GetWindow()->GetSwapChain().As<VulkanSwapChain>();
		for (uint32_t frame = 0; frame < framesInFlight; frame++)
				m_CommandBuffers[frame] = swapChain->GetDrawCommandBuffer(frame);

		VkQueryPoolCreateInfo queryPoolCI = {};
		queryPoolCI.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
		queryPoolCI.pNext = nullptr;

		m_PipelineQueryCount = 7;
		queryPoolCI.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;
		queryPoolCI.queryCount = m_PipelineQueryCount;
		queryPoolCI.pipelineStatistics = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;

		m_PipelineStatsQueryPools.resize(framesInFlight);
		for (auto& pipelineStatPool : m_PipelineStatsQueryPools)
			VKCheckError(vkCreateQueryPool(device->GetVulkanDevice(), &queryPoolCI, nullptr, &pipelineStatPool));

		m_PipelineStatsQueryResults.resize(framesInFlight);
	}

	VulkanRenderCommandBuffer::~VulkanRenderCommandBuffer() {
		if (m_OwnedBySwapChain)
			return;

		VkCommandPool commandPool = m_CommandPool;
		Render::Renderer::SubmitResourceFree([commandPool]() {
			auto device = VulkanContext::GetCurrentDevice();
			vkDestroyCommandPool(device->GetVulkanDevice(), commandPool, nullptr);
		});
	}

	void VulkanRenderCommandBuffer::Begin() {
		Ref<VulkanRenderCommandBuffer> instance = this;
		Render::Renderer::Submit([instance]() {
			uint32_t frameIndex = Render::Renderer::GetCurrentFrameIndex();

			VkCommandBufferBeginInfo cmdBufferInfo = {};
			cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			cmdBufferInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			cmdBufferInfo.pNext = nullptr;

			VKCheckError(vkBeginCommandBuffer(instance->m_CommandBuffers[frameIndex], &cmdBufferInfo));

			vkCmdResetQueryPool(instance->m_CommandBuffers[frameIndex], instance->m_PipelineStatsQueryPools[frameIndex], 0, instance->m_PipelineQueryCount);
			vkCmdBeginQuery(instance->m_CommandBuffers[frameIndex], instance->m_PipelineStatsQueryPools[frameIndex], 0, 0);
		});
	}

	void VulkanRenderCommandBuffer::End() {
		Ref<VulkanRenderCommandBuffer> instance = this;
		Render::Renderer::Submit([instance]() {
			uint32_t frameIndex = Render::Renderer::GetCurrentFrameIndex();

			vkCmdEndQuery(instance->m_CommandBuffers[frameIndex], instance->m_PipelineStatsQueryPools[frameIndex], 0);
			VKCheckError(vkEndCommandBuffer(instance->m_CommandBuffers[frameIndex]));
		});
	}

	void VulkanRenderCommandBuffer::Submit() {
		if (m_OwnedBySwapChain)
			return;

		Ref<VulkanRenderCommandBuffer> instance = this;
		Render::Renderer::Submit([instance]() {
			auto device = VulkanContext::GetCurrentDevice();
			uint32_t frameIndex = Render::Renderer::GetCurrentFrameIndex();

			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			VkCommandBuffer commandBuffer = instance->m_CommandBuffers[Render::Renderer::GetCurrentFrameIndex()];
			submitInfo.pCommandBuffers = &commandBuffer;

			VKCheckError(vkWaitForFences(device->GetVulkanDevice(), 1, &instance->m_WaitFences[frameIndex], VK_TRUE, UINT64_MAX));
			VKCheckError(vkResetFences(device->GetVulkanDevice(), 1, &instance->m_WaitFences[frameIndex]));
			VKCheckError(vkQueueSubmit(device->GetGraphicsQueue(), 1, &submitInfo, instance->m_WaitFences[frameIndex]));

			vkGetQueryPoolResults(device->GetVulkanDevice(), instance->m_PipelineStatsQueryPools[frameIndex], 0, 1, sizeof(PipelineStatistics), (void*)&instance->m_PipelineStatsQueryResults[frameIndex], sizeof(uint64_t), VK_QUERY_RESULT_64_BIT);
		});
	}
}