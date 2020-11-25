#include "Clove/ECS/World.hpp"

namespace garlic::clove {
    template<typename ComponentType, typename... ConstructorArgs>
    ComponentPtr<ComponentType> Entity::addComponent(ConstructorArgs &&... args) {
        if(isValid()) {
            return world->addComponent<ComponentType>(getID(), std::forward<ConstructorArgs>(args)...);
        } else {
            return {};
        }
    }

    template<typename ComponentType>
    ComponentPtr<ComponentType> Entity::getComponent() const {
        if(isValid()) {
            return world->getComponent<ComponentType>(getID());
        } else {
            return {};
        }
    }

    template<typename ComponentType>
    bool Entity::hasComponent() const {
        if(isValid()) {
            return world->hasComponent<ComponentType>(getID());
        } else {
            return false;
        }
    }

    template<typename ComponentType>
    void Entity::removeComponent() {
        if(isValid()) {
            world->removeComponent<ComponentType>(getID());
        }
    }
}
