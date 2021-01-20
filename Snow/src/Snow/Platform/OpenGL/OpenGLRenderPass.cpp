#include <spch.h>

#include "Snow/Platform/OpenGL/OpenGLRenderPass.h"

#include "Snow/Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Snow {
	OpenGLRenderPass::OpenGLRenderPass(const Render::RenderPassSpecification& spec) :
		m_Specification(spec) {}

	OpenGLRenderPass::~OpenGLRenderPass() {}

	void OpenGLRenderPass::BeginPass() {
		auto glFramebuffer = m_Specification.TargetFramebuffer.As<OpenGLFramebuffer>();
		GLenum clearBit = 0;
		if (glFramebuffer->GetColorAttachments().size()) {
			auto c = glFramebuffer->GetSpecification().ClearColor;
			clearBit |= GL_COLOR_BUFFER_BIT;
			glClearColor(c.r, c.g, c.b, c.a);
		}

		if (glFramebuffer->GetDepthAttachment() != 0) {
			clearBit |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
		}

		glClear(clearBit);
	}

	void OpenGLRenderPass::EndPass() {

	}
}