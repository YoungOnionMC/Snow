#include <spch.h>

#include "Snow/Render/UniformBuffer.h"

#include "Snow/Platform/Vulkan/VulkanUniformBuffer.h"

#include "Snow/Render/RendererAPI.h"

namespace Snow {
	namespace Render {
		Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
			switch (RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
			case RendererAPIType::Vulkan:	return Ref<VulkanUniformBuffer>::Create(size, binding);
			//case RendererAPIType::OpenGL:	return Ref<OpenGLUniformBuffer>::Create(size, binding);
			}

			SNOW_CORE_ASSERT(false, "unknown renderAPI");
			return nullptr;
		}
	}
}