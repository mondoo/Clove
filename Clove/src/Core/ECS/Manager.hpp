#pragma once

#include "Core/ECS/ECSTypes.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Utils/DeltaTime.hpp"

namespace clv::ecs{
	class Entity;
	class Component;
	class SystemBase;

	//TODO: Move out
	class ComponentContainerBase{

	};

	template<typename ComponentType>
	class ComponentContainer{
		//VARIABLES
	public: //private
		std::vector<ComponentType*> components;

		//FUNCTIONS
	public:
		//TODO: ctors

		//Create component (future allocator)
	};
	//~~~

	class Manager{
		//VARIABLES
	private:
		/* goals:
		 * Systems aren't tied to components they weirdly template, should be able to get all entities with a - z component
		 * Clients can easily add new components and systems
		 * Try and allocate components contiguously
		 * */

		std::unordered_map<EntityID, std::unordered_map<ComponentID, Component*>> components;
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
		std::vector<std::tuple<std::add_pointer_t<Components>...>> getComponentSets(){
			/* Loop through components, get entityID
			 * find the other components with said entity ID
			 * build tuple with components
			 * end when all components have been found
			 * */

			//Will need to store the components as their type

			//Loop through entities and smash in the tuples that matter
			//-This will require entities to be stored in a different way
			//--Although entites should just be an id
			//--Might have to go back to the old way of storing them

			for(auto [entity, comps] : components){
				//std::get ????
			}
		}

	private:
		void onEntityCreateComponent(EntityID entityID, ComponentID componentID, std::unique_ptr<Component> component);
		Component* getComponentForEntity(EntityID entityID, ComponentID componentID);
		bool isEntityValid(EntityID entityID);

		void bindEntity(Entity& entity);
	};
}