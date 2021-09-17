#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanBuffer.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"

namespace Snow {
	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size) :
		m_Size(size) {

		Ref<VulkanVertexBuffer> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			VkBufferCreateInfo vertexBufferInfo = {};
			vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferInfo.size = instance->m_Size;
			vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			VulkanAllocator allocator("VertexBuffer");

			instance->m_MemoryAllocation = allocator.AllocateBuffer(vertexBufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, instance->m_VulkanBuffer);
			instance->m_LocalData.Allocate(instance->m_Size);
		});
	}

	VulkanVertexBuffer::VulkanVertexBuffer(void* data, uint32_t size) :
		m_Size(size) {

		m_LocalData = Buffer::Copy(data, size);

		Ref<VulkanVertexBuffer> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			auto device = VulkanContext::GetCurrentDevice();
			VulkanAllocator allocator("VertexBuffer");
#define USE_STAGING 1
#if USE_STAGING
			VkBufferCreateInfo vertexBufferInfo = {};
			vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferInfo.size = instance->m_Size;
			vertexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			vertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			VkBuffer stagingBuffer;

			VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(vertexBufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

			uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAllocation);
			memcpy(destData, instance->m_LocalData.Data, instance->m_LocalData.Size);
			allocator.UnmapMemory(stagingBufferAllocation);

			VkBufferCreateInfo vertexBufferCreateInfo = {};
			vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferCreateInfo.size = instance->m_Size;
			vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			instance->m_MemoryAllocation = allocator.AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, instance->m_VulkanBuffer);

			VkCommandBuffer copyCmd = device->GetCommandBuffer(true);

			VkBufferCopy copyRegion = {};
			copyRegion.size = instance->m_LocalData.Size;
			vkCmdCopyBuffer(copyCmd, stagingBuffer, instance->m_VulkanBuffer, 1, &copyRegion);
			device->FlushCommandBuffer(copyCmd);

			allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
#else
			VkBufferCreateInfo vertexBufferCreateInfo = {};
			vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferCreateInfo.size = instance->m_Size;
			vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			auto bufferAlloc = allocator.AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, instance->m_VulkanBuffer);

			void* dstBuffer = allocator.MapMemory<void>(bufferAlloc);
			memcpy(dstBuffer, instance->m_LocalData.Data, instance->m_Size);
			allocator.UnmapMemory(bufferAlloc);
#endif
		});
	}

	VulkanVertexBuffer::~VulkanVertexBuffer() {
		VkBuffer buffer = m_VulkanBuffer;
		VmaAllocation allocation = m_MemoryAllocation;
		Render::Renderer::SubmitResourceFree([buffer, allocation]() {
			VulkanAllocator allocator("VertexBuffer");
			allocator.DestroyBuffer(buffer, allocation);
		});
	}

	void VulkanVertexBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
		SNOW_CORE_ASSERT(size <= m_LocalData.Size);
		memcpy(m_LocalData.Data, (uint8_t*)data + offset, size);
		Ref<VulkanVertexBuffer> instance = this;
		Render::Renderer::Submit([instance, size, offset]() mutable {
			instance->RTSetData(instance->m_LocalData.Data, size, offset);
		});
	}

	void VulkanVertexBuffer::RTSetData(void* data, uint32_t size, uint32_t offset) {
		VulkanAllocator allocator("VulkanVertexBuffer");
		uint8_t* pData = allocator.MapMemory<uint8_t>(m_MemoryAllocation);
		memcpy(pData, (uint8_t*)data + offset, size);
		allocator.UnmapMemory(m_MemoryAllocation);
	}

	// INDEX BUFFER
	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t size) :
		m_Size(size) {

		Ref<VulkanIndexBuffer> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			VkBufferCreateInfo indexBufferInfo = {};
			indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			indexBufferInfo.size = instance->m_Size;
			indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

			VulkanAllocator allocator("IndexBuffer");

			instance->m_MemoryAllocation = allocator.AllocateBuffer(indexBufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, instance->m_VulkanBuffer);
			instance->m_LocalData.Allocate(instance->m_Size);
		});
	}

	VulkanIndexBuffer::VulkanIndexBuffer(void* data, uint32_t size) :
		m_Count(size / sizeof(uint32_t)), m_Size(size) {

		m_LocalData = Buffer::Copy(data, size);

		Ref<VulkanIndexBuffer> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			auto device = VulkanContext::GetCurrentDevice();
			VulkanAllocator allocator("IndexBuffer");
#define USE_STAGING 1
#if USE_STAGING
			VkBufferCreateInfo indexBufferInfo = {};
			indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			indexBufferInfo.size = instance->m_Size;
			indexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			indexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			VkBuffer stagingBuffer;

			VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(indexBufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

			uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAllocation);
			memcpy(destData, instance->m_LocalData.Data, instance->m_LocalData.Size);
			allocator.UnmapMemory(stagingBufferAllocation);

			VkBufferCreateInfo indexBufferCreateInfo = {};
			indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			indexBufferCreateInfo.size = instance->m_Size;
			indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			instance->m_MemoryAllocation = allocator.AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, instance->m_VulkanBuffer);

			VkCommandBuffer copyCmd = device->GetCommandBuffer(true);

			VkBufferCopy copyRegion = {};
			copyRegion.size = instance->m_LocalData.Size;
			vkCmdCopyBuffer(copyCmd, stagingBuffer, instance->m_VulkanBuffer, 1, &copyRegion);
			device->FlushCommandBuffer(copyCmd);

			allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
#else
			VkBufferCreateInfo indexBufferCreateInfo = {};
			indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			indexBufferCreateInfo.size = instance->m_Size;
			indexBufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

			auto bufferAlloc = allocator.AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, instance->m_VulkanBuffer);

			void* dstBuffer = allocator.MapMemory<void>(bufferAlloc);
			memcpy(dstBuffer, instance->m_LocalData.Data, instance->m_Size);
			allocator.UnmapMemory(bufferAlloc);
#endif
			});
	}

	void VulkanIndexBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
		SNOW_CORE_ASSERT(size <= m_LocalData.Size);
		memcpy(m_LocalData.Data, (uint8_t*)data + offset, size);
		Ref<VulkanIndexBuffer> instance = this;
		Render::Renderer::Submit([instance, size, offset]() mutable {
			instance->RTSetData(instance->m_LocalData.Data, size, offset);
			});
	}

	void VulkanIndexBuffer::RTSetData(void* data, uint32_t size, uint32_t offset) {
		VulkanAllocator allocator("VulkanIndexBuffer");
		uint8_t* pData = allocator.MapMemory<uint8_t>(m_MemoryAllocation);
		memcpy(pData, (uint8_t*)data + offset, size);
		allocator.UnmapMemory(m_MemoryAllocation);
	}
}