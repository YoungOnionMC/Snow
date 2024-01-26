#pragma once

#include <stdint.h>

namespace Snow {
	namespace Render {
		namespace RT {
			struct RTCapabilities {
				uint32_t MaxGeometryCount;
				uint32_t MaxInstanceCount;
				uint32_t MaxPrimitiveCount;
			};
		};
	}
}