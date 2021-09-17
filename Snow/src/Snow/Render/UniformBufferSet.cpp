#include <spch.h>

#include "Snow/Render/UniformBufferSet.h"

#include "Snow/Platform/Vulkan/VulkanUniformBufferSet.h"

#include "Snow/Render/RendererAPI.h"

namespace Snow {
	namespace Render {
		Ref<UniformBufferSet> UniformBufferSet::Create(uint32_t frames) {
			switch (RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
			case RendererAPIType::Vulkan:	return Ref<VulkanUniformBufferSet>::Create(frames);
			//case RendererAPIType::OpenGL:	return Ref<OpenGLUniformBufferSet>::Create(frames);
			}
			SNOW_CORE_ASSERT(false, "unknown rendererAPI!");
			return nullptr;
		}
	}
}