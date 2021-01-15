#pragma once

#include <string>
#include <vector>

namespace garlic::clove {
    struct Member {
        std::string name{};
        size_t size{};
        size_t offset{};
    };

    struct Class {
        std::string name{};
        size_t size{};
        std::vector<Member> members;
    };
}

namespace garlic::clove {
    template<typename T>
    Class getClass() {
        //TODO: Error
        return {};
    }
}

#define CLOVE_REFLECT_CLASS_MEMBER(classType, member) \
    Member {                                          \
        .name   = #member,                            \
        .size   = sizeof(classType::member),          \
        .offset = offsetof(classType, member)         \
    }

#define CLOVE_REFLECT_CLASS(class, ...) \
    namespace garlic::clove {           \
        template<>                      \
        Class getClass<class>() {       \
            return {                    \
                .name = #class,         \
                .size = sizeof(class),  \
                                        \
            };                          \
        }                               \
    }
