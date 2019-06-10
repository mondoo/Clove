namespace clv::ecs{
	template<typename T>
	inline T* Entity::getComponent() const{
		if(const auto foundComp = components.find(T::ID); foundComp != components.end()){
			return static_cast<T*>(foundComp->second);
		}
		return nullptr;
	}
}