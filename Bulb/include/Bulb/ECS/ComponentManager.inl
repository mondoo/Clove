namespace blb::ecs {
	template<typename ComponentType>
	ComponentContainer<ComponentType>::ComponentContainer()
		: componentAllocator(100) {
	}

	template<typename ComponentType>
	ComponentContainer<ComponentType>::ComponentContainer(const ComponentContainer& other) = default;

	template<typename ComponentType>
	ComponentContainer<ComponentType>::ComponentContainer(ComponentContainer&& other) noexcept = default;

	template<typename ComponentType>
	ComponentContainer<ComponentType>& ComponentContainer<ComponentType>::operator=(const ComponentContainer& other) = default;

	template<typename ComponentType>
	ComponentContainer<ComponentType>& ComponentContainer<ComponentType>::operator=(ComponentContainer&& other) noexcept = default;

	template<typename ComponentType>
	ComponentContainer<ComponentType>::~ComponentContainer() = default;

	template<typename ComponentType>
	void ComponentContainer<ComponentType>::cloneComponent(EntityID fromID, EntityID toID) {
		if constexpr(std::is_copy_constructible_v<ComponentType>) {
			if(auto iter = entityIDToIndex.find(fromID); iter != entityIDToIndex.end()) {
				ComponentType* componentPtr = components[iter->second];
				addComponent(toID, *componentPtr);
			}
		} else {
			CLV_LOG_ERROR("Component that is not copyable was attempted to be copied. Entity will be incomplete");
		}
	}

	template<typename ComponentType>
	void ComponentContainer<ComponentType>::removeComponent(EntityID entityID) {
		if(auto iter = entityIDToIndex.find(entityID); iter != entityIDToIndex.end()) {
			const size_t index = iter->second;
			const size_t lastIndex = components.size() - 1;

			ComponentType* removedComp = components[index];

			if(index < lastIndex) {
				components[index] = components.back();
			}
			components.pop_back();
			entityIDToIndex.erase(entityID);

			//Update the index map so it knows about the moved component
			if(index < lastIndex) {
				EntityID movedCompEntityID = components[index]->entityID;
				entityIDToIndex[movedCompEntityID] = index;
			}

			componentRemovedDelegate.broadcast(removedComp);
			componentAllocator.free(removedComp);
		}
	}

	template<typename ComponentType>
	template<typename... ConstructArgs>
	ComponentPtr<ComponentType> ComponentContainer<ComponentType>::addComponent(EntityID entityID, ConstructArgs&&... args) {
		ComponentType* comp = componentAllocator.alloc(std::forward<ConstructArgs>(args)...);
		if(comp == nullptr) {
			CLV_LOG_ERROR("{0}: Could not create component", CLV_FUNCTION_NAME_PRETTY);
			return { comp };
		}

		comp->entityID = entityID;

		if(auto iter = entityIDToIndex.find(entityID); iter != entityIDToIndex.end()) {
			components[iter->second] = comp;
		} else {
			components.push_back(comp);
			entityIDToIndex[entityID] = components.size() - 1;
		}

		componentAddedDelegate.broadcast(comp);

		return { comp };
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType> ComponentContainer<ComponentType>::getComponent(EntityID entityID) {
		if(auto iter = entityIDToIndex.find(entityID); iter != entityIDToIndex.end()) {
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
			auto container = std::make_unique<ComponentContainer<ComponentType>>();
			container->componentAddedDelegate.bind(&ComponentManager::onContainerAddedComponent, this);
			container->componentRemovedDelegate.bind(&ComponentManager::onContainerRemovedComponent, this);

			containers[componentID] = std::move(container);
			return static_cast<ComponentContainer<ComponentType>&>(*containers[componentID].get());
		}
	}
}