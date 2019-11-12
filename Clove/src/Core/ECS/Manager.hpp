#pragma once

#include "Core/ECS/ECSTypes.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Utils/DeltaTime.hpp"

namespace clv::ecs{
	class Entity;
	class Component;
	class SystemBase;

	class Manager{
		//VARIABLES
	private:
		/* goals:
		 * Systems aren't tied to components they weirdly template, should be able to get all entities with a - z component
		 * Clients can easily add new components and systems
		 * Try and allocate components contiguously
		 * */

		std::unordered_map<EntityID, std::unordered_map<ComponentID, std::unique_ptr<Component>>> components;
		std::array<std::unique_ptr<SystemBase>, 8> systems;


		static EntityID nextID; //TODO: have a better system for generating and reusing IDs

		//FUNCTIONS
	public:
		Manager();

		Manager(const Manager& other) = delete;
		Manager(Manager&& other) noexcept = delete;

		Manager& operator=(const Manager& other) = delete;
		Manager& operator=(Manager&& other) noexcept = delete;

		~Manager();

		void update(utl::DeltaTime deltaTime);

		Entity createEntity();
		void destroyEntity(EntityID ID);
		Entity getEntity(EntityID ID);

		template<typename ...Components>
		std::vector<Entity> getEntitiesWithComponents(){
			//TODO:
			//Can pass in: <MeshComponent, TransformComponent> returns all entities with at least those 2
			//These will be iterated a bunch, any easy wins?
			//Challenge here is how to store the components
		}

	private:
		void onEntityCreateComponent(EntityID entityID, ComponentID componentID, std::unique_ptr<Component> component);
		Component* getComponentForEntity(EntityID entityID, ComponentID componentID);
		bool isEntityValid(EntityID entityID);

		void bindEntity(Entity& entity);
	};
}