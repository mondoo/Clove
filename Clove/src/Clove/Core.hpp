#pragma once

//Utility defines
#if CLV_DEBUG || CLV_DEVELOPMENT
	#define CLV_ENABLE_ASSERTS 1
#else
	#define CLV_ENABLE_ASSERTS 0
#endif

#if CLV_PROFILING
	#define CLV_ENABLE_PROFILING 1
#else
	#define CLV_ENABLE_PROFILING 0
#endif

#if CLV_ENABLE_ASSERTS
	#define CLV_ASSERT(x, ...) { if(!(x)){ CLV_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); CLV_DEBUG_BREAK; } }
#else
	#define CLV_ASSERT(x, ...) (x)
#endif

#define BIT(x) (1 << x)

//TODO: Should check for MSVC instead of windows
#if CLV_PLATFORM_WINDOWS
	#define CLV_DEBUG_BREAK __debugbreak()
#elif CLV_PLATFORM_LINUX
	#define CLV_DEBUG_BREAK __builtin_trap()
#else
	#define CLV_DEBUG_BREAK
#endif

//TODO: Should check for MSVC instead of windows
#if CLV_PLATFORM_WINDOWS
	#define	CLV_FUNCTION_NAME __FUNCTION__
#else 
	#define	CLV_FUNCTION_NAME __func__
#endif

#if CLV_PLATFORM_WINDOWS
	#define CLV_APIENTRY APIENTRY
#else
	#define CLV_APIENTRY
#endif
