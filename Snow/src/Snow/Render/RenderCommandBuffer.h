#pragma once

#include "Snow/Core/Ref.h"

namespace Snow {
	namespace Render {
		class RenderCommandBuffer : public RefCounted {
		public:
			virtual ~RenderCommandBuffer() = default;

			virtual void Begin() = 0;
			virtual void End() = 0;
			virtual void Submit() = 0;


			static Ref<RenderCommandBuffer> Create(uint32_t count = 0, const std::string& debugName = "");
			static Ref<RenderCommandBuffer> CreateFromSwapChain(const std::string& debugName = "");
		};
	}
}