namespace clv::ecs{
	template<typename T>
	inline T* Entity::getComponent() const{
		if(const auto foundComp = components.find(T::ID); foundComp != components.end()){
			return static_cast<T*>(foundComp->second.get());
		}
		return nullptr;
	}

	template<typename T>
	inline T* Entity::addComponent(){
		std::unique_ptr<T> component = std::make_unique<T>();
		component->entityID = ID;

		T* outPTr = component.get();
		components[T::ID] = std::move(component);

		return outPTr;
	}
}