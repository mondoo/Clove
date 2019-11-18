#include "Manager.hpp"
namespace clv::ecs{
	template<typename ComponentType, typename ...ConstructArgs>
	ComponentType* Manager::addComponent(EntityID entityID, ConstructArgs&& ...args){
		ComponentType* component = componentManager.getComponentContainer<ComponentType>()->addComponent(entityID, args...);

		std::for_each(systems.begin(), systems.end(), [component](const std::unique_ptr<System>& system){
			system->onComponentCreated(component);
		});

		return component;
	}

	template<typename ComponentType>
	ComponentType* Manager::getComponent(EntityID entityID){
		return componentManager.getComponentContainer<ComponentType>()->getComponent(entityID);
	}

	template<typename ...ComponentTypes>
	std::vector<std::tuple<std::add_pointer_t<ComponentTypes>...>> Manager::getComponentSets(){
		std::vector<std::tuple<std::add_pointer_t<ComponentTypes>...>> componentSets;
		for(int32 i = 0; i < nextID; ++i){
			std::tuple<std::add_pointer_t<ComponentTypes>...> tuple = std::make_tuple(componentManager.getComponentContainer<ComponentTypes>()->getComponent(i)...);
			if(checkForNullptr<0, ComponentTypes...>(tuple) != FoundState::NullptrFound){
				componentSets.push_back(tuple);
			}
		}
		return componentSets;
	}

	template<std::size_t index, typename ...ComponentTypes>
	Manager::FoundState Manager::checkForNullptr(const std::tuple<std::add_pointer_t<ComponentTypes>...>& tuple, typename std::enable_if_t<(index == sizeof...(ComponentTypes)), int>){
		return FoundState::EndOfTuple;
	}

	template<std::size_t index, typename ...ComponentTypes>
	Manager::FoundState Manager::checkForNullptr(const std::tuple<std::add_pointer_t<ComponentTypes>...>& tuple, typename std::enable_if_t<(index < sizeof ...(ComponentTypes)), int>){
		if(std::get<index>(tuple)){
			return checkForNullptr<index + 1, ComponentTypes...>(tuple);
		} else{
			return FoundState::NullptrFound;
		}
	}
}