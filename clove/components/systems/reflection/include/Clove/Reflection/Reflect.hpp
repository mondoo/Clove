#pragma once

#include "Clove/Reflection/Common.hpp"

#include <string_view>
#include <tuple>
#include <utility>

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
            template<size_t>                                              \
            struct MemberInfo;

#define CLOVE_REFLECT_MEMBER(member)                                    \
    template<>                                                          \
    struct MemberInfo<__COUNTER__ - memberIndexOffset> {                \
        using ParentType = Type;                                        \
        using Type       = decltype(ParentType::member);                \
                                                                        \
        static std::string_view constexpr name{ #member };              \
        static size_t constexpr size{ sizeof(Type) };                   \
        static size_t constexpr offset{ offsetof(ParentType, member) }; \
                                                                        \
        Type value{};                                                   \
    };

#define CLOVE_REFLECT_END                                                   \
                                                                            \
    static size_t constexpr memberCount{ __COUNTER__ - memberIndexOffset }; \
                                                                            \
    template<std::size_t... indices>                                        \
    auto getMembersImpl(std::index_sequence<indices...>) {                  \
        return std::tuple{ MemberInfo<indices>{}... };                      \
    }                                                                       \
    auto getMembers() {                                                     \
        return getMembersImpl(std::make_index_sequence<memberCount>{});     \
    }                                                                       \
    }                                                                       \
    ;                                                                       \
    }