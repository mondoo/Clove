#pragma once

#ifdef CLV_PLATFORM_WINDOWS
	#ifdef CLV_BUILD_DLL
		#define	CLV_API __declspec(dllexport)
	#else
		#define	CLV_API __declspec(dllimport)
	#endif
#else
	#error Clove only supports Windows!
#endif

#ifdef CLV_ENABLE_ASSERTS
	#define CLV_ASSERT(x, ...) { if(!(x)){ CLV_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CLV_CORE_ASSERT(x, ...) { if(!(x)){ CLV_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CLV_ASSERT(x, ...)
	#define CLV_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)