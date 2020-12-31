#include <spch.h>
#include "Snow/Platform/DirectX11/DirectXSwapChain.h"

#include "Snow/Platform/DirectX11/DirectXContext.h"

#include "Snow/Core/Application.h"

#if defined(SNOW_WINDOW_GLFW)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>
#endif

namespace Snow {
	void DirectX11SwapChain::Init(Ref<DirectX11Device>& device) {
		m_Device = device;
	}

	void DirectX11SwapChain::Create(uint32_t width, uint32_t height, HWND windowHandle) {

		m_Width = width;
		m_Height = height;

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.OutputWindow = windowHandle;
		swapChainDesc.Windowed = true;

		DXCheckError(m_Device->GetDXGIFactory()->CreateSwapChain(m_Device->GetDevice().Get(), &swapChainDesc, &m_SwapChain));

		m_Device->GetDXGIFactory()->MakeWindowAssociation(windowHandle, DXGI_MWA_NO_WINDOW_CHANGES);


		m_MaxFramebuffers = DXGI_MAX_SWAP_CHAIN_BUFFERS;

		CreateBackBuffer();

		D3D11_VIEWPORT viewport = {};
		viewport.Width = m_Width;
		viewport.Height = m_Height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		m_Device->GetDeviceContext()->RSSetViewports(1, &viewport);
	}

	void DirectX11SwapChain::CreateBackBuffer() {

		m_ColorFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		D3D11_TEXTURE2D_DESC desc = {};

		desc.Width = m_Width;
		desc.Height = m_Height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;

		if (m_ColorFormat != DXGI_FORMAT_UNKNOWN) {
			desc.Format = m_ColorFormat;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

			DXCheckError(m_SwapChain->GetBuffer(0, __uuidof(m_BackBuffer.Image), (void**)&m_BackBuffer.Image));

			//m_Device->GetDevice()->CreateTexture2D(&desc, nullptr, &m_BackBuffer.Image));

			//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			//rtvDesc.Format = desc.Format;
			//rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			DXCheckError(m_Device->GetDevice()->CreateRenderTargetView(m_BackBuffer.Image, nullptr, &m_BackBuffer.RenderTargetView));
		}
	}

	void DirectX11SwapChain::SwapBuffers() {
		m_SwapChain->Present(0, 0);
	}
}