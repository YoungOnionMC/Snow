#include <spch.h>

#include "Snow/Render/RenderPass.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLRenderPass.h"

namespace Snow {
	namespace Render {
		Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec) {
			switch (Renderer::GetRenderAPI()) {
			case RenderAPIType::None:	return nullptr;
			case RenderAPIType::OpenGL:	return Ref<OpenGLRenderPass>::Create(spec);
			}

			return nullptr;
		}
	}
}