#include "Clove/Reflection/Reflection.hpp"

#include <Clove/Log/Log.hpp>

CLOVE_DECLARE_LOG_CATEGORY(CloveReflection)

namespace clove::reflection {
    TypeInfo const *getTypeInfo(std::string_view typeName) {
        for(auto &&[typeId, typeInfo] : internal::Registry::get().getRegisteredTypes()) {
            if(typeInfo.name == typeName) {
                return &typeInfo;
            }
        }

        return nullptr;
    }

    TypeInfo const *getTypeInfo(TypeId typeId) {
        auto const &registeredTypes{ internal::Registry::get().getRegisteredTypes() };

        if(registeredTypes.contains(typeId)) {
            return &registeredTypes.at(typeId);
        } else {
            return nullptr;
        }
    }

    namespace internal {
        Registry::Registry() = default;

        Registry::~Registry() = default;

        Registry &Registry::get() {
            static Registry *instance{ nullptr };
            if(instance == nullptr) {
                instance = new Registry{};
            }

            return *instance;
        }

        void Registry::addTypeInfo(TypeId const typeId, reflection::TypeInfo typeInfo) {
            if(!types.contains(typeId)) {
                types.emplace(typeId, std::move(typeInfo));
            } else {
                CLOVE_LOG(CloveReflection, LogLevel::Warning, "TypeId {0} ({1}) already exists in registry. Ignoring potential duplicate.", typeId, typeInfo.name);
            }
        }
    }
}