#include <spch.h>

#include "Snow/Platform/Vulkan/VulkanRenderPass.h"

#include "Snow/Platform/Vulkan/VulkanFramebuffer.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"



namespace Snow {
	VulkanRenderPass::VulkanRenderPass(const Render::RenderPassSpecification& spec) :
		m_Specification(spec) {

	}

	void VulkanRenderPass::BeginPass() {
	}

	void VulkanRenderPass::EndPass() {
	}
}