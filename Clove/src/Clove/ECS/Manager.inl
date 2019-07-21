#include "Clove/ECS/System.hpp"

namespace clv::ecs{
	template<typename... EntityComponents>
	Entity Manager::createEntity(){
		EntityID ID = ++nextID;

		buildComponentMap<0, EntityComponents...>(components[ID]);
		for(const auto& system : systems){
			system->onEntityCreated(ID, components[ID]);
		}

		Entity entity{ ID };
		entity.onComponentRequestedDelegate.bind(&Manager::getComponentForEntity, this);
		entity.isEntityIdValidDelegate.bind(&Manager::isEntityValid, this);

		return entity;
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