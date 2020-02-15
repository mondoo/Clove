namespace tnc::ecs{
	template<typename ComponentType, typename ...ConstructArgs>
	ComponentType* World::addComponent(EntityID entityID, ConstructArgs&& ...args){
		return componentManager.getComponentContainer<ComponentType>()->addComponent(entityID, args...);
	}

	template<typename ComponentType>
	ComponentType* World::getComponent(EntityID entityID){
		return componentManager.getComponentContainer<ComponentType>()->getComponent(entityID);
	}

	template<typename ComponentType>
	void World::removeComponent(EntityID entityID){
		componentManager.getComponentContainer<ComponentType>()->removeComponent(entityID);
	}

	template<typename ...ComponentTypes>
	std::vector<std::tuple<std::add_pointer_t<ComponentTypes>...>> World::getComponentSets(){
		CLV_PROFILE_FUNCTION();

		std::vector<std::tuple<std::add_pointer_t<ComponentTypes>...>> componentSets;
		for(EntityID entityID : activeIDs){
			std::tuple<std::add_pointer_t<ComponentTypes>...> tuple = std::make_tuple(componentManager.getComponentContainer<ComponentTypes>()->getComponent(entityID)...);
			if(checkForNullptr<0, ComponentTypes...>(tuple) != FoundState::NullptrFound){
				componentSets.push_back(tuple);
			}
		}
		return componentSets;
	}

	template<typename SystemType, typename ...ConstructArgs>
	void World::addSystem(ConstructArgs&& ...args){
		auto system = std::make_unique<SystemType>(std::forward<ConstructArgs>(args)...);
		systems.push_back(std::move(system));
	}

	template<std::size_t index, typename ...ComponentTypes>
	World::FoundState World::checkForNullptr(const std::tuple<std::add_pointer_t<ComponentTypes>...>& tuple, typename std::enable_if_t<(index == sizeof...(ComponentTypes)), int>){
		return FoundState::EndOfTuple;
	}

	template<std::size_t index, typename ...ComponentTypes>
	World::FoundState World::checkForNullptr(const std::tuple<std::add_pointer_t<ComponentTypes>...>& tuple, typename std::enable_if_t<(index < sizeof ...(ComponentTypes)), int>){
		if(std::get<index>(tuple)){
			return checkForNullptr<index + 1, ComponentTypes...>(tuple);
		} else{
			return FoundState::NullptrFound;
		}
	}
}