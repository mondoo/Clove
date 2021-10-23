#pragma once

#include <optional>
#include <string_view>
#include <tuple>

namespace clove::reflection {
    /**
     * @brief Provides information about a specific type.
     */
    template<typename T>
    struct TypeInfo;

    /**
     * @brief Returns true if typeInfo has attribute of type AttributeType.
     * @tparam AttributeType 
     * @tparam ClassType 
     * @param typeInfo 
     * @return 
     */
    template<typename AttributeType, typename ClassType>
    bool constexpr hasAttribute(TypeInfo<ClassType> const &typeInfo);

    /**
     * @brief Returns true if member has attribute of type AttributeType.
     * @tparam AttributeType 
     * @tparam MemberType 
     * @param member 
     * @return 
     */
    template<typename AttributeType, typename MemberType>
    bool constexpr hasAttribute(MemberType const &member);

    /**
     * @brief Returns the attribute on the typeInfo if it exists.
     * @tparam AttributeType 
     * @tparam ClassType 
     * @param typeInfo 
     * @return 
     */
    template<typename AttributeType, typename ClassType>
    std::optional<AttributeType> getAttribute(TypeInfo<ClassType> const &typeInfo);

    /**
     * @brief Returns the attribute on the member if it exists.
     * @tparam AttributeType 
     * @tparam MemberType 
     * @param member 
     * @return 
     */
    template<typename AttributeType, typename MemberType>
    std::optional<AttributeType> getAttribute(MemberType const &member);

    /**
     * @brief Calls functor on each member inside a reflected Type.
     * @tparam Type 
     * @tparam FunctorType 
     * @param functor 
     */
    template<typename Type, typename FunctorType>
    void constexpr forEachMember(FunctorType &&functor);
}

#define CLOVE_REFLECT_BEGIN(classType, ...)                                  \
    template<>                                                               \
    struct clove::reflection::TypeInfo<classType> {                          \
        using Type = classType;                                              \
                                                                             \
        template<size_t>                                                     \
        struct Member;                                                       \
                                                                             \
        static inline auto const attributes{ std::make_tuple(__VA_ARGS__) }; \
        static size_t constexpr memberIndexOffset{ __COUNTER__ + 1 };

#define CLOVE_REFLECT_PROPERTY(property, ...)                                \
    template<>                                                               \
    struct Member<__COUNTER__ - memberIndexOffset> {                         \
        static std::string_view constexpr name{ #property };                 \
        static size_t constexpr offset{ offsetof(Type, property) };          \
        static size_t constexpr size{ sizeof(Type::property) };              \
                                                                             \
        static inline auto const attributes{ std::make_tuple(__VA_ARGS__) }; \
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
    friend struct clove::reflection::TypeInfo<classType>;

#include "Reflection.inl"