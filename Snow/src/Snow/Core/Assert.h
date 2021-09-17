#pragma once

#include <filesystem>

#define SNOW_ENABLE_VERIFY

#ifdef SNOW_ENABLE_ASSERTS
	#define SNOW_ASSERT_MSG(condition, ...) {if(!(condition)) {SNOW_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define SNOW_ASSERT_NO_MSG(condition) {if(!(condition)) {SNOW_CORE_ERROR("Assertion Failed"); __debugbreak();}}
	
	#define SNOW_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
	#define SNOW_GET_ASSERT_MACRO(...) SNOW_EXPAND_MACRO( SNOW_ASSERT_RESOLVE(__VA_ARGS__, SNOW_ASSERT_MSG, SNOW_ASSERT_NO_MSG) )
	
	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define SNOW_ASSERT(...) SNOW_EXPAND_MACRO( SNOW_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
	#define SNOW_CORE_ASSERT(...) SNOW_EXPAND_MACRO( SNOW_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
	#define SNOW_ASSERT(...)
	#define SNOW_CORE_ASSERT(...)
#endif

#ifdef SNOW_ENABLE_VERIFY
	#define SNOW_VERIFY_NO_MSG(condition) {if(!(condition)) {SNOW_CORE_ERROR("Verify Failed"); __debugbreak();}}
	#define SNOW_VERIFY_MSG(condition, ...) {if(!(condition)) {SNOW_CORE_ERROR("Verify Failed: {0}", __VA_ARGS__); __debugbreak();}}
	
	#define SNOW_VERIFY_RESOLVE(arg1, arg2, macro, ...) macro
	#define SNOW_GET_VERIFY_MACRO(...) SNOW_EXPAND_MACRO(SNOW_VERIFY_RESOLVE(__VA_ARGS__, SNOW_VERIFY_MSG, SNOW_VERIFY_NO_MSG))
	
	#define SNOW_VERIFY(...) SNOW_EXPAND_MACRO(SNOW_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__))
	#define SNOW_CORE_VERIFY(...) SNOW_EXPAND_MACRO(SNOW_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__))
#else
	#define SNOW_VERIFY(...)
	#define SNOW_CORE_VERIFY(...)
#endif