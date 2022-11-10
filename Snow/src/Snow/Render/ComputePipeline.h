#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Render/Shader.h"

#include "Snow/Render/RenderCommandBuffer.h"

namespace Snow {
	namespace Render {
		class ComputePipeline : public RefCounted {
		public:
			virtual void Begin(Ref<RenderCommandBuffer> commandBuffer = nullptr) = 0;
			virtual void End() = 0;

			

			virtual Ref<Shader> GetShader() = 0;

			static Ref<ComputePipeline> Create(Ref<Shader> computeShader);
		};
	}
}