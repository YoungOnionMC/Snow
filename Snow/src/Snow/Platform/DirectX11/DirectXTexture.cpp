#include <spch.h>
#include "Snow/Platform/DirectX11/DirectXTexture.h"

#include "Snow/Platform/DirectX11/DirectXContext.h"

#include <stb_image.h>

namespace Snow {
	static DXGI_FORMAT SnowTextureFormatToDXGI(Render::API::TextureFormat format) {
		switch (format) {
		case Render::API::TextureFormat::RGB:	return DXGI_FORMAT_R8G8B8A8_UNORM;
		case Render::API::TextureFormat::RGBA:	return DXGI_FORMAT_R8G8B8A8_UNORM;
		case Render::API::TextureFormat::Float16:	return DXGI_FORMAT_R16G16B16A16_FLOAT;
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	static uint32_t CalculateMips(uint32_t width, uint32_t height) {
		int levels = 1;
		while ((width | height) >> levels)
			levels++;
		return levels;
	}

	DirectX11Texture2D::DirectX11Texture2D(Render::API::TextureFormat format, uint32_t width, uint32_t height, Render::API::TextureWrap wrap) {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();

		m_Width = width;
		m_Height = height;
		m_Format = format;
		
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = nullptr;
		initData.SysMemPitch = 4 * m_Width;
		initData.SysMemSlicePitch = 4 * m_Width * m_Height;

		bool generateMips = false;

		DXGI_FORMAT dxFormat = SnowTextureFormatToDXGI(format);
		uint32_t formatSupport = 0;
		DXCheckError(dxDevice->GetDevice()->CheckFormatSupport(dxFormat, &formatSupport));

		m_TextureDesc.Width = m_Width;
		m_TextureDesc.Height = m_Height;
		m_TextureDesc.MipLevels = generateMips ? CalculateMips(m_Width, m_Height) : 1;
		m_TextureDesc.ArraySize = 1;
		m_TextureDesc.Format = dxFormat;
		m_TextureDesc.SampleDesc.Count = 1;
		m_TextureDesc.SampleDesc.Quality = 0;
		m_TextureDesc.Usage = generateMips ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
		m_TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (generateMips)
			m_TextureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		m_TextureDesc.CPUAccessFlags = m_TextureDesc.Usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		m_TextureDesc.MiscFlags = generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		DXCheckError(dxDevice->GetDevice()->CreateTexture2D(&m_TextureDesc, NULL, &m_Texture));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = m_TextureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_TextureDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		DXCheckError(dxDevice->GetDevice()->CreateShaderResourceView(m_Texture, &srvDesc, &m_ResourceView));
		if (generateMips) {
			dxDevice->GetDeviceContext()->UpdateSubresource(m_Texture, 0, nullptr, initData.pSysMem, initData.SysMemPitch, initData.SysMemSlicePitch);
			dxDevice->GetDeviceContext()->GenerateMips(m_ResourceView);
		}
	}

	DirectX11Texture2D::DirectX11Texture2D(const std::string& path, bool srgb) {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();

		int width, height, channels;
		m_ImageData.Data = stbi_load(path.c_str(), &width, &height, &channels, srgb ? STBI_rgb : STBI_rgb_alpha);

		m_Width = width;
		m_Height = height;
		m_Format = channels == 4 ? Render::API::TextureFormat::RGBA : Render::API::TextureFormat::RGB;

		if (!m_ImageData.Data)
			return;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = m_ImageData.Data;
		initData.SysMemPitch = 4 * m_Width;
		initData.SysMemSlicePitch = 4 * m_Width * m_Height;

		DXGI_FORMAT dxFormat = SnowTextureFormatToDXGI(m_Format);
		uint32_t formatSupport = 0;
		DXCheckError(dxDevice->GetDevice()->CheckFormatSupport(dxFormat, &formatSupport));

		bool generateMips = true;

		m_TextureDesc.Width = m_Width;
		m_TextureDesc.Height = m_Height;
		m_TextureDesc.MipLevels = generateMips ? CalculateMips(m_Width, m_Height) : 1;
		m_TextureDesc.ArraySize = 1;
		m_TextureDesc.Format = dxFormat;
		m_TextureDesc.SampleDesc.Count = 1;
		m_TextureDesc.SampleDesc.Quality = 0;
		m_TextureDesc.Usage = generateMips ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
		m_TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (generateMips)
			m_TextureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		m_TextureDesc.CPUAccessFlags = m_TextureDesc.Usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		m_TextureDesc.MiscFlags = generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		DXCheckError(dxDevice->GetDevice()->CreateTexture2D(&m_TextureDesc, NULL, &m_Texture));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = m_TextureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_TextureDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		DXCheckError(dxDevice->GetDevice()->CreateShaderResourceView(m_Texture, &srvDesc, &m_ResourceView));
		if (generateMips) {
			dxDevice->GetDeviceContext()->UpdateSubresource(m_Texture, 0, nullptr, initData.pSysMem, initData.SysMemPitch, initData.SysMemSlicePitch);
			dxDevice->GetDeviceContext()->GenerateMips(m_ResourceView);
		}
	}

	void DirectX11Texture2D::ResizeBuffer(uint32_t width, uint32_t height) {
		SNOW_CORE_ASSERT(m_Locked, "Trying to write to texture without it being locked");
		m_ImageData.Allocate(width * height * Render::API::Texture::GetBPP(m_Format));
		m_ImageData.ZeroInitialize();
	}

	void DirectX11Texture2D::Lock() {
		m_Locked = true;
	}

	void DirectX11Texture2D::Unlock() {
		m_Locked = false;
	}

	void DirectX11Texture2D::SetData(void* data, uint32_t size) {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;
		initData.SysMemPitch = 4 * m_Width;
		initData.SysMemSlicePitch = 4 * m_Width * m_Height;

		dxDevice->GetDeviceContext()->UpdateSubresource(m_Texture, 0, NULL, initData.pSysMem, initData.SysMemPitch, initData.SysMemSlicePitch);
		dxDevice->GetDeviceContext()->GenerateMips(m_ResourceView);
	}

	void DirectX11Texture2D::Bind(uint32_t slot) const {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();
		dxDevice->GetDeviceContext()->PSSetShaderResources(slot, 1, &m_ResourceView);
	}
}