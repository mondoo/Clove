#pragma once

#include <string>
#include <tuple>
#include <vector>

namespace clove {
    /**
     * @brief Provides information about a specific class.
     */
    template<typename T>
    struct MetaClass;

    /**
     * @brief Provides information about a property for a class.
     */
    struct MetaProperty {
        std::string name{}; /**< Name of the property. */
        size_t offset{ 0 }; /**< Offset of the property within the class. */
        size_t size{ 0 };   /**< Size of the property. */
    };
}

#define CLOVE_REFLECT_BEGIN(classType)   \
    template<>                           \
    struct clove::MetaClass<classType> { \
        using Type = classType;          \
                                         \
        template<size_t>                 \
        struct Property;                 \
                                         \
        static size_t constexpr memberIndexOffset{ __COUNTER__ + 1 };

#define CLOVE_REFLECT_PROPERTY(property)               \
    template<>                                         \
    struct Property<__COUNTER__ - memberIndexOffset> { \
        std::string name{ #property };                 \
        size_t offset{ offsetof(Type, property) };     \
        size_t size{ sizeof(Type::property) };         \
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
        return std::tuple<Property<indices>...>{};                              \
    }                                                                           \
    }                                                                           \
    ;

/**
 * @brief Allows reflection of private members within a class.
 */
#define CLOVE_REFLECT_PRIVATE(classType) \
    friend struct clove::MetaClass<classType>;