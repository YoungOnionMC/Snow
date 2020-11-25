#include <spch.h>

#include "Snow/Platform/OpenGL/OpenGLRenderPass.h"

namespace Snow {
	OpenGLRenderPass::OpenGLRenderPass(const Render::RenderPassSpecification& spec) :
		m_Specification(spec) {}

	OpenGLRenderPass::~OpenGLRenderPass() {}
}