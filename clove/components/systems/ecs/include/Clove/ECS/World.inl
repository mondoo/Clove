#include <Clove/Timer.hpp>

namespace garlic::clove {
    template<typename ComponentType, typename... ConstructArgs>
    ComponentPtr<ComponentType> World::addComponent(Entity entity, ConstructArgs &&... args) {
        return componentManager.getComponentContainer<ComponentType>().addComponent(entity, std::forward<ConstructArgs>(args)...);
    }

    template<typename ComponentType>
    ComponentPtr<ComponentType> World::getComponent(Entity entity) {
        return componentManager.getComponentContainer<ComponentType>().getComponent(entity);
    }

    template<typename ComponentType>
    bool World::hasComponent(Entity entity) {
        return componentManager.getComponentContainer<ComponentType>().hasComponent(entity);
    }

    template<typename ComponentType>
    void World::removeComponent(Entity entity) {
        componentManager.getComponentContainer<ComponentType>().removeComponent(entity);
    }

    template<typename... ComponentTypes>
    ComponentSet<ComponentTypes...> World::getComponentSets() {
        CLOVE_PROFILE_FUNCTION();

        std::vector<std::tuple<ComponentPtr<ComponentTypes>...>> componentSets;
        for(Entity entity : activeEntities) {
            std::tuple<ComponentPtr<ComponentTypes>...> tuple = std::make_tuple(getComponent<ComponentTypes>(entity)...);
            if(checkForNullptr<0, ComponentTypes...>(tuple) != FoundState::NullptrFound) {
                componentSets.push_back(std::move(tuple));
            }
        }
        return componentSets;
    }

    template<typename SystemType, typename... ConstructArgs>
    SystemType *World::addSystem(ConstructArgs &&... args) {
        auto system  = std::make_unique<SystemType>(std::forward<ConstructArgs>(args)...);
        auto *retSys = system.get();
        system->registerToEvents(ecsEventDispatcher);
        systems.push_back(std::move(system));
        return retSys;
    }

    template<std::size_t index, typename... ComponentTypes>
    World::FoundState World::checkForNullptr(const std::tuple<ComponentPtr<ComponentTypes>...> &tuple) {
        if constexpr(index < sizeof...(ComponentTypes)) {
            if(std::get<index>(tuple)) {
                return checkForNullptr<index + 1, ComponentTypes...>(tuple);
            } else {
                return FoundState::NullptrFound;
            }
        } else {
            return FoundState::EndOfTuple;
        }
    }
}