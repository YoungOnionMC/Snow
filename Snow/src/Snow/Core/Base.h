#pragma once

#include <memory>
#include <stdint.h>

#include "Assert.h"
#include "Ref.h"
#include "Scope.h"

#if defined SNOW_DEBUG
	#if defined(SNOW_PLATFORM_WINDOWS)
		#define SNOW_DEBUGBREAK() __debugbreak()
	#elif defined(SNOW_PLATFORM_LINUX)
		#include <signal.h>
		#define SNOW_DEBUGBREAK() raise(SIGTRAP)
	#endif
	#ifndef SNOW_ENABLE_ASSERTS
		#define SNOW_ENABLE_ASSERTS
	#endif
	//#define SNOW_DEBUGBREAK()
#else
	#define SNOW_DEBUGBREAK()
#endif

#define SNOW_EXPAND_MACRO(x) x
#define SNOW_STRINGIFY_MACRO(x) #x

#define BIT(x) (1<<x)

template<class type, ptrdiff_t n>
ptrdiff_t countOf(type(&)[n]) { return n; }

#define CountOf(x) countOf(x)

#define SNOW_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Snow {
	namespace Core {
		template<typename T>
		using Scope = std::unique_ptr<T>;

		template<typename T, typename ... Args>
		constexpr Scope<T> CreateScope(Args&& ... args) {
			return std::make_unique<T>(std::forward<Args>(args)...);
		}
	}

	using byte = uint8_t;
}