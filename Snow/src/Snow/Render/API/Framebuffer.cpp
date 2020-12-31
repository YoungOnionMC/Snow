#include <spch.h>
#include "Snow/Render/API/Framebuffer.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLFramebuffer.h"

#if defined SNOW_PLATFORM_WINDOWS
	#include "Snow/Platform/DirectX11/DirectXFramebuffer.h"
#endif

namespace Snow {
	namespace Render {

		Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
			Ref<Framebuffer> result = nullptr;

			switch (Renderer::GetRenderAPI()) {
			case RenderAPIType::None:	return nullptr;
			case RenderAPIType::OpenGL:	result = Ref<OpenGLFramebuffer>::Create(spec); break;
#if defined SNOW_PLATFORM_WINDOWS
			case RenderAPIType::DirectX:	result = Ref<DirectX11Framebuffer>::Create(spec); break;
#endif
			}

			return result;
		}

	}
}