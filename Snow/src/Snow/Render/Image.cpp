#include <spch.h>
#include "Snow/Render/Image.h"

#include "Snow/Render/Renderer.h"

//#include "Snow/Platform/OpenGL/OpenGLImage.h"
#include "Snow/Platform/Vulkan/VulkanImage.h"

namespace Snow {
	namespace Render {
		Ref<Image2D> Image2D::Create(ImageSpecification spec, Buffer buffer) {
			switch (Render::RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
			//case RendererAPIType::OpenGL:	return Ref<OpenGLImage2D>::Create(spec, buffer);
			case RendererAPIType::Vulkan:	return Ref<VulkanImage2D>::Create(spec);
			}
		}

		Ref<Image2D> Image2D::Create(ImageSpecification spec, const void* data) {
			switch (Render::RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
				//case RendererAPIType::OpenGL:	return Ref<OpenGLImage2D>::Create(spec, data);
			case RendererAPIType::Vulkan:	return Ref<VulkanImage2D>::Create(spec);
			}
		}
	}
}