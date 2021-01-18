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

#define CLOVE_REFLECT_CLASS_MEMBER(classType, memberName) \
    Member {                                              \
        .name   = #memberName,                            \
        .size   = sizeof(classType::memberName),          \
        .offset = offsetof(classType, memberName)         \
    }

#define CLOVE_REFLECT_CLASS(classType, ...)   \
    namespace garlic::clove {                 \
        template<>                            \
        Class getClass<classType>() {         \
            return {                          \
                .name    = #classType,        \
                .size    = sizeof(classType), \
                .members = {                  \
                    __VA_ARGS__ }             \
            };                                \
        }                                     \
    }
