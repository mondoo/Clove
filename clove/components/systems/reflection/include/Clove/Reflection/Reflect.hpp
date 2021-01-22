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

#define CLOVE_REFLECT_MEMBER(member)                                    \
    template</* typename T */>                                          \
    struct MemberInfo<__COUNTER__ - memberIndexOffset> {                \
        using ParentType = Type;                                        \
        using Type       = decltype(ParentType::member);                \
                                                                        \
        static std::string_view constexpr name{ #member };              \
        static size_t constexpr size{ sizeof(Type) };                   \
        static size_t constexpr offset{ offsetof(ParentType, member) }; \
    };

#define CLOVE_REFLECT_END                                                   \
                                                                            \
    static size_t constexpr memberCount{ __COUNTER__ - memberIndexOffset }; \
    }                                                                       \
    ;                                                                       \
    }