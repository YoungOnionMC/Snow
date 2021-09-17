#include <spch.h>
#include "Snow/Render/Framebuffer.h"

#include "Snow/Render/Renderer.h"

//#include "Snow/Platform/OpenGL/OpenGLFramebuffer.h"
#include "Snow/Platform/Vulkan/VulkanFramebuffer.h"

#if defined SNOW_PLATFORM_WINDOWS
	//#include "Snow/Platform/DirectX11/DirectXFramebuffer.h"
#endif

namespace Snow {
	namespace Render {

		Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
			Ref<Framebuffer> result = nullptr;

			switch (Render::RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
			//case RendererAPIType::OpenGL:	result = Ref<OpenGLFramebuffer>::Create(spec); break;
			case RendererAPIType::Vulkan:	result = Ref<VulkanFramebuffer>::Create(spec); break;
#if defined SNOW_PLATFORM_WINDOWS
			//case RendererAPIType::DirectX:	result = Ref<DirectX11Framebuffer>::Create(spec); break;
#endif
			}

			return result;
		}

	}
}