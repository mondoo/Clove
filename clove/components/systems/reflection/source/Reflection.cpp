#include "Clove/Reflection/Reflection.hpp"

namespace clove::reflection {
    TypeInfo const *getTypeInfo(std::string_view typeName) {
        for(auto &typeInfo : internal::Registry::get().getRegisteredTypes()) {
            if(typeInfo.name == typeName) {
                return &typeInfo;
            }
        }

        return nullptr;
    }

    TypeInfo const *getTypeInfo(TypeId typeId) {
        for(auto &typeInfo : internal::Registry::get().getRegisteredTypes()) {
            if(typeInfo.id == typeId) {
                return &typeInfo;
            }
        }

        return nullptr;
    }
}