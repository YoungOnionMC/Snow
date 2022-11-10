#pragma once

#include "Snow/Render/RenderCommandBuffer.h"
#include "Snow/Platform/Vulkan/VulkanCommon.h"

namespace Snow {
	class VulkanRenderCommandBuffer : public Render::RenderCommandBuffer {
	public:
		VulkanRenderCommandBuffer(uint32_t count = 0, const std::string& debugName = "");
		VulkanRenderCommandBuffer(const std::string& debugName, bool swapchain);
		~VulkanRenderCommandBuffer();

		const Render::PipelineStatistics GetPipelineStatistics(uint32_t frameIndex) const override { return m_PipelineStatsQueryResults[frameIndex]; }

		virtual void Begin() override;
		virtual void End() override;
		virtual void Submit() override;

		VkCommandBuffer GetCommandBuffer(uint32_t frameIndex) const {
			SNOW_CORE_ASSERT(frameIndex < m_CommandBuffers.size());
			return m_CommandBuffers[frameIndex];
		}
	private:
		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::vector<VkFence> m_WaitFences;

		uint32_t m_PipelineQueryCount;
		std::vector<VkQueryPool> m_PipelineStatsQueryPools;
		std::vector<Render::PipelineStatistics> m_PipelineStatsQueryResults;

		bool m_OwnedBySwapChain = false;

		std::string m_DebugName;

	};
}