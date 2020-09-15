#pragma once

namespace Snow {
	namespace Render {
		struct DeviceSpecification {

		};

		class Device {
		public:
			static Device* Create(const DeviceSpecification& spec);
		};
	}
}