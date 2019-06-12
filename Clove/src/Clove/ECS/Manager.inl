namespace clv::ecs{
	template<typename T>
	T* Manager::getSystem(){
		if(auto foundSystem = systems.find(T::ID); foundSystem != systems.end()){
			return static_cast<T*>(foundSystem->second.get());
		}
		return nullptr;
	}

	template<typename T>
	EntityID Manager::createEntity(){
		EntityID ID = ++nextID;

		std::unique_ptr<T> entity = std::make_unique<T>(ID);
		for(const auto& [sysID, system] : systems){
			system->onEntityCreated(*entity);
		}
		entities[ID] = std::move(entity);

		return ID;
	}
}