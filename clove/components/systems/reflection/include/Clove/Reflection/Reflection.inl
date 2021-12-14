#include <type_traits>

namespace clove::reflection {
    template<typename AttributeType>
    void AttributeContainer::add(AttributeType &&attribute) {
        attributes.setValue(typeid(AttributeType).hash_code(), std::forward<AttributeType>(attribute));
    }

    template<typename AttributeType>
    bool AttributeContainer::contains() const {
        return attributes.hasValue(typeid(AttributeType).hash_code());
    }

    template<typename AttributeType>
    std::optional<AttributeType> AttributeContainer::get() const {
        return attributes.getValue<AttributeType>(typeid(AttributeType).hash_code());
    }

    template<typename Type>
    TypeInfo const *getTypeInfo() {
        return getTypeInfo(typeid(Type).hash_code());
    }

    template<typename AttributeType>
    std::vector<TypeInfo const *> getTypesWithAttribute() {
        std::vector<TypeInfo const *> typeInfos{};

        for(auto &typeInfo : internal::Registry::get().getRegisteredTypes()) {
            if(typeInfo.attributes.contains<AttributeType>()) {
                typeInfos.push_back(&typeInfo);
            }
        }

        return typeInfos;
    }

    namespace internal {
        std::vector<reflection::TypeInfo> const &Registry::getRegisteredTypes() const {
            return types;
        }

        void Registry::addTypeInfo(reflection::TypeInfo typeInfo) {
            types.push_back(std::move(typeInfo));
        }

        template<size_t index, typename TupleType>
        static void populateAttributes(AttributeContainer &container, TupleType const &attributeTuple) {
            if constexpr(index < std::tuple_size_v<TupleType>) {
                container.add(std::get<index>(attributeTuple));

                populateAttributes<index + 1>(container, attributeTuple);
            }
        }
    }
}