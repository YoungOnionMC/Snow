#pragma once

#include "Snow/Render/Framebuffer.h"

#include "Snow/Platform/DirectX11/DirectXCommon.h"

namespace Snow {
	struct FramebufferColorAttachment {
		ID3D11Texture2D* Image = nullptr;
		ID3D11RenderTargetView* RenderTargetView = nullptr;
		ID3D11ShaderResourceView* ShaderResourceView = nullptr;
	};

	struct FramebufferDepthStencilAttachment {
		ID3D11Texture2D* Image = nullptr;
		ID3D11DepthStencilView* DepthStencilView = nullptr;
		ID3D11ShaderResourceView* ShaderResourceView = nullptr;
	};

	class DirectX11Framebuffer : public Render::Framebuffer {
	public:
		DirectX11Framebuffer(const Render::FramebufferSpecification& spec);


		void Bind() const override;
		void Unbind() const override {}
		void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override;

		virtual void* GetColorAttachmentTexture(int index = 0) const override { return m_ColorAttachments[index].ShaderResourceView; }
		virtual void* GetDepthAttachmentTexture() const override { return m_DepthStencilAttachment.Image; }

		const Render::FramebufferSpecification& GetSpecification() const { return m_Specification; }

		const std::vector<FramebufferColorAttachment>& GetColorAttachments() const { return m_ColorAttachments; }
		const FramebufferDepthStencilAttachment& GetDepthStencilAttachment() const { return m_DepthStencilAttachment; }
		Render::ImageFormat GetDepthStencilFormat() { return m_DepthStencilAttachmentSpecification.Format; }

	private:

		Render::FramebufferSpecification m_Specification;


		std::vector<FramebufferColorAttachment> m_ColorAttachments;
		std::vector<Render::FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferDepthStencilAttachment m_DepthStencilAttachment;
		Render::FramebufferTextureSpecification m_DepthStencilAttachmentSpecification;


		bool m_Created = false;
		uint32_t m_Width, m_Height;
	};
}