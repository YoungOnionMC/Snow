#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanRenderCommand.h"

#include <vulkan/vulkan.h>

#include "Snow/Platform/Vulkan/VulkanContext.h"
#include "Snow/Platform/Vulkan/VulkanSwapChain.h"

namespace Snow {
    namespace Render {
        VulkanRenderCommand::VulkanRenderCommand() {

        }

        static float r = 0.0f;

        void VulkanRenderCommand::BeginCommandBuffer() {
           

            VulkanContext* vkContext = VulkanContext::Get();

            VulkanSwapChain vkSwapChain = vkContext->GetSwapChain();

            vkSwapChain.BeginFrame();

            VkCommandBufferBeginInfo commandBufferInfo = {};
            commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            commandBufferInfo.pNext = nullptr;


            {
                m_DrawCommandBuffer = vkSwapChain.GetCurrentDrawCommandBuffer();
                VKCheckError(vkBeginCommandBuffer(m_DrawCommandBuffer, &commandBufferInfo));
            }
                /*
                VkRect2D scissor = {};
				scissor.extent.width = width;
				scissor.extent.height = height;
				scissor.offset.x = 0;
				scissor.offset.y = 0;

				vkCmdSetScissor(m_DrawCommandBuffer, 0, 1, &scissor);
                */

            
        }

        void VulkanRenderCommand::EndCommandBuffer() {
            vkCmdEndRenderPass(m_DrawCommandBuffer);
            VKCheckError(vkEndCommandBuffer(m_DrawCommandBuffer));
        }

        void VulkanRenderCommand::SetViewport(uint32_t width, uint32_t height) {
            VulkanContext* vkContext = VulkanContext::Get();

            VulkanSwapChain vkSwapChain = vkContext->GetSwapChain();
            m_DrawCommandBuffer = vkSwapChain.GetCurrentDrawCommandBuffer();

            VkViewport viewport = {};
            viewport.x = 0.0f;
            viewport.y = (float)height;
            viewport.height = -(float)height;
            viewport.width = (float)width;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            vkCmdSetViewport(m_DrawCommandBuffer, 0, 1, &viewport);
        }

        void VulkanRenderCommand::SwapBuffers() {
            VulkanContext* vkContext = VulkanContext::Get();

            VulkanSwapChain vkSwapChain = vkContext->GetSwapChain();
            vkSwapChain.SwapBuffers();
        }
    }
}