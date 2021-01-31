#include <spch.h>

#include "Snow/Platform/DirectX11/DirectXFramebuffer.h"

#include "Snow/Platform/DirectX11/DirectXContext.h"

namespace Snow {
	namespace Utils {
		const DXGI_FORMAT SnowToDXGIFormat(Render::FramebufferTextureFormat format) {
			switch (format) {
			case Render::FramebufferTextureFormat::None:
				return DXGI_FORMAT_UNKNOWN;
			case Render::FramebufferTextureFormat::RGBA8:
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			case Render::FramebufferTextureFormat::RGBA16F:
				return DXGI_FORMAT_R16G16B16A16_FLOAT;
			case Render::FramebufferTextureFormat::RGBA32F:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case Render::FramebufferTextureFormat::Depth24Stencil8:
				return DXGI_FORMAT_D24_UNORM_S8_UINT;
			case Render::FramebufferTextureFormat::Depth32F:
				return DXGI_FORMAT_D32_FLOAT;

			}

			return DXGI_FORMAT_UNKNOWN;
		}

		void CreateColorAttachment(FramebufferColorAttachment& image, uint32_t samples, uint32_t width, uint32_t height, Render::FramebufferTextureFormat format) {
			auto dxContext = DirectX11RenderContext::Get();
			auto dxDevice = dxContext->GetDevice();

			bool multisampled = samples > 1;

			D3D11_TEXTURE2D_DESC textureDesc = {};

			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.SampleDesc.Count = samples;
			textureDesc.SampleDesc.Quality = 0;

			textureDesc.Format = SnowToDXGIFormat(format);

			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

			DXCheckError(dxDevice->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &image.Image));

			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = textureDesc.Format;
			rtvDesc.ViewDimension = multisampled ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
			DXCheckError(dxDevice->GetDevice()->CreateRenderTargetView(image.Image, &rtvDesc, &image.RenderTargetView));

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = multisampled ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
			if (multisampled) {
				
			}
			else {
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = 1;
			}

			DXCheckError(dxDevice->GetDevice()->CreateShaderResourceView(image.Image, &srvDesc, &image.ShaderResourceView));

		}

		void CreateDepthStencilAttachment(FramebufferDepthStencilAttachment& image, uint32_t samples, uint32_t width, uint32_t height, Render::FramebufferTextureFormat format) {
			auto dxContext = DirectX11RenderContext::Get();
			auto dxDevice = dxContext->GetDevice();

			bool multisampled = samples > 1;

			D3D11_TEXTURE2D_DESC textureDesc = {};

			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.SampleDesc.Count = samples;
			textureDesc.SampleDesc.Quality = 0;

			textureDesc.Format = SnowToDXGIFormat(format) == DXGI_FORMAT_D24_UNORM_S8_UINT ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS;

			textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

			DXCheckError(dxDevice->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &image.Image));

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = SnowToDXGIFormat(format);
			dsvDesc.ViewDimension = multisampled ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
			DXCheckError(dxDevice->GetDevice()->CreateDepthStencilView(image.Image, &dsvDesc, &image.DepthStencilView));

			
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = SnowToDXGIFormat(format) == DXGI_FORMAT_D24_UNORM_S8_UINT ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
			srvDesc.ViewDimension = multisampled ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
			

