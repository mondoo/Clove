#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <type_traits>

namespace garlic::clove {
    template<typename ComponentType>
    ComponentContainer<ComponentType>::ComponentContainer() = default;

    template<typename ComponentType>
    ComponentContainer<ComponentType>::ComponentContainer(ComponentContainer &&other) noexcept = default;

    template<typename ComponentType>
    ComponentContainer<ComponentType> &ComponentContainer<ComponentType>::operator=(ComponentContainer &&other) noexcept;

    template<typename ComponentType>
    ComponentContainer<ComponentType>::~ComponentContainer() = default;

    template<typename ComponentType>
    template<typename... ConstructArgs>
    ComponentType &ComponentContainer<ComponentType>::addComponent(Entity entity, ConstructArgs &&... args) {
        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            components[iter->second] = ComponentType{ std::forward<ConstructArgs>(args)... };

            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Debug, "{0} was called for Entity {1} that alread has that component. Old component is replaced with a new one", CLOVE_FUNCTION_NAME_PRETTY, entity);

            return components[iter->second];
        } else {
            components.emplace_back(ComponentType{ std::forward<ConstructArgs>(args)... });
            entityToIndex[entity] = components.size() - 1;

            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "{0} added new entry for Entity {1}. Index in array is {2}", CLOVE_FUNCTION_NAME_PRETTY, entity, entityToIndex[entity]);

            return components[entityToIndex[entity]];
        }
    }

    template<typename ComponentType>
    ComponentType &ComponentContainer<ComponentType>::getComponent(Entity entity) {
        CLOVE_ASSERT(hasComponent(entity), "{0}: Entity does not have component", CLOVE_FUNCTION_NAME_PRETTY);
        return components[entityToIndex[entity]];
    }

    template<typename ComponentType>
    bool ComponentContainer<ComponentType>::hasComponent(Entity entity) const {
        return entityToIndex.find(entity) != entityToIndex.end();
    }

    template<typename ComponentType>
    void ComponentContainer<ComponentType>::cloneComponent(Entity from, Entity to) {
        if constexpr(std::is_copy_constructible_v<ComponentType>) {
            addComponent(to, getComponent(from));
        } else {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Warning, "{0}: Component is not copy constructable. Entity {1} will be incomplete.", CLOVE_FUNCTION_NAME_PRETTY, to);
        }
    }

    template<typename ComponentType>
    void ComponentContainer<ComponentType>::removeComponent(Entity entity) {
        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            size_t const index{ iter->second };
            size_t const lastIndex{ components.size() - 1 };

            ComponentType removedComp{ std::move(components[index]) };

            if(index < lastIndex) {
                components[index] = std::move(components.back());

                //Update the index map so it knows about the moved component
                for(auto [entityId, componentIndex] : entityToIndex) {
                    if(componentIndex == lastIndex) {
                        entityToIndex[entityId] = index;
                        break;
                    }
                }
            }
            components.pop_back();
            entityToIndex.erase(entity);
        }
    }
}