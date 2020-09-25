#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanPipeline.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"

namespace Snow {
	namespace Render {
		VulkanPipeline::VulkanPipeline(const PipelineSpecification& spec) :
			m_Specification(spec) {

			Invalidate();

		}

		void VulkanPipeline::Invalidate() {
			Ref<VulkanPipeline> instance = this;

			VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

			VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
			graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		}

		void VulkanPipeline::Bind() {
			VulkanContext* vkContext = VulkanContext::Get();
			vkCmdBindPipeline(nullptr, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VulkanPipeline);
		}
	}
}