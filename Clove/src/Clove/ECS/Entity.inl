namespace clv::ecs{
	template<typename ComponentType>
	ComponentType* Entity::getComponent() const{
		if(isValid()){
			return static_cast<ComponentType*>(onComponentRequestedDelegate.broadcast(entityID, ComponentType::ID));
		}
		return nullptr;
	}
}