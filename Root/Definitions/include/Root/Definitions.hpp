#pragma once

#if defined(__clang__)
    #define GARLIC_DEBUG_BREAK __builtin_debugtrap()
#elif(defined(__GNUC__) || defined(__GNUG__))
    #define GARLIC_DEBUG_BREAK __builtin_trap()
#elif defined(_MSC_VER)
    #define GARLIC_DEBUG_BREAK __debugbreak()
#endif

#define GARLIC_FUNCTION_NAME __FUNCTION__
#if defined(_MSC_VER)
    #define GARLIC_FUNCTION_NAME_PRETTY __FUNCSIG__
#else
    #define GARLIC_FUNCTION_NAME_PRETTY __PRETTY_FUNCTION__
#endif

#if GARLIC_PLATFORM_WINDOWS
    #define GARLIC_APIENTRY __stdcall
#else
    #define GARLIC_APIENTRY
#endif