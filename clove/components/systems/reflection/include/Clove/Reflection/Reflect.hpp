#pragma once

#include "Clove/Reflection/Common.hpp"

#include <string_view>

#define CLOVE_REFLECT_TYPE(type)                                          \
    namespace garlic::clove {                                             \
        template<>                                                        \
        struct TypeInfo<type> {                                           \
            using Type = type;                                            \
                                                                          \
            static std::string_view constexpr name{ #type };              \
            static size_t constexpr size{ sizeof(type) };                 \
                                                                          \
            static size_t constexpr memberIndexOffset{ __COUNTER__ + 1 }; \
                                                                          \
            template<size_t /* , typename T */>                           \
            struct MemberInfo;

#define CLOVE_REFLECT_MEMBER(member)                       \
    template</* typename T */>                             \
    struct MemberInfo<__COUNTER__ - memberIndexOffset> {   \
        using Type = decltype(Type::member);               \
                                                           \
        static std::string_view constexpr name{ #member }; \
    };

#define CLOVE_REFLECT_END                                                   \
                                                                            \
    static size_t constexpr memberCount{ __COUNTER__ - memberIndexOffset }; \
    }                                                                       \
    ;                                                                       \
    }