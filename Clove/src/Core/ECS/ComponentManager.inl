namespace clv::ecs{
	template<typename ComponentType>
	ComponentManager::ComponentContainer<ComponentType>::ComponentContainer() = default;

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
		auto comp = std::make_unique<ComponentType>(std::forward<ConstructArgs>(args)...);
		comp->entityID = entityID; //TODO: make this container a friend of the component now it's typed
		components[entityID] = std::move(comp);
		return components[entityID].get();
	}

	template<typename ComponentType>
	ComponentType* ComponentManager::ComponentContainer<ComponentType>::getComponent(EntityID entityId){
		if(auto iter = components.find(entityId); iter != components.end()){
			return iter->second.get();
		} else{
			return nullptr;
		}
	}

	template<typename ComponentType>
	ComponentManager::ComponentContainer<ComponentType>* ComponentManager::getComponentContainer(){
		const ComponentID componentID = ComponentType::id();
		if(auto iter = containers.find(componentID); iter != containers.end()){
			return static_cast<ComponentContainer<ComponentType>*>(iter->second.get());
		} else{
			containers[componentID] = std::make_unique<ComponentContainer<ComponentType>>();
			return static_cast<ComponentContainer<ComponentType>*>(containers[componentID].get());
		}
	}
}