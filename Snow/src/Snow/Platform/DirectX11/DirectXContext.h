#pragma once

#include "Snow/Render/RenderContext.h"

#include "Snow/Platform/DirectX11/DirectXCommon.h"

#include "Snow/Platform/DirectX11/DirectXDevice.h"
#include "Snow/Platform/DirectX11/DirectXSwapChain.h"

#include "Snow/Render/Renderer.h"

namespace Snow {
	class DirectX11RenderContext : public Render::Context {
	public:
		DirectX11RenderContext(const Render::ContextSpecification& spec);

		const Ref<DirectX11Device>& GetDevice() { return m_Device; }
		const DirectX11SwapChain& GetSwapChain() { return m_SwapChain; }

		static Ref<DirectX11RenderContext> Get() { return Ref<DirectX11RenderContext>(Render::Renderer::GetContext()); }
		static Ref<DirectX11Device> GetCurrentDevice() { return Get()->GetDevice(); }

		const Render::ContextSpecification& GetSpecification() const { return m_Specification; }
	private:
		Render::ContextSpecification m_Specification;

		Ref<DirectX11Device> m_Device;

		DirectX11SwapChain m_SwapChain;
	};
}
