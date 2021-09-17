#pragma once

#include "Snow/Render/Buffer.h"

#include "Snow/Core/Buffer.h"

#include "Snow/Platform/Vulkan/VulkanAllocator.h"

namespace Snow {
	class VulkanVertexBuffer : public Render::VertexBuffer {
	public:
		VulkanVertexBuffer(uint32_t size);
		VulkanVertexBuffer(void* data, uint32_t size);

		virtual ~VulkanVertexBuffer();

		void Bind() const override {}
		void Unbind() const override {}

		void SetData(void* data, uint32_t size, uint32_t offset = 0) override;
		void RTSetData(void* data, uint32_t size, uint32_t offset = 0) override;

		VkBuffer GetBuffer() const { return m_VulkanBuffer; }
	private:
		uint32_t m_Size = 0;
		Buffer m_LocalData;

		VkBuffer m_VulkanBuffer;
		VmaAllocation m_MemoryAllocation;
	};

	class VulkanIndexBuffer : public Render::IndexBuffer {
	public:
		VulkanIndexBuffer(uint32_t size);
		VulkanIndexBuffer(void* data, uint32_t size);

		virtual ~VulkanIndexBuffer() {}

		void Bind() const override {}
		void Unbind() const override {}

		void SetData(void* data, uint32_t size, uint32_t offset = 0) override;
		void RTSetData(void* data, uint32_t size, uint32_t offset = 0) override;

		VkBuffer GetBuffer() const { return m_VulkanBuffer; }

		uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_Size = 0;
		uint32_t m_Count = 0;
		Buffer m_LocalData;

		VkBuffer m_VulkanBuffer;
		VmaAllocation m_MemoryAllocation;
	};
}