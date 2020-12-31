#pragma once

#include "Snow/Render/API/Framebuffer.h"

namespace Snow {
	class OpenGLFramebuffer : public Render::Framebuffer {
	public:
		OpenGLFramebuffer(const Render::FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void* GetColorAttachmentTexture(int index) const override { return reinterpret_cast<void*>(m_ColorAttachments[index]); }
		virtual void* GetDepthAttachmentTexture() const override { return reinterpret_cast<void*>(m_DepthAttachment); }

		virtual const Render::FramebufferSpecification& GetSpecification() const override { return m_Specification; }



	private:
		Render::FramebufferSpecification m_Specification;
		uint32_t m_RendererID = 0;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;

		std::vector<Render::FramebufferTextureFormat> m_ColorAttachmentFormats;
		Render::FramebufferTextureFormat m_DepthAttachmentFormat = Render::FramebufferTextureFormat::None;

		uint32_t m_Width = 0, m_Height = 0;

	};
}