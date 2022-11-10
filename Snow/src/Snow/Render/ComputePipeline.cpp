#include <spch.h>
#include "Snow/Render/ComputePipeline.h"

#include "Snow/Platform/Vulkan/VulkanComputePipeline.h"

#include "Snow/Render/RendererAPI.h"

namespace Snow {
	namespace Render {
		Ref<ComputePipeline> ComputePipeline::Create(Ref<Shader> computeShader) {
			switch (Render::RendererAPI::Current()) {
			case Render::RendererAPIType::None:		return nullptr;
			case Render::RendererAPIType::Vulkan:	return Ref<VulkanComputePipeline>::Create(computeShader);
			}
			SNOW_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}

	}
}