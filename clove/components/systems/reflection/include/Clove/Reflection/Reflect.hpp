#pragma once

#include <string>

namespace garlic::clove {
    struct Class {
        std::string name{};
        size_t size{};
        //TODO: Members etc.
    };
}

namespace garlic::clove {
    template<typename T>
    Class getClass() {
        //TODO: Error
        return {};
    }
}

#define CLOVE_REFLECT_CLASS(class)    \
    namespace garlic::clove {         \
        template<>                    \
        Class getClass<class>() {     \
            return {                  \
                .name = #class,       \
                .size = sizeof(class) \
            };                        \
        }                             \
    }
