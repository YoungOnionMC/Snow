#pragma once

#include <memory>
#include <stdint.h>
#include "Ref.h"

#define SNOW_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Snow {
	namespace Core {
		template<typename T>
		using Scope = std::unique_ptr<T>;

		template<typename T, typename ... Args>
		constexpr Scope<T> CreateScope(Args&& ... args) {
			return std::make_unique<T>(std::forward<Args>(args)...);
		}
	}

}