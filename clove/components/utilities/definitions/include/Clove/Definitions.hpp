#pragma once

#if defined(__clang__)
    #define CLOVE_DEBUG_BREAK __builtin_debugtrap()
#elif(defined(__GNUC__) || defined(__GNUG__))
    #define CLOVE_DEBUG_BREAK __builtin_trap()
#elif defined(_MSC_VER)
    #define CLOVE_DEBUG_BREAK __debugbreak()
#endif

#define CLOVE_FUNCTION_NAME __FUNCTION__
#if defined(_MSC_VER)
    #define CLOVE_FUNCTION_NAME_PRETTY __FUNCSIG__
#else
    #define CLOVE_FUNCTION_NAME_PRETTY __PRETTY_FUNCTION__
#endif

#define CLOVE_ENUM_BIT_FLAG_OPERATORS(EnumType, IntegralType)                                      \
    inline EnumType constexpr operator&(EnumType l, EnumType r) {                                  \
        return static_cast<EnumType>(static_cast<IntegralType>(l) & static_cast<IntegralType>(r)); \
    }                                                                                              \
    inline EnumType constexpr operator|(EnumType l, EnumType r) {                                  \
        return static_cast<EnumType>(static_cast<IntegralType>(l) | static_cast<IntegralType>(r)); \
    }                                                                                              \
    inline EnumType constexpr operator^(EnumType l, EnumType r) {                                  \
        return static_cast<EnumType>(static_cast<IntegralType>(l) ^ static_cast<IntegralType>(r)); \
    }                                                                                              \
    inline EnumType constexpr operator&=(EnumType &l, EnumType r) {                                \
        l = l & r;                                                                                 \
        return l;                                                                                  \
    }                                                                                              \
    inline EnumType constexpr operator|=(EnumType &l, EnumType r) {                                \
        l = l | r;                                                                                 \
        return l;                                                                                  \
    }                                                                                              \
    inline EnumType constexpr operator^=(EnumType &l, EnumType r) {                                \
        l = l ^ r;                                                                                 \
        return l;                                                                                  \
    }                                                                                              \
    inline bool constexpr operator==(EnumType l, IntegralType r) {                                 \
        return static_cast<IntegralType>(l) == r;                                                  \
    }                                                                                              \
    inline bool constexpr operator!=(EnumType l, IntegralType r) {                                 \
        return !(l == r);                                                                          \
    }
