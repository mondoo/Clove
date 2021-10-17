#pragma once

#include <string>
#include <vector>

namespace clove {
    /**
     * @brief Provides information about a specific class.
     */
    template<typename T>
    class MetaClass;

    /**
     * @brief Provides information about a property for a class.
     */
    struct MetaProperty {
        std::string name{}; /**< Name of the property. */
        size_t offset{ 0 }; /**< Offset of the property within the class. */
        size_t size{ 0 };   /**< Size of the property. */
    };
}

#define CLOVE_REFLECT_BEGIN(classType)                            \
    template<>                                                    \
    class clove::MetaClass<classType> {                           \
    private:                                                      \
        using Type = classType;                                   \
                                                                  \
        std::vector<clove::MetaProperty> properties{};            \
                                                                  \
    public:                                                       \
        std::vector<clove::MetaProperty> const &getProperties() { \
            return properties;                                    \
        }                                                         \
                                                                  \
    public:                                                       \
        MetaClass() {
            
#define CLOVE_REFLECT_PROPERTY(property)    \
    properties.push_back({                  \
        .name   = #property,                \
        .offset = offsetof(Type, property), \
        .size   = sizeof(Type::property),   \
    });

#define CLOVE_REFLECT_END \
    }                     \
    }                     \
    ;