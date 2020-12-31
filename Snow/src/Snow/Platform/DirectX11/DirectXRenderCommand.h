#pragma once

#include "Snow/Render/RenderCommand.h"

namespace Snow {
	class DirectX11RenderCommand : public Render::RenderAPI {
	public:
		virtual void Init() override {}

		void SetClearColor(const glm::vec4& color) override {}
		void Clear() override {}

		void DrawIndexed(uint32_t count, Render::PrimitiveType type) override;

		void BeginCommandBuffer() override {}
		void EndCommandBuffer() override {}

		void SetBlending(bool blend) override {}
		void SetDepthTesting(bool depthTest) override {}

		void SetViewport(uint32_t width, uint32_t height) override {}

		void SwapBuffers() override;
	};
}
