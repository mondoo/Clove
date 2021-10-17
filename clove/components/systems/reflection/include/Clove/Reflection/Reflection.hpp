#pragma once

#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace clove::reflection {
    /**
     * @brief Provides information about a specific class.
     */
    template<typename T>
    struct MetaClass;

    namespace internal {
        template<typename T, typename Tuple>
        struct HasType;
        template<typename T, typename... Us>
        struct HasType<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...> {};
    }

    /**
     * @brief Returns true if member has attribute of type AttributeType.
     * @tparam AttributeType 
     * @tparam MemberType 
     * @param member 
     * @return 
     */
    template<typename AttributeType, typename MemberType>
    bool constexpr hasAttribute(MemberType const &member) {
        return internal::HasType<AttributeType, std::decay_t<decltype(member.attributes)>>::value;
    }
}

#define CLOVE_REFLECT_BEGIN(classType)               \
    template<>                                       \
    struct clove::reflection::MetaClass<classType> { \
        using Type = classType;                      \
                                                     \
        template<size_t>                             \
        struct Member;                               \
                                                     \
        static size_t constexpr memberIndexOffset{ __COUNTER__ + 1 };

#define CLOVE_REFLECT_PROPERTY(property, ...)                             \
    template<>                                                            \
    struct Member<__COUNTER__ - memberIndexOffset> {                      \
        static std::string_view constexpr name{ #property };              \
        static size_t constexpr offset{ offsetof(Type, property) };       \
        static size_t constexpr size{ sizeof(Type::property) };           \
                                                                          \
        static auto constexpr attributes{ std::make_tuple(__VA_ARGS__) }; \
    };

#define CLOVE_REFLECT_END                                                       \
    static size_t constexpr memberCount{ __COUNTER__ - memberIndexOffset };     \
                                                                                \
    static auto constexpr getMembers() {                                        \
        return getMembersInternal(std::make_index_sequence<memberCount>{});     \
    }                                                                           \
                                                                                \
private:                                                                        \
    template<size_t... indices>                                                 \
    static auto constexpr getMembersInternal(std::index_sequence<indices...>) { \
        return std::tuple<Member<indices>...>{};                                \
    }                                                                           \
    }                                                                           \
    ;

/**
 * @brief Allows reflection of private members within a class.
 */
#define CLOVE_REFLECT_PRIVATE(classType) \
    friend struct clove::reflection::MetaClass<classType>;