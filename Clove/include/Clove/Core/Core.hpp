#pragma once

#if CLV_DEBUG
	#define CLV_ENABLE_ASSERTS 1
#else
	#define CLV_ENABLE_ASSERTS 0
#endif

#define CLV_ENABLE_PROFILING 0

#if CLV_ENABLE_ASSERTS
	#define CLV_ASSERT(x, ...) { if(!(x)){ CLV_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); CLV_DEBUG_BREAK; } }
#else
	#define CLV_ASSERT(x, ...) (x)
#endif

#define BIT(x) (1 << x)

#if defined(__clang__)
	#define CLV_DEBUG_BREAK __builtin_debugtrap()
#elif (defined(__GNUC__) || defined(__GNUG__))
	#define CLV_DEBUG_BREAK __builtin_trap()
#elif defined(_MSC_VER)
	#define CLV_DEBUG_BREAK __debugbreak()
#endif

#define	CLV_FUNCTION_NAME __FUNCTION__
#if defined(_MSC_VER)
	#define CLV_FUNCTION_NAME_PRETTY __FUNCSIG__
#else
	#define CLV_FUNCTION_NAME_PRETTY __PRETTY_FUNCTION__
#endif

#if CLV_PLATFORM_WINDOWS
	#define CLV_APIENTRY __stdcall
#else
	#define CLV_APIENTRY
#endif
