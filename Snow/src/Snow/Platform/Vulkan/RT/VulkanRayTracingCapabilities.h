#pragma once


#include "Snow/Render/RT/RayTracingCapabilities.h"

namespace Snow {
	struct VulkanRTCapabilities : Render::RT::RTCapabilities {
		uint32_t MaxDescriptorSetAccelerationStructures;
		uint32_t MaxRayRecursionDepth;
	};
}