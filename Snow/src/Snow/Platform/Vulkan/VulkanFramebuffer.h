#pragma once

#include "Snow/Render/API/Framebuffer.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

namespace Snow {
	namespace Render {
		struct ImageAttachment {
			VkImage Image;
			VkDeviceMemory Memory;
			VkImageView ImageView;

			glm::vec4 ClearColor;
		};

		class VulkanFramebuffer : public Framebuffer {
		public:
			VulkanFramebuffer(const FramebufferSpecification& spec);
			virtual ~VulkanFramebuffer();

			virtual void Resize(uint32_t width, uint32_t height) override;

			virtual void Bind() const override {}
			virtual void Unbind() const override {}

			virtual uint32_t GetWidth() const override { return m_Width; }
			virtual uint32_t GetHeight() const override { return m_Height; }

			virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override {}

			virtual void* GetColorAttachmentTexture(int index = 0) const override { return 0; }
			virtual void* GetDepthAttachmentTexture() const override { return 0; }

			VkFramebuffer GetFramebufferHandle() { return m_VulkanFramebufferHandle; }
			VkRenderPass GetRenderPass() { return m_RenderPass; }

			const std::vector<ImageAttachment> GetColorAttachments() const { return m_ColorAttachments; }
			const ImageAttachment GetDepthAttachment() const { return m_DepthAttachment; }

			const FramebufferTextureFormat GetDepthAttachmentFormat() const { return m_DepthAttachmentFormat; }

			const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
		private:


			FramebufferSpecification m_Specification;

			VkFramebuffer m_VulkanFramebufferHandle = VK_NULL_HANDLE;

			VkRenderPass m_RenderPass;

			

			std::vector<ImageAttachment> m_ColorAttachments;
			ImageAttachment m_DepthAttachment;

			std::vector<Render::FramebufferTextureFormat> m_ColorAttachmentFormats;
			Render::FramebufferTextureFormat m_DepthAttachmentFormat = Render::FramebufferTextureFormat::None;

			std::vector<VkAttachmentReference> m_ColorAttachmentReferences;
			VkAttachmentReference m_DepthAttachmentReference;
			std::vector<VkAttachmentDescription> m_ColorAttachmentDescriptions;
			VkAttachmentDescription m_DepthAttachmentDescription;

			uint32_t m_Width = 0, m_Height = 0;
		};
	}
}
