#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Render/Shader.h"

namespace Snow {
	namespace Render {
		class ComputePipeline : public RefCounted {
		public:
			virtual void Execute(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;

			virtual Ref<Shader> GetShader() const = 0;

			Ref<ComputePipeline> Create(Ref<Shader> computeShader);
		};
	}
}