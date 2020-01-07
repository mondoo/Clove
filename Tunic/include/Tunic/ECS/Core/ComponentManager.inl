namespace tnc::ecs{
	template<typename ComponentType>
	ComponentManager::ComponentContainer<ComponentType>::ComponentContainer()
		: componentAllocator(100){
	}

	template<typename ComponentType>
	ComponentManager::ComponentContainer<ComponentType>::ComponentContainer(const ComponentContainer& other) = default;

	template<typename ComponentType>
	ComponentManager::ComponentContainer<ComponentType>::ComponentContainer(ComponentContainer&& other) noexcept = default;

	template<typename ComponentType>
	ComponentManager::ComponentContainer<ComponentType>& ComponentManager::ComponentContainer<ComponentType>::operator=(const ComponentContainer& other) = default;

	template<typename ComponentType>
	ComponentManager::ComponentContainer<ComponentType>& ComponentManager::ComponentContainer<ComponentType>::operator=(ComponentContainer&& other) noexcept = default;

	template<typename ComponentType>
	ComponentManager::ComponentContainer<ComponentType>::~ComponentContainer() = default;

	template<typename ComponentType>
	template<typename ...ConstructArgs>
	ComponentType* ComponentManager::ComponentContainer<ComponentType>::addComponent(EntityID entityID, ConstructArgs&& ...args){
		ComponentType* comp = componentAllocator.alloc(std::forward<ConstructArgs>(args)...);
		comp->entityID = entityID; //TODO: make this container a friend of the component now it's typed
		
		if(auto iter = entityIDToIndex.find(entityID); iter != entityIDToIndex.end()){
			components[iter->second] = comp;
		} else{
			components.push_back(comp);
			entityIDToIndex[entityID] = components.size() - 1;
		}

		componentAddedDelegate.broadcast(comp);

		return comp;
	}

	template<typename ComponentType>
	void ComponentManager::ComponentContainer<ComponentType>::cloneComponent(EntityID fromID, EntityID toID){
		if(auto iter = entityIDToIndex.find(fromID); iter != entityIDToIndex.end()){
			ComponentType* componentPtr = components[iter->second];
			addComponent(toID, *componentPtr);
		}
	}

	template<typename ComponentType>
	ComponentType* ComponentManager::ComponentContainer<ComponentType>::getComponent(EntityID entityID){
		if(auto iter = entityIDToIndex.find(entityID); iter != entityIDToIndex.end()){
			return components[iter->second];
		} else{
			return nullptr;
		}
	}

	template<typename ComponentType>
	void ComponentManager::ComponentContainer<ComponentType>::removeComponent(EntityID entityID){
		if(auto iter = entityIDToIndex.find(entityID); iter != entityIDToIndex.end()){
			const size_t index = iter->second;
			const size_t lastIndex = components.size() - 1;

			ComponentType* removedComp = components[index];

			if(index < lastIndex){
				components[index] = components.back();
			}
			components.pop_back();
			entityIDToIndex.erase(entityID);

			//Update the index map so it knows about the moved component
			if(index < lastIndex){
				EntityID movedCompEntityID = components[index]->entityID;
				entityIDToIndex[movedCompEntityID] = index;
			}

			componentRemovedDelegate.broadcast(removedComp);
			componentAllocator.free(removedComp);
		}
	}

	template<typename ComponentType>
	ComponentManager::ComponentContainer<ComponentType>* ComponentManager::getComponentContainer(){
		const ComponentID componentID = ComponentType::id();
		if(auto iter = containers.find(componentID); iter != containers.end()){
			return static_cast<ComponentContainer<ComponentType>*>(iter->second.get());
		} else{
			auto container = std::make_unique<ComponentContainer<ComponentType>>();
			container->componentAddedDelegate.bind(&ComponentManager::onContainerAddedComponent, this);
			container->componentRemovedDelegate.bind(&ComponentManager::onContainerRemovedComponent, this);

			containers[componentID] = std::move(container);
			return static_cast<ComponentContainer<ComponentType>*>(containers[componentID].get());
		}
	}
}