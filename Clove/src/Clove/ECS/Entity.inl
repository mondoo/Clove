namespace clv::ecs{
	template<typename ComponentType>
	inline ComponentType* Entity::getComponent() const{
		if(const auto foundComp = components.find(ComponentType::ID); foundComp != components.end()){
			return static_cast<ComponentType*>(foundComp->second.get());
		}
		return nullptr;
	}

	template<typename ComponentType>
	inline ComponentType* Entity::addComponent(){
		std::unique_ptr<ComponentType> component = std::make_unique<ComponentType>();
		component->entityID = ID;

		ComponentType* outPTr = component.get();
		components[ComponentType::ID] = std::move(component);

		return outPTr;
	}

	template<typename ComponentType, typename ...ConstructTypes>
	inline ComponentType* Entity::addComponent(ConstructTypes&&... args){
		std::unique_ptr<ComponentType> component = std::make_unique<ComponentType>(args...);
		component->entityID = ID;

		ComponentType* outPTr = component.get();
		components[ComponentType::ID] = std::move(component);

		return outPTr;
	}
}