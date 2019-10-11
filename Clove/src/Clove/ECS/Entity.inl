namespace clv::ecs{
	template<typename ComponentType, typename ...ConstructorArgs>
	ComponentType* Entity::addComponent(ConstructorArgs&& ...args){
		auto comp = std::make_unique<ComponentType>(std::forward(args)...);
		ComponentType* compPtr = comp.get();
		onComponentCreated.broadcast(entityID, ComponentType::ID, std::move(comp));
		return compPtr;
	}
	
	template<typename ComponentType>
	ComponentType* Entity::getComponent() const{
		if(isValid()){
			return static_cast<ComponentType*>(onComponentRequestedDelegate.broadcast(entityID, ComponentType::ID));
		}
		return nullptr;
	}
}