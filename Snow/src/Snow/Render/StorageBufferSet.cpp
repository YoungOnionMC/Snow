#include <spch.h>

#include "Snow/Render/StorageBufferSet.h"

#include "Snow/Platform/Vulkan/VulkanStorageBufferSet.h"

#include "Snow/Render/RendererAPI.h"

namespace Snow {
	namespace Render {
		Ref<StorageBufferSet> StorageBufferSet::Create(uint32_t frames) {
			switch (RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
			case RendererAPIType::Vulkan:	return Ref<VulkanStorageBufferSet>::Create(frames);
				//case RendererAPIType::OpenGL:	return Ref<OpenGLStorageBufferSet>::Create(frames);
			}
			SNOW_CORE_ASSERT(false, "unknown rendererAPI!");
			return nullptr;
		}
	}
}