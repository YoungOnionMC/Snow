#pragma once

#include "Snow/Render/RenderPass.h"

namespace Snow {
	class OpenGLRenderPass : public Render::RenderPass {
	public:
		OpenGLRenderPass(const Render::RenderPassSpecification& spec);
		virtual ~OpenGLRenderPass();

		virtual void BeginPass() override;
		virtual void EndPass() override;

		virtual Render::RenderPassSpecification& GetSpecification() override { return m_Specification; }
		virtual const Render::RenderPassSpecification& GetSpecification() const override { return m_Specification; }
	private:
		Render::RenderPassSpecification m_Specification;
	};
}
