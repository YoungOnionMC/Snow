#pragma once

#include "Snow/Render/StorageBuffer.h"
#include "Snow/Platform/Vulkan/VulkanAllocator.h"

namespace Snow {
	class VulkanStorageBuffer : public Render::StorageBuffer {
	public:
		VulkanStorageBuffer(uint32_t size, uint32_t binding);
		virtual ~VulkanStorageBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0);
		virtual void RTSetData(const void* data, uint32_t size, uint32_t offset = 0);
		virtual void Resize(uint32_t size);

		virtual uint32_t GetBinding() const override { return m_Binding; }

		const VkDescriptorBufferInfo& GetDescriptorBufferInfo() const { return m_DescriptorInfo; }

	private:
		void Release();
		void RTInvalidate();

		VmaAllocation m_MemoryAlloc = nullptr;
		VkBuffer m_Buffer = {};
		VkDescriptorBufferInfo m_DescriptorInfo = {};
		uint32_t m_Size = 0;
		uint32_t m_Binding = 0;
		std::string m_Name;
		VkShaderStageFlagBits m_ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;

		uint8_t* m_LocalStorage = nullptr;
	};
}