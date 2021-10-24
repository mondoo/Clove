#include "Clove/Reflection/Reflection.hpp"

namespace clove::reflection {
    TypeInfo const *getTypeInfo(std::string_view typeName) {
        for(auto &typeInfo : internal::Registry::get().getRegisteredTypes()) {
            if(typeInfo.name == typeName) {
                return &typeInfo;
            }
        }

        return {};
    }
}