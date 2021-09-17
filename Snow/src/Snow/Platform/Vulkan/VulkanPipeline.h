#pragma once

#include "Snow/Render/Pipeline.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include "Snow/Render/ShaderUniform.h"

#include <spirv_cross.hpp>

namespace Snow {
	class VulkanPipeline : public Render::Pipeline {
	public:
		VulkanPipeline(const Render::PipelineSpecification& spec);

		void Bind() const override;

		Render::PipelineSpecification& GetSpecification() override { return m_Specification; }
		const Render::PipelineSpecification& GetSpecification() const override { return m_Specification; }

		virtual void SetUniformBuffer(Ref<Render::UniformBuffer> uniformBuffer, uint32_t binding, uint32_t set) override {}

		VkPipeline GetVulkanPipeline() { return m_VulkanPipeline; }
		VkPipelineLayout GetVulkanPipelineLayout() { return m_PipelineLayout; }
	private:
		void Invalidate();

		void Reflect(VkGraphicsPipelineCreateInfo& pipelineCreateInfo);

		static VkFormat SPIRVTypeToVKFormat(spirv_cross::SPIRType type);
		static VkShaderStageFlagBits SnowShaderTypeToVKShaderStage(Render::ShaderType type);

		Render::PipelineSpecification m_Specification;

		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_VulkanPipeline;
	};
}