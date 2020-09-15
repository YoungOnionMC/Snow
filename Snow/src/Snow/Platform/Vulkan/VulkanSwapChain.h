#pragma once

#include "Snow/Render/SwapChain.h"

#include <vulkan/vulkan.h>

namespace Snow {
	namespace Render {
		class VulkanSwapChain : public SwapChain {
		public:
			VulkanSwapChain(const SwapChainSpecification& spec);

			void SwapBuffers() override;

		private:

		};
	}
}