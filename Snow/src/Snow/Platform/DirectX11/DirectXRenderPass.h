#pragma once

#include "Snow/Render/RenderPass.h"

namespace Snow {
	class DirectX11RenderPass : public Render::RenderPass {
	public:
		DirectX11RenderPass(const Render::RenderPassSpecification& spec);

		virtual void BeginPass() override;
		virtual void EndPass() override;

		const Render::RenderPassSpecification& GetSpecification() const override { return m_Specification; }
		Render::RenderPassSpecification& GetSpecification() override { return m_Specification; }
	private:
		Render::RenderPassSpecification m_Specification;
	};
}