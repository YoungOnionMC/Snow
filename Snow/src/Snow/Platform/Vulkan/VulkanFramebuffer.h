#pragma once

#include "Snow/Render/Framebuffer.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"
#include "Snow/Platform/Vulkan/VulkanImage.h"

namespace Snow {

	class VulkanFramebuffer : public Render::Framebuffer {
	public:
		VulkanFramebuffer(const Render::FramebufferSpecification& spec);
		virtual ~VulkanFramebuffer();

		virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate) override;

		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override {}

		//virtual void* GetColorAttachmentTexture(int index = 0) const override { return 0; }
		//virtual void* GetDepthAttachmentTexture() const override { return 0; }

		VkFramebuffer GetFramebufferHandle() { return m_Framebuffer; }
		VkRenderPass GetRenderPass() { return m_RenderPass; }

		virtual Ref<Render::Image2D> GetImage(uint32_t attachmentIndex = 0) const override { SNOW_CORE_ASSERT(attachmentIndex < m_ColorAttachmentImages.size()); return m_ColorAttachmentImages[attachmentIndex]; }
		virtual Ref<Render::Image2D> GetDepthImage() const override { return m_DepthAttachmentImage; }
		size_t GetColorAttachmentCount() const { return (m_Specification.SwapChainTarget ? 1 : m_ColorAttachmentImages.size()); }

		const std::vector<VkClearValue> GetVkClearValues() const { return m_ClearValues; }

		bool HasDepthAttachment() const { return m_DepthAttachmentImage; }

		//const Render::ImageFormat GetDepthAttachmentFormat() const { return m_DepthAttachmentFormat; }

		const Render::FramebufferSpecification& GetSpecification() const override { return m_Specification; }

		void Invalidate();
		void RTInvalidate();
	private:


		Render::FramebufferSpecification m_Specification;
		uint32_t m_Width = 0, m_Height = 0;

		VkRenderPass m_RenderPass = nullptr;
		VkFramebuffer m_Framebuffer = nullptr;

		std::vector<Ref<Render::Image2D>> m_ColorAttachmentImages;
		Ref<Render::Image2D> m_DepthAttachmentImage;

		std::vector<VkClearValue> m_ClearValues;
	};
}
