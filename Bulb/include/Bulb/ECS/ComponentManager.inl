#include "Bulb/ECS/ECSEvents.hpp"

#include <Clove/Event/EventDispatcher.hpp>

namespace blb::ecs {
	template<typename ComponentType>
	ComponentContainer<ComponentType>::ComponentContainer(clv::EventDispatcher* dispatcher)
		: ecsEventDispatcher(dispatcher)
		, componentAllocator(1000) {
	}

	template<typename ComponentType>
	ComponentContainer<ComponentType>::ComponentContainer(ComponentContainer&& other) noexcept = default;

	template<typename ComponentType>
	ComponentContainer<ComponentType>& ComponentContainer<ComponentType>::operator=(ComponentContainer&& other) noexcept = default;

	template<typename ComponentType>
	ComponentContainer<ComponentType>::~ComponentContainer() = default;

	template<typename ComponentType>
	bool ComponentContainer<ComponentType>::hasComponent(EntityID entityId) const {
		if(auto iter = entityIDToIndex.find(entityId); iter != entityIDToIndex.end()) {
			return components[iter->second] != nullptr;
		} else {
			return false;
		}
	}

	template<typename ComponentType>
	void ComponentContainer<ComponentType>::cloneComponent(EntityID fromId, EntityID toId) {
		if constexpr(std::is_copy_constructible_v<ComponentType>) {
			if(auto iter = entityIDToIndex.find(fromId); iter != entityIDToIndex.end()) {
				ComponentType* componentPtr = components[iter->second];
				addComponent(toId, *componentPtr);
			}
		} else {
			CLV_LOG_ERROR("Component that is not copyable was attempted to be copied. Entity will be incomplete");
		}
	}

	template<typename ComponentType>
	void ComponentContainer<ComponentType>::removeComponent(EntityID entityId) {
		if(auto iter = entityIDToIndex.find(entityId); iter != entityIDToIndex.end()) {
			const size_t index = iter->second;
			const size_t lastIndex = components.size() - 1;

			ComponentType* removedComp = components[index];

			if(index < lastIndex) {
				components[index] = components.back();
			}
			components.pop_back();
			entityIDToIndex.erase(entityId);

			//Update the index map so it knows about the moved component
			if(index < lastIndex) {
				EntityID movedCompEntityID = components[index]->entityID;
				entityIDToIndex[movedCompEntityID] = index;
			}

			ecsEventDispatcher->broadCastEvent(ComponentRemovedEvent<ComponentType>{ removedComp });

			componentAllocator.free(removedComp);
		}
	}

	template<typename ComponentType>
	template<typename... ConstructArgs>
	ComponentPtr<ComponentType> ComponentContainer<ComponentType>::addComponent(EntityID entityId, ConstructArgs&&... args) {
		ComponentType* comp = componentAllocator.alloc(std::forward<ConstructArgs>(args)...);
		if(comp == nullptr) {
			CLV_LOG_ERROR("{0}: Could not create component", CLV_FUNCTION_NAME_PRETTY);
			return { comp };
		}

		comp->entityID = entityId;

		if(auto iter = entityIDToIndex.find(entityId); iter != entityIDToIndex.end()) {
			components[iter->second] = comp;
		} else {
			components.push_back(comp);
			entityIDToIndex[entityId] = components.size() - 1;
		}

		ComponentAddedEvent<ComponentType> event{ comp };
		ecsEventDispatcher->broadCastEvent<ComponentAddedEvent<ComponentType>>(event);

		return { comp };
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType> ComponentContainer<ComponentType>::getComponent(EntityID entityId) {
		if(auto iter = entityIDToIndex.find(entityId); iter != entityIDToIndex.end()) {
			return { components[iter->second] };
		} else {
			return {};
		}
	}

	template<typename ComponentType>
	ComponentContainer<ComponentType>& ComponentManager::getComponentContainer() {
		const ComponentID componentID = ComponentType::id();
		if(auto iter = containers.find(componentID); iter != containers.end()) {
			return static_cast<ComponentContainer<ComponentType>&>(*iter->second.get());
		} else {
			auto container = std::make_unique<ComponentContainer<ComponentType>>(ecsEventDispatcher);

			containers[componentID] = std::move(container);
			return static_cast<ComponentContainer<ComponentType>&>(*containers[componentID].get());
		}
	}
}