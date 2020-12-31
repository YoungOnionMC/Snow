#include <spch.h>
#include "Snow/Platform/DirectX11/DirectXRenderCommand.h"

#include "Snow/Platform/DirectX11/DirectXContext.h"

namespace Snow {
	static D3D11_PRIMITIVE_TOPOLOGY GetD3DTopology(Render::PrimitiveType type) {
		switch (type) {
		case Render::PrimitiveType::Line:	return D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		case Render::PrimitiveType::Triangle:	return D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
		return D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}

	void DirectX11RenderCommand::DrawIndexed(uint32_t count, Render::PrimitiveType type) {
		auto dxContext = DirectX11RenderContext::Get();
		auto dxDevice = dxContext->GetDevice();

		dxDevice->GetDeviceContext()->IASetPrimitiveTopology(GetD3DTopology(type));
		dxDevice->GetDeviceContext()->DrawIndexed(count, 0, 0);
	}

	void DirectX11RenderCommand::SwapBuffers() {
		auto dxContext = DirectX11RenderContext::Get();
		auto dxSwapChain = dxContext->GetSwapChain();

		dxSwapChain.SwapBuffers();
	}
}
