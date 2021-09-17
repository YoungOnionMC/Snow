#pragma once

#include "Snow/Render/Buffer.h"

#include "Snow/Platform/DirectX11/DirectXCommon.h"

#include "Snow/Core/Buffer.h"

namespace Snow {
	class DirectX11VertexBuffer : public Render::VertexBuffer {
	public:
		DirectX11VertexBuffer(void* data, uint32_t size);

		void Bind() const override;
		void Unbind() const override;

		void SetData(void* data, uint32_t size) override;

		void* Map();
		void Unmap();

		void ResizeBuffer(uint32_t size);

	private:
		D3D11_BUFFER_DESC m_BufferDesc;
		ID3D11Buffer* m_BufferHandle;

		D3D11_MAPPED_SUBRESOURCE m_MappedResource;

		Buffer m_LocalBuffer;

		bool m_Mapped = false;
	};

	class DirectX11IndexBuffer : public Render::IndexBuffer {
	public:
		DirectX11IndexBuffer(void* data, uint32_t size);

		void Bind() const override;
		void Unbind() const override;

		void SetData(void* data, uint32_t size) override;

		void* Map();
		void Unmap();

		uint32_t GetCount() const override { return m_Count; }

		void ResizeBuffer(uint32_t size);

	private:
		D3D11_BUFFER_DESC m_BufferDesc;
		ID3D11Buffer* m_BufferHandle;

		D3D11_MAPPED_SUBRESOURCE m_MappedResource;

		Buffer m_LocalBuffer;

		uint32_t m_Count = 0;

		bool m_Mapped = false;
	};
}