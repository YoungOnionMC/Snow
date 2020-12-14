#include <spch.h>

#include "Snow/Platform/Vulkan/VulkanRenderPass.h"

#include "Snow/Platform/Vulkan/VulkanFramebuffer.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"



namespace Snow {
	namespace Render {
		VulkanRenderPass::VulkanRenderPass(const RenderPassSpecification& spec) :
			m_Specification(spec) {

		}

		void VulkanRenderPass::BeginPass() {
			auto vkSwapChain = VulkanContext::Get()->GetSwapChain();


			glm::vec4 clearColor = m_Specification.TargetFramebuffer->GetSpecification().ClearColor;
			

			auto vkFramebuffer = m_Specification.TargetFramebuffer.As<VulkanFramebuffer>();

			std::vector<VkClearValue> clearValues;
			clearValues.resize(vkFramebuffer->GetColorAttachments().size());
			for (uint32_t i = 0; i < clearValues.size(); i++) {
				glm::vec4 clearColor = vkFramebuffer->GetColorAttachments()[i].ClearColor;
				clearValues[i].color = { { clearColor.r, clearColor.g, clearColor.b, clearColor.a } };
			}
			if (vkFramebuffer->GetDepthAttachmentFormat() != FramebufferTextureFormat::None) {
				VkClearValue clearValue;
				clearValue.depthStencil = { 1.0f, 0 };
				clearValues.push_back(clearValue);
			}

			VkRenderPassBeginInfo renderpassBeginInfo;
			ZeroInitVulkanStruct(renderpassBeginInfo, VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO);
			renderpassBeginInfo.pNext = nullptr;
			renderpassBeginInfo.renderPass = vkFramebuffer->GetRenderPass();
			renderpassBeginInfo.framebuffer = vkFramebuffer->GetFramebufferHandle();
			renderpassBeginInfo.renderArea.offset.x = 0;
			renderpassBeginInfo.renderArea.offset.y = 0;
			renderpassBeginInfo.renderArea.extent.width = vkFramebuffer->GetWidth();
			renderpassBeginInfo.renderArea.extent.height = vkFramebuffer->GetHeight();
			renderpassBeginInfo.clearValueCount = clearValues.size();
			renderpassBeginInfo.pClearValues = clearValues.data();

			

			{
				m_DrawCommandBuffer = vkSwapChain.GetCurrentDrawCommandBuffer();
				vkCmdBeginRenderPass(m_DrawCommandBuffer, &renderpassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			}
		}

		void VulkanRenderPass::EndPass() {
			auto vkSwapChain = VulkanContext::Get()->GetSwapChain();
			{
				m_DrawCommandBuffer = vkSwapChain.GetCurrentDrawCommandBuffer();
				vkCmdEndRenderPass(m_DrawCommandBuffer);
			}
		}
	}
}