#include <algorithm>

namespace garlic::clove {
    template<typename DerivedClassType>
    Component<DerivedClassType>::Component() = default;

    template<typename DerivedClassType>
    Component<DerivedClassType>::Component(Component const &other) = default;

    template<typename DerivedClassType>
    Component<DerivedClassType>::Component(Component &&other) noexcept = default;

    template<typename DerivedClassType>
    Component<DerivedClassType> &Component<DerivedClassType>::operator=(Component const &other) = default;

    template<typename DerivedClassType>
    Component<DerivedClassType> &Component<DerivedClassType>::operator=(Component &&other) noexcept = default;

    template<typename DerivedClassType>
    Component<DerivedClassType>::~Component() = default;

    template<typename DerivedClassType>
    ComponentID Component<DerivedClassType>::id() {
        return typeid(DerivedClassType).hash_code();
    }

    template<typename DerivedClassType>
    ComponentID Component<DerivedClassType>::getComponentID() const {
        return id();
    }

    template<typename DerivedClassType>
    Entity Component<DerivedClassType>::getEntity() const {
        return entity;
    }
}