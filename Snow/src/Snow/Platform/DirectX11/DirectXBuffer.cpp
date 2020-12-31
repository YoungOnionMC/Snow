#include <spch.h>
#include "Snow/Platform/DirectX11/DirectXBuffer.h"

#include "Snow/Platform/DirectX11/DirectXContext.h"

namespace Snow {
	DirectX11VertexBuffer::DirectX11VertexBuffer(void* data, uint32_t size) {
		m_BufferDesc = {};
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.ByteWidth = size;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_BufferDesc.MiscFlags = 0;

		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();

		D3D11_SUBRESOURCE_DATA* bufferData = nullptr;
		D3D11_SUBRESOURCE_DATA bufData = {};
		if (data) {
			bufData.pSysMem = data;
			bufData.SysMemPitch = 0;
			bufData.SysMemSlicePitch = 0;
			bufferData = &bufData;

			m_LocalBuffer.Allocate(size);
			m_LocalBuffer.Write(data, size);
		}

		DXCheckError(dxDevice->GetDevice()->CreateBuffer(&m_BufferDesc, bufferData, &m_BufferHandle));
	}

	void DirectX11VertexBuffer::Bind() const {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();
		SNOW_CORE_TRACE("Mapped {0}", m_Mapped);
		uint32_t offset = 0;
		uint32_t stride = 40;
		dxDevice->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_BufferHandle, &stride, &offset);
	}

	void DirectX11VertexBuffer::Unbind() const {

	}

	void* DirectX11VertexBuffer::Map() {
		SNOW_ASSERT(!m_Mapped);
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();
		DXCheckError(dxDevice->GetDeviceContext()->Map(m_BufferHandle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_MappedResource));
		m_Mapped = true;
		return m_MappedResource.pData;
	}

	void DirectX11VertexBuffer::Unmap() {
		SNOW_ASSERT(m_Mapped);
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();
		dxDevice->GetDeviceContext()->Unmap(m_BufferHandle, NULL);
		m_Mapped = false;
	}

	void DirectX11VertexBuffer::ResizeBuffer(uint32_t size) {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();
		if (m_BufferHandle) {
			m_BufferHandle->Release();
		}
		m_BufferDesc.ByteWidth = size;

		Buffer tempBuffer = m_LocalBuffer;
		m_LocalBuffer.Allocate(size);
		if(tempBuffer.Data)
			m_LocalBuffer.Write(tempBuffer.Data, tempBuffer.Size);

		D3D11_SUBRESOURCE_DATA* bufferData = nullptr;
		D3D11_SUBRESOURCE_DATA bufData = {};
		if (m_LocalBuffer.Data) {
			bufData.pSysMem = m_LocalBuffer.Data;
			bufData.SysMemPitch = 0;
			bufData.SysMemSlicePitch = 0;
			bufferData = &bufData;
		}

		DXCheckError(dxDevice->GetDevice()->CreateBuffer(&m_BufferDesc, bufferData, &m_BufferHandle));
	}

	void DirectX11VertexBuffer::SetData(void* data, uint32_t size) {

		if (m_LocalBuffer.Size < size)
			ResizeBuffer(size);

		m_LocalBuffer.Write(data, size);

		void* mappedData = Map();
		memcpy(mappedData, data, size);
		Unmap();
	}



	DirectX11IndexBuffer::DirectX11IndexBuffer(void* data, uint32_t size) :
		m_Count(size / sizeof(uint32_t)) {
		m_BufferDesc = {};
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_BufferDesc.ByteWidth = size;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_BufferDesc.MiscFlags = 0;

		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();

		D3D11_SUBRESOURCE_DATA* bufferData = nullptr;
		D3D11_SUBRESOURCE_DATA bufData = {};
		if (data) {
			bufData.pSysMem = data;
			bufData.SysMemPitch = 0;
			bufData.SysMemSlicePitch = 0;
			bufferData = &bufData;

			m_LocalBuffer.Allocate(size);
			m_LocalBuffer.Write(data, size);
		}

		DXCheckError(dxDevice->GetDevice()->CreateBuffer(&m_BufferDesc, bufferData, &m_BufferHandle));
	}

	void DirectX11IndexBuffer::Bind() const {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();
		dxDevice->GetDeviceContext()->IASetIndexBuffer(m_BufferHandle, DXGI_FORMAT_R32_UINT, 0);
	}

	void DirectX11IndexBuffer::Unbind() const {

	}

	void* DirectX11IndexBuffer::Map() {
		SNOW_ASSERT(!m_Mapped);
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();
		DXCheckError(dxDevice->GetDeviceContext()->Map(m_BufferHandle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_MappedResource));
		m_Mapped = true;
		return m_MappedResource.pData;
	}

	void DirectX11IndexBuffer::Unmap() {
		SNOW_ASSERT(m_Mapped);
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();
		dxDevice->GetDeviceContext()->Unmap(m_BufferHandle, NULL);
		m_Mapped = false;
	}

	void DirectX11IndexBuffer::ResizeBuffer(uint32_t size) {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();
		if (m_BufferHandle) {
			m_BufferHandle->Release();
		}
		m_BufferDesc.ByteWidth = size;

		Buffer tempBuffer = m_LocalBuffer;
		m_LocalBuffer.Allocate(size);
		if (tempBuffer.Data)
			m_LocalBuffer.Write(tempBuffer.Data, tempBuffer.Size);

		m_Count = size / sizeof(uint32_t);

		D3D11_SUBRESOURCE_DATA* bufferData = nullptr;
		D3D11_SUBRESOURCE_DATA bufData = {};
		if (m_LocalBuffer.Data) {
			bufData.pSysMem = m_LocalBuffer.Data;
			bufData.SysMemPitch = 0;
			bufData.SysMemSlicePitch = 0;
			bufferData = &bufData;
		}

		DXCheckError(dxDevice->GetDevice()->CreateBuffer(&m_BufferDesc, bufferData, &m_BufferHandle));
	}

	void DirectX11IndexBuffer::SetData(void* data, uint32_t size) {

		if (m_LocalBuffer.Size < size)
			ResizeBuffer(size);

		m_LocalBuffer.Write(data, size);

		void* mappedData = Map();
		memcpy(mappedData, data, size);
		Unmap();
	}
}