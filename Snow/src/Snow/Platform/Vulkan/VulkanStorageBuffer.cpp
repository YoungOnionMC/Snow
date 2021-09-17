#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanStorageBuffer.h"

#include "Snow/Render/Renderer.h"

namespace Snow {
	VulkanStorageBuffer::VulkanStorageBuffer(uint32_t size, uint32_t binding) :
		m_Size(size), m_Binding(binding) {
		Ref<VulkanStorageBuffer> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			instance->RTInvalidate();
		});
	}

	VulkanStorageBuffer::~VulkanStorageBuffer() {
		Release();
	}

	void VulkanStorageBuffer::Release() {
		if (!m_MemoryAlloc)
			return;

		Render::Renderer::SubmitResourceFree([buffer = m_Buffer, memoryAlloc = m_MemoryAlloc]() {
			VulkanAllocator allocator("StorageBuffer");
			allocator.DestroyBuffer(buffer, memoryAlloc);
		});

		m_Buffer = nullptr;
		m_MemoryAlloc = nullptr;
	}

	void VulkanStorageBuffer::RTInvalidate() {
		Release();

		VkBufferCreateInfo bufferCI = {};
		bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCI.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		bufferCI.size = m_Size;

		VulkanAllocator allocator("StorageBuffer");
		m_MemoryAlloc = allocator.AllocateBuffer(bufferCI, VMA_MEMORY_USAGE_GPU_ONLY, m_Buffer);

		m_DescriptorInfo.buffer = m_Buffer;
		m_DescriptorInfo.offset = 0;
		m_DescriptorInfo.range = m_Size;
	}

	void VulkanStorageBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
		memcpy(m_LocalStorage, data, size);

		Ref<VulkanStorageBuffer> instance = this;
		Render::Renderer::Submit([instance, size, offset]() mutable {
			instance->RTSetData(instance->m_LocalStorage, size, offset);
		});
	}

	void VulkanStorageBuffer::RTSetData(const void* data, uint32_t size, uint32_t offset) {
		VulkanAllocator allocator("StorageBuffer");
		uint8_t* pData = allocator.MapMemory<uint8_t>(m_MemoryAlloc);
		memcpy(pData, (uint8_t*)data + offset, size);
		allocator.UnmapMemory(m_MemoryAlloc);
	}

	void VulkanStorageBuffer::Resize(uint32_t size) {
		m_Size = size;
		Ref<VulkanStorageBuffer> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			instance->RTInvalidate();
		});
	}
}