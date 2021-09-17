#include <spch.h>
#include "Snow/Render/RenderCommandBuffer.h"

#include "Snow/Platform/Vulkan/VulkanRenderCommandBuffer.h"

#include "Snow/Render/RendererAPI.h"

namespace Snow {
	namespace Render {
		Ref<RenderCommandBuffer> RenderCommandBuffer::Create(uint32_t count, const std::string& debugName) {
			switch (RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
			case RendererAPIType::Vulkan:	return Ref<VulkanRenderCommandBuffer>::Create(count, debugName);
			}
			SNOW_CORE_ASSERT(false, "unknown renderAPI");
			return nullptr;
		}

		Ref<RenderCommandBuffer> RenderCommandBuffer::CreateFromSwapChain(const std::string& debugName) {
			switch (RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
			case RendererAPIType::Vulkan:	return Ref<VulkanRenderCommandBuffer>::Create(debugName, true);
			}
			SNOW_CORE_ASSERT(false, "unknown renderAPI");
			return nullptr;
		}
	}
}