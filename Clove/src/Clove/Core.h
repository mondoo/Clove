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

#if CLV_DEBUG
	#define CLV_ENABLE_ASSERTS 1
#endif

#if CLV_ENABLE_ASSERTS
	#define CLV_ASSERT(x, ...) { if(!(x)){ CLV_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CLV_CORE_ASSERT(x, ...) { if(!(x)){ CLV_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CLV_ASSERT(x, ...)
	#define CLV_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_FUNCTION(func, obj) std::bind(func, obj)
#define BIND_FUNCTION_OneParam(func, obj) std::bind(func, obj, std::placeholders::_1)
#define BIND_FUNCTION_TwoParam(func, obj) std::bind(func, obj, std::placeholders::_2)
#define BIND_FUNCTION_ThreeParam(func, obj) std::bind(func, obj, std::placeholders::_3)