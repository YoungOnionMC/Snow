#pragma once

#include "Snow/Platform/DirectX11/DirectXCommon.h"

#include "Snow/Platform/DirectX11/DirectXDevice.h"

#include "Snow/Render/SwapChain.h"

namespace Snow {
	class DirectX11SwapChain : public Render::SwapChain {
		struct SwapChainFramebuffer;
	public:
		DirectX11SwapChain() = default;

		void Init(Ref<DirectX11Device>& device);

		void Create(uint32_t width, uint32_t height, HWND windowHandle);

		void SwapBuffers() override;

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		SwapChainFramebuffer& GetBackBuffer() { return m_BackBuffer; }

		const Render::SwapChainSpecification& GetSpecification() const { return m_Specification; }
	private:
		void CreateBackBuffer();

		Ref<DirectX11Device> m_Device;

		Render::SwapChainSpecification m_Specification;

		uint32_t m_Width, m_Height;
		DXGI_FORMAT m_ColorFormat;

		struct SwapChainFramebuffer {
			ID3D11Texture2D* Image;
			ID3D11RenderTargetView* RenderTargetView;
		};

		SwapChainFramebuffer m_BackBuffer;

		uint32_t m_MinFramebuffers, m_MaxFramebuffers;

		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
	};
}