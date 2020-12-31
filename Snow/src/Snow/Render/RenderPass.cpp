#include <spch.h>

#include "Snow/Render/RenderPass.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLRenderPass.h"

#if defined SNOW_PLATFORM_WINDOWS
#include "Snow/Platform/DirectX11/DirectXRenderPass.h"
#endif

namespace Snow {
	namespace Render {
		Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec) {
			switch (Renderer::GetRenderAPI()) {
			case RenderAPIType::None:	return nullptr;
			case RenderAPIType::OpenGL:	return Ref<OpenGLRenderPass>::Create(spec);

#if defined SNOW_PLATFORM_WINDOWS
			case RenderAPIType::DirectX:	return Ref<DirectX11RenderPass>::Create(spec);
#endif
			}

			return nullptr;
		}
	}
}