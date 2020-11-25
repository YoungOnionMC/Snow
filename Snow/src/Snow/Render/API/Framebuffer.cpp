#include <spch.h>
#include "Snow/Render/API/Framebuffer.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Snow {
	namespace Render {

		Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
			Ref<Framebuffer> result = nullptr;

			switch (Renderer::GetRenderAPI()) {
			case RenderAPIType::None:	return nullptr;
			case RenderAPIType::OpenGL:	result = Ref<OpenGLFramebuffer>::Create(spec);
			}

			return result;
		}

	}
}