#pragma once

#include "Snow/Render/RenderPass.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

namespace Snow {
	using namespace Render;
	class VulkanRenderPass : public Render::RenderPass {
	public:
		VulkanRenderPass(const RenderPassSpecification& spec);

		virtual void BeginPass() override;
		virtual void EndPass() override;

		virtual RenderPassSpecification& GetSpecification() override { return m_Specification; }
		virtual const RenderPassSpecification& GetSpecification() const override { return m_Specification; }

	private:
		RenderPassSpecification m_Specification;

		VkCommandBuffer m_DrawCommandBuffer;
	};
}