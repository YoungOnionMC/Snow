#pragma once

#include "Snow/Render/Pipeline.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

namespace Snow {
	namespace Render {
		class VulkanPipeline : public Pipeline {
		public:
			VulkanPipeline(const PipelineSpecification& spec);

			void Bind() override;

			const PipelineSpecification& GetSpecification() const override { return m_Specification; }
		private:
			void Invalidate();

			PipelineSpecification m_Specification;

			VkPipeline m_VulkanPipeline;
		};
	}
}