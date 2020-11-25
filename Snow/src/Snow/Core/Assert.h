#pragma once

#include "Snow/Core/Base.h"
#include "Snow/Core/Log.h"
#include <filesystem>

#ifdef SNOW_ENABLE_ASSERTS
	#define SNOW_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { SNOW##type##ERROR(msg, __VA_ARGS__); SNOW_DEBUGBREAK(); } }
	#define SNOW_INTERNAL_ASSERT_WITH_MSG(type, check, ...) SNOW_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define SNOW_INTERNAL_ASSERT_NO_MSG(type, check) SNOW_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", SNOW_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
	
	#define SNOW_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define SNOW_INTERNAL_ASSERT_GET_MACRO(...) SNOW_EXPAND_MACRO( SNOW_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, SNOW_INTERNAL_ASSERT_WITH_MSG, SNOW_INTERNAL_ASSERT_NO_MSG) )
	
	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define SNOW_ASSERT(...) SNOW_EXPAND_MACRO( SNOW_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CLIENT_, __VA_ARGS__) )
	#define SNOW_CORE_ASSERT(...) SNOW_EXPAND_MACRO( SNOW_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define SNOW_ASSERT(...)
	#define SNOW_CORE_ASSERT(...)
#endif