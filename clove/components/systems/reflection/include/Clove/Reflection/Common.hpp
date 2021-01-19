#pragma once

#include <type_traits>

namespace garlic::clove {
    template<typename T>
    struct TypeInfo {};

    template<typename T>
    TypeInfo<T> getTypeInfo(T const &object){
        //TODO: Populate with data
        return TypeInfo<T>{};
    }
}