			if (multisampled) {

			}
			else {
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = 1;
			}
			DXCheckError(dxDevice->GetDevice()->CreateShaderResourceView(image.Image, &srvDesc, &image.ShaderResourceView));
			

		}

		bool IsDepthFormat(Render::FramebufferTextureFormat format) {
			switch (format) {
			case Render::FramebufferTextureFormat::Depth24Stencil8:
			case Render::FramebufferTextureFormat::Depth32F:
				return true;
			}
			return false;
		}
	}

	DirectX11Framebuffer::DirectX11Framebuffer(const Render::FramebufferSpecification& spec) :
		m_Specification(spec) {

		auto dxSwapChain = DirectX11RenderContext::Get()->GetSwapChain();
		if (spec.Width == 0 && spec.Height == 0) {
			m_Specification.Width = dxSwapChain.GetWidth();
			m_Specification.Height = dxSwapChain.GetHeight();
		}


		for (auto format : m_Specification.AttachmentList.Attachments) {
			if (!Utils::IsDepthFormat(format.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(format);
			else
				m_DepthStencilAttachmentSpecification = format;
		}

		Resize(spec.Width, spec.Height);
		m_Created = true;
	}

	void DirectX11Framebuffer::Resize(uint32_t width, uint32_t height) {
		
		
		
		if (!m_Specification.SwapChainTarget) {
			if (m_Created) {
				for (auto& attachment : m_ColorAttachments) {
					attachment.Image->Release();
					attachment.RenderTargetView->Release();
					attachment.ShaderResourceView->Release();
				}

				if (m_DepthStencilAttachment.Image) {
					m_DepthStencilAttachment.Image->Release();
					m_DepthStencilAttachment.DepthStencilView->Release();
				}
			}

			if (m_ColorAttachmentSpecifications.size()) {
				m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
				for (uint32_t i = 0; i < m_ColorAttachments.size(); i++) {
					Utils::CreateColorAttachment(m_ColorAttachments[i], m_Specification.Samples, m_Specification.Width, m_Specification.Height, m_ColorAttachmentSpecifications[i].TextureFormat);
				}
			}

			if (m_DepthStencilAttachmentSpecification.TextureFormat != Render::FramebufferTextureFormat::None) {
				Utils::CreateDepthStencilAttachment(m_DepthStencilAttachment, m_Specification.Samples, m_Specification.Width, m_Specification.Height, m_DepthStencilAttachmentSpecification.TextureFormat);
			}
		}
		else {
			auto dxContext = DirectX11RenderContext::Get();
			auto dxSwapChain = dxContext->GetSwapChain();

			m_ColorAttachments.resize(1);
			m_ColorAttachments[0].Image = dxSwapChain.GetBackBuffer().Image;
			m_ColorAttachments[0].RenderTargetView = dxSwapChain.GetBackBuffer().RenderTargetView;
		}
	}

	void DirectX11Framebuffer::Bind() const {
		auto dxContext = DirectX11RenderContext::Get();
		auto dxDevice = dxContext->GetDevice();
		auto dxSwapChain = dxContext->GetSwapChain();

		if (!m_Specification.SwapChainTarget) {
			
			std::vector<ID3D11RenderTargetView*> rtvs;
			for (auto att : m_ColorAttachments)
				rtvs.push_back(att.RenderTargetView);

			ID3D11DepthStencilView* dsv = nullptr;
			if (m_DepthStencilAttachmentSpecification.TextureFormat != Render::FramebufferTextureFormat::None)
				dsv = m_DepthStencilAttachment.DepthStencilView;

			dxDevice->GetDeviceContext()->OMSetRenderTargets(rtvs.size(), rtvs.data(), dsv);
		} 
		else {
			dxDevice->GetDeviceContext()->OMSetRenderTargets(1, &dxSwapChain.GetBackBuffer().RenderTargetView, nullptr);
		}
	}

	void DirectX11Framebuffer::BindTexture(uint32_t attachmentIndex, uint32_t slot) const {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();

		if (!m_Specification.SwapChainTarget) {
			if (attachmentIndex > m_ColorAttachments.size()) {
				// No shader resource for the depth stencil buffer
				//dxDevice->GetDeviceContext()->PSSetShaderResources(slot, 1, &m_DepthStencilAttachment.ShaderResourceView);
			}
			else {
				dxDevice->GetDeviceContext()->PSSetShaderResources(slot, 1, &m_ColorAttachments[attachmentIndex].ShaderResourceView);
			}
		}

		
	}
}