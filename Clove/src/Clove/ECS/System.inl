namespace clv::ecs{
	template<typename ...ComponentTypes>
	System<ComponentTypes...>::System(System&& other) noexcept{
		components = std::move(other.components);
		entityIdToIndexMap = std::move(other.entityIdToIndexMap);
	}

	template<typename ...ComponentTypes>
	System<ComponentTypes...>& System<ComponentTypes...>::operator=(System&& other) noexcept{
		components = std::move(other.components);
		entityIdToIndexMap = std::move(other.entityIdToIndexMap);

		return *this;
	}

	template<typename ...ComponentTypes>
	void System<ComponentTypes...>::onEntityCreated(const Entity& entity){
		ComponentTuple comptuple;
		size_t matchingComps = 0;
		for(const auto& [compID, comp] : entity.getComponents()){
			if(proccessEntityComponent<0, ComponentTypes...>(compID, comp.get(), comptuple)){
				++matchingComps;
				if(matchingComps == sizeof...(ComponentTypes)){
					components.emplace_back(comptuple);
					entityIdToIndexMap[entity.getID()] = components.size() - 1;
					break;
				}
			}
		}
	}

	template<typename ...ComponentTypes>
	void System<ComponentTypes...>::onEntityDestroyed(const Entity& entity){
		const auto findIt = entityIdToIndexMap.find(entity.getID());
		if(findIt != entityIdToIndexMap.end()){
			components[findIt->second] = std::move(components.back());
			components.pop_back();

			if(findIt->second < components.size()){
				const auto& movedComponent = std::get<0>(components[findIt->second]);
				auto movedTupleIt = entityIdToIndexMap.find(movedComponent->entityID);

				CLV_ASSERT(movedTupleIt != entityIdToIndexMap.end(), "Entity ID not tracked!");
				movedTupleIt->second = findIt->second;
			}
		}
	}

	template<typename ...ComponentTypes>
	template<size_t index, typename ComponentType, typename ...ComponentArgs>
	bool System<ComponentTypes...>::proccessEntityComponent(ComponentID componentID, Component* component, ComponentTuple& tupleToFill){
		if(ComponentType::ID == componentID){
			std::get<index>(tupleToFill) = static_cast<ComponentType*>(component);
			return true;
		} else{
			return proccessEntityComponent<index + 1, ComponentArgs...>(componentID, component, tupleToFill);
		}
	}

	template<typename ...ComponentTypes>
	template<size_t index>
	bool System<ComponentTypes...>::proccessEntityComponent(ComponentID componentID, Component* component, ComponentTuple& tupleToFill){
		return false;
	}
}
