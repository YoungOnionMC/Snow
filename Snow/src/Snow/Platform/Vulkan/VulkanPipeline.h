#pragma once

#include "Snow/Render/Pipeline.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include "Snow/Render/Shader/ShaderUniform.h"

#include <spirv_cross.hpp>

namespace Snow {
	namespace Render {
		class VulkanPipeline : public Pipeline {
		public:
			VulkanPipeline(const PipelineSpecification& spec);

			void Bind() override;

			void SetUniformBufferData(const std::string& name, void* data, uint32_t size) override {}
			void SetUniformBufferData(uint32_t bindingPoint, void* data, uint32_t size) override {}

			const PipelineSpecification& GetSpecification() const override { return m_Specification; }
		private:
			void Invalidate();

			void Reflect(VkGraphicsPipelineCreateInfo& pipelineCreateInfo);

			static VkFormat SPIRVTypeToVKFormat(spirv_cross::SPIRType type);
			static VkShaderStageFlagBits SnowShaderTypeToVKShaderStage(ShaderType type);

			PipelineSpecification m_Specification;

			VkGraphicsPipelineCreateInfo m_GraphicsPipelineCreateInfo;
			VkPipeline m_VulkanPipeline;

			struct VulkanUniformBuffer {
				VkDeviceMemory Memory;
				VkBuffer Buffer;
				VkDescriptorBufferInfo DescriptorInfo;
				uint32_t Size;
				uint32_t BindingPoint;
				std::string Name;
				VkShaderStageFlagBits ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
			};

			void AllocateUniformBuffer(VulkanUniformBuffer& buffer);

			std::unordered_map<uint32_t, VulkanUniformBuffer> m_UniformBuffers;
		};
	}
}