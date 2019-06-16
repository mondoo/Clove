namespace clv::ecs{
	template<typename T>
	T* Manager::getSystem(){
		if(auto foundSystem = systems.find(T::ID); foundSystem != systems.end()){
			return static_cast<T*>(foundSystem->second.get());
		}
		return nullptr;
	}

	template<typename... EntityComponents>
	EntityPtr Manager::createEntity(){
		EntityID ID = ++nextID;

		buildComponentMap<0, EntityComponents...>(components[ID]);
		for(const auto& [sysID, system] : systems){
			system->onEntityCreated(ID, components[ID]);
		}

		return { this, ID };
	}

	template<size_t index, typename EntityComponent, typename... EntityComponents>
	void Manager::buildComponentMap(std::unordered_map<ComponentID, std::unique_ptr<Component>>& map){
		map[EntityComponent::ID] = std::make_unique<EntityComponent>();
		buildComponentMap<index + 1, EntityComponents...>(map);
	}

	template<size_t index>
	void Manager::buildComponentMap(std::unordered_map<ComponentID, std::unique_ptr<Component>>& map){
		//End of recursion
	}
}