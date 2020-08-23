namespace blb::ecs{
	template<typename ComponentType, typename ...ConstructArgs>
	ComponentPtr<ComponentType> World::addComponent(EntityID entityId, ConstructArgs&&... args) {
		return componentManager.getComponentContainer<ComponentType>().addComponent(entityId, std::forward<ConstructArgs>(args)...);
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType> World::getComponent(EntityID entityId) {
		return componentManager.getComponentContainer<ComponentType>().getComponent(entityId);
	}

	template<typename ComponentType>
	bool World::hasComponent(EntityID entityId) {
		return componentManager.getComponentContainer<ComponentType>().hasComponent(entityId);
	}

	template<typename ComponentType>
	void World::removeComponent(EntityID entityId) {
		componentManager.getComponentContainer<ComponentType>().removeComponent(entityId);
	}

	template<typename ...ComponentTypes>
    ComponentSet<ComponentTypes...> World::getComponentSets() {
		CLV_PROFILE_FUNCTION();

		std::vector<std::tuple<ComponentPtr<ComponentTypes>...>> componentSets;
		for (EntityID entityID : activeIDs){
			std::tuple<ComponentPtr<ComponentTypes>...> tuple = std::make_tuple(getComponent<ComponentTypes>(entityID)...);
			if (checkForNullptr<0, ComponentTypes...>(tuple) != FoundState::NullptrFound){
				componentSets.push_back(std::move(tuple));
			}
		}
		return componentSets;
	}

	template<typename SystemType, typename ...ConstructArgs>
	void World::addSystem(ConstructArgs&& ...args){
		auto system = std::make_unique<SystemType>(std::forward<ConstructArgs>(args)...);
		system->registerToEvents(ecsEventDispatcher);
		systems.push_back(std::move(system));
	}

	template<std::size_t index, typename ...ComponentTypes>
	World::FoundState World::checkForNullptr(const std::tuple<ComponentPtr<ComponentTypes>...>& tuple){
		if constexpr(index < sizeof...(ComponentTypes)) {
			if(std::get<index>(tuple)) {
				return checkForNullptr<index + 1, ComponentTypes...>(tuple);
			} else {
				return FoundState::NullptrFound;
			}
		} else {
			return FoundState::EndOfTuple;
		}
	}
}