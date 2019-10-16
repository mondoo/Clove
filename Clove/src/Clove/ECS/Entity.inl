namespace clv::ecs{
	template<typename ComponentType, typename ...ConstructorArgs>
	ComponentType* Entity::addComponent(ConstructorArgs&& ...args){
		if (entityID != INVALID_ENTITY_ID){
			auto comp = std::make_unique<ComponentType>(std::forward<ConstructorArgs>(args)...);
			ComponentType* compPtr = comp.get();
			onComponentCreated.broadcast(entityID, ComponentType::ID, std::move(comp));
			return compPtr;
		}
		return nullptr;
	}
	
	template<typename ComponentType>
	ComponentType* Entity::getComponent() const{
		if(isValid()){
			return static_cast<ComponentType*>(onComponentRequestedDelegate.broadcast(entityID, ComponentType::ID));
		}
		return nullptr;
	}
}