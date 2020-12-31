#pragma once

#include "Snow/Render/Pipeline.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include "Snow/Render/Shader/ShaderUniform.h"

#include <spirv_cross.hpp>

namespace Snow {

	struct VulkanUniformBuffer {
		VkDeviceMemory Memory; // Vulkan specific buffer stuff
		VkBuffer VKBuffer;
		VkDescriptorBufferInfo DescriptorInfo;

		Render::ShaderUniformBuffer Buffer;
	};

	class VulkanPipeline : public Render::Pipeline {
	public:
		VulkanPipeline(const Render::PipelineSpecification& spec);

		void Bind() const override;

		void SetUniformBufferData(const std::string& name, void* data, uint32_t size) override {}
		void SetUniformBufferData(uint32_t bindingPoint, void* data, uint32_t size) override {}

		const Render::PipelineSpecification& GetSpecification() const override { return m_Specification; }
	private:
		void Invalidate();

		void Reflect(VkGraphicsPipelineCreateInfo& pipelineCreateInfo);

		static VkFormat SPIRVTypeToVKFormat(spirv_cross::SPIRType type);
		static VkShaderStageFlagBits SnowShaderTypeToVKShaderStage(Render::ShaderType type);

		Render::PipelineSpecification m_Specification;

		VkGraphicsPipelineCreateInfo m_GraphicsPipelineCreateInfo;
		VkPipeline m_VulkanPipeline;



		void AllocateUniformBuffer(VulkanUniformBuffer& buffer);

		std::unordered_map<uint32_t, VulkanUniformBuffer> m_UniformBuffers;
	};
}