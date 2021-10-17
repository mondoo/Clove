#pragma once

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
    };
}

#define CLOVE_REFLECT_BEGIN(ClassType)                            \
    template<>                                                    \
    class clove::MetaClass<ClassType> {                          \
    private:                                                      \
        std::vector<clove::MetaProperty> properties{};            \
                                                                  \
    public:                                                       \
        std::vector<clove::MetaProperty> const &getProperties() { \
            return properties;                                    \
        }                                                         \
                                                                  \
    public:                                                       \
        MetaClass() {
#define CLOVE_REFLECT_PROPERTY(Property) \
    properties.push_back({});

#define CLOVE_REFLECT_END \
    }                     \
    }                     \
    ;