#pragma once

//Define checks
#ifndef CLV_DEBUG
	#define CLV_DEBUG 0
#endif

#ifndef CLV_RELEASE
	#define CLV_RELEASE 0
#endif

#if (CLV_DEBUG + CLV_RELEASE) != 1
	#error Invalid configuration!
#endif

#ifndef CLV_STATIC
	#define CLV_STATIC 0
#endif

#ifndef CLV_DYNAMIC
	#define CLV_DYNAMIC 0
#endif 

#if (CLV_STATIC + CLV_DYNAMIC) != 1
	#error Invalid library setting!
#endif

#ifndef CLV_ENGINE
	#define CLV_ENGINE 0
#endif

#ifndef CLV_EXPORT_DLL
	#define CLV_EXPORT_DLL 0
#endif

#if CLV_PLATFORM_WINDOWS
	#if CLV_DYNAMIC
		#if CLV_EXPORT_DLL
			#define	CLV_API __declspec(dllexport)
		#else
			#define	CLV_API __declspec(dllimport)
		#endif
	#else
		#define	CLV_API
	#endif
#else
	#error Clove only supports Windows!
#endif

#if CLV_DEBUG
	#define CLV_ENABLE_ASSERTS 1
	#define CLV_ENABLE_PROFILING 1
#else
	#define CLV_ENABLE_ASSERTS 0
	#define CLV_ENABLE_PROFILING 0
#endif

#if CLV_ENABLE_ASSERTS
	#define CLV_ASSERT(x, ...) { if(!(x)){ CLV_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CLV_ASSERT(x, ...) (x)
#endif

//Clove defines
#define BIT(x) (1 << x)

#define CLV_BIND_FUNCTION_0P(func, obj) std::bind(func, obj)
#define CLV_BIND_FUNCTION_1P(func, obj) std::bind(func, obj, std::placeholders::_1)
#define CLV_BIND_FUNCTION_2P(func, obj) std::bind(func, obj, std::placeholders::_2)
#define CLV_BIND_FUNCTION_3P(func, obj) std::bind(func, obj, std::placeholders::_3)