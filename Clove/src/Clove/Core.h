#pragma once

#ifdef CLV_PLATFORM_WINDOWS
	#ifdef CLV_BUILD_DLL
		#define	CLV_API __declspec(dllexport)
	#else
		#define	CLV_API __declspec(dllimport)
	#endif // CLV_BUILD_DLL
#else
	#error Clove only supports Windows!
#endif // CLV_PLATFORM_WINDOWS

#define BIT(x) (1 << x)