#pragma once

#include "Snow/Render/UniformBuffer.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include "Snow/Platform/Vulkan/VulkanAllocator.h"

namespace Snow {
	class VulkanUniformBuffer : public Render::UniformBuffer {
	public:
		VulkanUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~VulkanUniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
		virtual void RTSetData(const void* data, uint32_t size, uint32_t offset = 0) override;

		const VkDescriptorBufferInfo& GetDescriptorBufferInfo() const { return m_DescriptorInfo; }

		virtual uint32_t GetBinding() const override { return m_Binding; }
	private:
		void RTInvalidate();
	private:
		VmaAllocation m_MemoryAlloc = nullptr;
		VkBuffer m_Buffer;
		VkDescriptorBufferInfo m_DescriptorInfo{};
		uint32_t m_Size = 0;
		uint32_t m_Binding = 0;
		std::string m_Name;
		VkShaderStageFlagBits m_ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;

		uint8_t* m_LocalStorage = nullptr;
	};
}