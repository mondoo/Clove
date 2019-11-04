#include "Core/ECS/Component.hpp"

namespace clv::ecs{
	template<typename ...ComponentTypes>
	 System<ComponentTypes...>::System() = default;

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
	System<ComponentTypes...>::~System() = default;

	template<typename ...ComponentTypes>
	void System<ComponentTypes...>::onEntityComponentAdded(EntityID entity, const std::unordered_map<ComponentID, std::unique_ptr<Component>>& entityComponents){
		if(entityIdToIndexMap.find(entity) != entityIdToIndexMap.end()){
			return;
		}
		
		ComponentTuple comptuple;
		size_t matchingComps = 0;
		for(const auto& [compID, comp] : entityComponents){
			if(proccessEntityComponent<0, ComponentTypes...>(compID, comp.get(), comptuple)){
				++matchingComps;
				if(matchingComps == sizeof...(ComponentTypes)){
					components.emplace_back(comptuple);
					entityIdToIndexMap[entity] = components.size() - 1;
					handleEntityCreation(comptuple);
					break;
				}
			}
		}
	}

	template<typename ...ComponentTypes>
	void System<ComponentTypes...>::onEntityDestroyed(EntityID entity){
		if(const auto findIt = entityIdToIndexMap.find(entity); findIt != entityIdToIndexMap.end()){
			const std::size_t componentIndex = findIt->second;

			handleEntityDestruction(components[componentIndex]);

			components[componentIndex] = std::move(components.back());
			components.pop_back();

			if(componentIndex < components.size()){
				const auto& movedComponent = std::get<0>(components[componentIndex]); //We just need any component to do the look up
				auto movedTupleIdIndex = entityIdToIndexMap.find(movedComponent->entityID);

				CLV_ASSERT(movedTupleIdIndex != entityIdToIndexMap.end(), "Entity ID not tracked!");
				movedTupleIdIndex->second = componentIndex;
			}
		}
	}

	template<typename ...ComponentTypes>
	template<size_t index, typename ComponentType, typename... ComponentArgs>
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
