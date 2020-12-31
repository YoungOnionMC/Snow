#include <spch.h>
#include "Snow/Platform/DirectX11/DirectXRenderPass.h"

#include "Snow/Platform/DirectX11/DirectXContext.h"

#include "Snow/Platform/DirectX11/DirectXFramebuffer.h"

namespace Snow {
	DirectX11RenderPass::DirectX11RenderPass(const Render::RenderPassSpecification& spec) :
		m_Specification(spec) {

	}

	void DirectX11RenderPass::BeginPass() {
		auto dxContext = DirectX11RenderContext::Get();
		auto dxDevice = dxContext->GetDevice();

		auto dxFramebuffer = m_Specification.TargetFramebuffer.As<DirectX11Framebuffer>();


		for (uint32_t i = 0; i < dxFramebuffer->GetColorAttachments().size(); i++) {
			glm::vec4 c = dxFramebuffer->GetSpecification().ClearColor;
			FLOAT color[4] = { c.r, c.g, c.b, c.a };
			dxDevice->GetDeviceContext()->ClearRenderTargetView(dxFramebuffer->GetColorAttachments()[i].RenderTargetView, color);
		}

		if(dxFramebuffer->GetDepthStencilFormat() != Render::FramebufferTextureFormat::None)
			dxDevice->GetDeviceContext()->ClearDepthStencilView(dxFramebuffer->GetDepthStencilAttachment().DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void DirectX11RenderPass::EndPass() {

	}
}