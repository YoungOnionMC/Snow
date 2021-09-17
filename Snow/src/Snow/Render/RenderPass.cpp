#include <spch.h>

#include "Snow/Render/RenderPass.h"

#include "Snow/Render/Renderer.h"

//#include "Snow/Platform/OpenGL/OpenGLRenderPass.h"
#include "Snow/Platform/Vulkan/VulkanRenderPass.h"

#if defined SNOW_PLATFORM_WINDOWS
//#include "Snow/Platform/DirectX11/DirectXRenderPass.h"
#endif

namespace Snow {
	namespace Render {
		Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec) {
			switch (Render::RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
			//case RendererAPIType::OpenGL:	return Ref<OpenGLRenderPass>::Create(spec);
			case RendererAPIType::Vulkan:	return Ref<VulkanRenderPass>::Create(spec);
#if defined SNOW_PLATFORM_WINDOWS
			//case RendererAPIType::DirectX:	return Ref<DirectX11RenderPass>::Create(spec);
#endif
			}

			return nullptr;
		}
	}
}