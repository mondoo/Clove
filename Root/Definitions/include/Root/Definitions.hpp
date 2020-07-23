#pragma once

#if GARLIC_ENABLE_ASSERTS
    #define GARLIC_ASSERT(x, ...)                                                                           \
        {                                                                                                   \
            if(!(x)) {                                                                                      \
                GARLIC_LOG(garlicLogContext, clv::Log::Level::Error, "Assertion Failed: {0}", __VA_ARGS__); \
                GARLIC_DEBUG_BREAK;                                                                         \
            }                                                                                               \
        }
#else
    #define GARLIC_ASSERT(x, ...) (x)
#endif

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

#define GARLIC_BIT(x) (1 << x)

#define GARLIC_ENUM_BIT_FLAG_OPERATORS(EnumType, IntegralType)                                              \
    inline constexpr EnumType operator&(EnumType l, EnumType r) {                                  \
        return static_cast<EnumType>(static_cast<IntegralType>(l) & static_cast<IntegralType>(r)); \
    }                                                                                              \
    inline constexpr EnumType operator|(EnumType l, EnumType r) {                                  \
        return static_cast<EnumType>(static_cast<IntegralType>(l) | static_cast<IntegralType>(r)); \
    }                                                                                              \
    inline constexpr EnumType operator^(EnumType l, EnumType r) {                                  \
        return static_cast<EnumType>(static_cast<IntegralType>(l) ^ static_cast<IntegralType>(r)); \
    }                                                                                              \
    inline constexpr EnumType operator&=(EnumType l, EnumType r) {                                 \
        l = l & r;                                                                                 \
        return l;                                                                                  \
    }                                                                                              \
    inline constexpr EnumType operator|=(EnumType l, EnumType r) {                                 \
        l = l | r;                                                                                 \
        return l;                                                                                  \
    }                                                                                              \
    inline constexpr EnumType operator^=(EnumType l, EnumType r) {                                 \
        l = l ^ r;                                                                                 \
        return l;                                                                                  \
    }                                                                                              \
    inline constexpr bool operator==(EnumType l, IntegralType r) {                                 \
        return static_cast<IntegralType>(l) == r;                                                  \
    }                                                                                              \
    inline constexpr bool operator!=(EnumType l, IntegralType r) {                                 \
        return !(l == r);                                                                          \
    }