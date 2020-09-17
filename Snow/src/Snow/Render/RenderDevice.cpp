#include <spch.h>
#include "Snow/Render/RenderDevice.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLDevice.h"

namespace Snow {
	namespace Render {
		Device* Device::Create(const DeviceSpecification& spec) {
			switch (Renderer::GetRenderAPI()) {
			case RenderAPI::None:	return nullptr;
			case RenderAPI::OpenGL:	return new OpenGLDevice(spec);
			}
		}
	}
}