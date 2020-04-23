namespace blb::ecs{
	template<typename ComponentType, typename ...ConstructArgs>
	ComponentPtr<ComponentType> World::addComponent(EntityID entityID, ConstructArgs&&... args) {
		return componentManager.getComponentContainer<ComponentType>().addComponent(entityID, std::forward<ConstructArgs>(args)...);
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType> World::getComponent(EntityID entityID) {
		return componentManager.getComponentContainer<ComponentType>().getComponent(entityID);
	}

	template<typename ComponentType>
	void World::removeComponent(EntityID entityID){
		componentManager.getComponentContainer<ComponentType>().removeComponent(entityID);
	}

	template<typename ...ComponentTypes>
	std::vector<std::tuple<ComponentPtr<ComponentTypes>...>> World::getComponentSets(){
		CLV_PROFILE_FUNCTION();

		std::vector<std::tuple<ComponentPtr<ComponentTypes>...>> componentSets;
		for(EntityID entityID : activeIDs){
			std::tuple<ComponentPtr<ComponentTypes>...> tuple = std::make_tuple(getComponent<ComponentTypes>(entityID)...);
			if(checkForNullptr<0, ComponentTypes...>(tuple) != FoundState::NullptrFound){
				componentSets.push_back(std::move(tuple));
			}
		}
		return componentSets;
	}

	template<typename SystemType, typename ...ConstructArgs>
	void World::addSystem(ConstructArgs&& ...args){
		systems.push_back(std::make_unique<SystemType>(std::forward<ConstructArgs>(args)...));
	}

	template<std::size_t index, typename ...ComponentTypes>
	World::FoundState World::checkForNullptr(const std::tuple<ComponentPtr<ComponentTypes>...>& tuple, typename std::enable_if_t<(index == sizeof...(ComponentTypes)), int>) {
		return FoundState::EndOfTuple;
	}

	template<std::size_t index, typename ...ComponentTypes>
	World::FoundState World::checkForNullptr(const std::tuple<ComponentPtr<ComponentTypes>...>& tuple, typename std::enable_if_t<(index < sizeof...(ComponentTypes)), int>) {
		if(std::get<index>(tuple)){
			return checkForNullptr<index + 1, ComponentTypes...>(tuple);
		} else{
			return FoundState::NullptrFound;
		}
	}
}