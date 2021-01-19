#pragma once

#include "Clove/Reflection/Common.hpp"

#define CLOVE_REFLECT_CLASS_MEMBER(classType, memberName) \
    Member {                                              \
        .name   = #memberName,                            \
        .size   = sizeof(classType::memberName),          \
        .offset = offsetof(classType, memberName)         \
    }

#define CLOVE_REFLECT_CLASS(classType, ...)                  \
    namespace garlic::clove {                                \
        template<>                                           \
        Class getClass<classType>(classType const &object) { \
            return {                                         \
                .name    = #classType,                       \
                .size    = sizeof(classType),                \
                .members = {                                 \
                    __VA_ARGS__ }                            \
            };                                               \
        }                                                    \
    }
