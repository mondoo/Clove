#pragma once

#include "Clove/Reflection/Common.hpp"

#define CLOVE_REFLECT_TYPE(type)                 \
    namespace garlic::clove {                    \
        template<>                               \
        Type reflect<type>(type const &object) { \
            using ReflectType = type;            \
            Type reflectedType {                 \
                .name   = #type,                 \
                .size   = sizeof(type),          \
                .fields = {
#define CLOVE_REFLECT_FIELD(member)              \
    Field{                                       \
        .name   = #member,                       \
        .size   = sizeof(ReflectType::member),   \
        .offset = offsetof(ReflectType, member), \
        .value  = object.member,                 \
    },
#define CLOVE_REFLECT_END \
    }                     \
    ,                     \
    }                     \
    ;                     \
    return reflectedType; \
    }                     \
    }
