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

#define BIT(x) (1 << x)