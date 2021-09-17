#include <spch.h>
#include "Snow/Render/StorageBuffer.h"

#include "Snow/Platform/Vulkan/VulkanStorageBuffer.h"

#include "Snow/Render/RendererAPI.h"

namespace Snow {
	namespace Render {
		Ref<StorageBuffer> StorageBuffer::Create(uint32_t size, uint32_t binding) {
			switch (RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
			case RendererAPIType::Vulkan:	return Ref<VulkanStorageBuffer>::Create(size, binding);
				//case RendererAPIType::OpenGL:	return Ref<OpenGLUniformBuffer>::Create(size, binding);
			}

			SNOW_CORE_ASSERT(false, "unknown renderAPI");
			return nullptr;
		}
	}
}