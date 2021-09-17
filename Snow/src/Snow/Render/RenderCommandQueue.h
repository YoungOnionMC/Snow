#pragma once

#include <spch.h>

namespace Snow {
	namespace Render {
		class RenderCommandQueue {
		public:
			typedef void(*RenderCommandFn)(void*);

			RenderCommandQueue();
			~RenderCommandQueue();

			void* Allocate(RenderCommandFn func, uint32_t size);

			void Execute();
		private:
			uint8_t* m_CommandBuffer;
			uint8_t* m_CommandBufferPtr;
			uint32_t m_CommandCount;
		};
	}
}