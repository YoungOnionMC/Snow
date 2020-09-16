#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanRenderCommand.h"

#include <vulkan/vulkan.h>

#include "Snow/Platform/Vulkan/VulkanContext.h"
#include "Snow/Platform/Vulkan/VulkanSwapChain.h"

namespace Snow {
    namespace Render {
        VulkanRenderCommand::VulkanRenderCommand() {

        }

        void VulkanRenderCommand::BeginScene() {
            VulkanContext* vkContext = VulkanContext::Get();

            VulkanSwapChain vkSwapChain = vkContext->GetSwapChain();

            vkSwapChain.BeginFrame();

            VkCommandBufferBeginInfo commandBufferInfo = {};
            commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            commandBufferInfo.pNext = nullptr;

            VkClearValue clearValues[2];
            clearValues[0].color = { { 1.0f, 0.0f, 0.2f, 1.0f } };
            clearValues[1].depthStencil = {1.0f, 0};

            uint32_t width = vkSwapChain.GetWidth();
            uint32_t height = vkSwapChain.GetHeight();

            VkRenderPassBeginInfo renderpassBeginInfo = {};
            renderpassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderpassBeginInfo.pNext = nullptr;
            renderpassBeginInfo.renderPass = vkSwapChain.GetRenderPass();
            renderpassBeginInfo.renderArea.offset.x = 0;
            renderpassBeginInfo.renderArea.offset.y = 0;
            renderpassBeginInfo.renderArea.extent.width = width;
            renderpassBeginInfo.renderArea.extent.height = height;
            renderpassBeginInfo.clearValueCount = 2;
            renderpassBeginInfo.pClearValues = clearValues;

            renderpassBeginInfo.framebuffer = vkSwapChain.GetCurrentFramebuffer();

            {
                m_DrawCommandBuffer = vkSwapChain.GetCurrentDrawCommandBuffer();
                vkBeginCommandBuffer(m_DrawCommandBuffer, &commandBufferInfo);

                vkCmdBeginRenderPass(m_DrawCommandBuffer, &renderpassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

                VkViewport viewport = {};
				viewport.x = 0.0f;
				viewport.y = (float)height;
				viewport.height = -(float)height;
				viewport.width = (float)width;
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				vkCmdSetViewport(m_DrawCommandBuffer, 0, 1, &viewport);

                VkRect2D scissor = {};
				scissor.extent.width = width;
				scissor.extent.height = height;
				scissor.offset.x = 0;
				scissor.offset.y = 0;
				vkCmdSetScissor(m_DrawCommandBuffer, 0, 1, &scissor);


            }
        }

        void VulkanRenderCommand::EndScene() {
            {
                vkCmdEndRenderPass(m_DrawCommandBuffer);
                vkEndCommandBuffer(m_DrawCommandBuffer);

                VulkanContext* vkContext = VulkanContext::Get();

                VulkanSwapChain vkSwapChain = vkContext->GetSwapChain();
                vkSwapChain.SwapBuffers();
            }
        }

        void VulkanRenderCommand::SetViewport(int width, int height) {
            
        }
    }
}