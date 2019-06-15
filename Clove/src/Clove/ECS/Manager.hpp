#pragma once

#include "Clove/ECS/ECSTypes.hpp"
#include "Clove/ECS/System.hpp"

//Entities are just IDs (with some helper functions)
//This guy manages everything

//The problem I'm trying to fix is that currently entities and components can be dangling 


namespace clv::gfx{
	class Renderer;
}

namespace clv::ecs{
	class Entity;
	class Component;
	class SystemBase;

	class Manager{
		friend struct EntityPtr; //Me being lazy

		//VARIABLES
	private:
		std::unordered_map<EntityID, std::unique_ptr<Entity>> entities;
		//std::unordered_map<EntityID, std::unordered_map<ComponentID, std::unique_ptr<Component>>> components;
		std::unordered_map<SystemID, std::unique_ptr<SystemBase>> systems;

		static EntityID nextID; //TODO: have a better system for generating and reusing IDs

		//FUNCTIONS
	public:
		Manager();
		Manager(const Manager& other) = delete;
		Manager(Manager&& other) noexcept = delete;
		Manager& operator=(const Manager& other) = delete;
		Manager& operator=(Manager&& other) noexcept = delete;
		~Manager();

		void update(float deltaTime);

		template<typename T>
		T* getSystem();

		template<typename T>
		EntityPtr createEntity();
		void destroyEntity(EntityID ID);
		EntityPtr getEntity(EntityID ID); //
	};

	//Temp move to own header / cpp ect.
	struct EntityPtr{ //Lazy work around for now
	protected:
		Manager* manager = nullptr;
		EntityID entityID = INVALID_ENTITY_ID;

	public:
		EntityPtr() = default;
		EntityPtr(Manager* manager, EntityID entityID)
			: manager(manager)
			, entityID(entityID){

		}

		bool isValid() const{
			if(entityID == INVALID_ENTITY_ID){
				return false;
			}

			const auto it = manager->entities.find(entityID);
			return it != manager->entities.end();
		}

		template<typename ComponentType>
		ComponentType* getComponent() const{
			if(isValid()){
				manager->entities[entityID]->getComponent<ComponentType>();
			} else{
				return false;
			}
		}

		EntityID getID(){
			return entityID;
		}
	};

	//Component ptr will do very similar to entity ptr, check manager if entity is alive then get the component from it

	/*template<typename T>
	struct EntityPtr : public EntityPtrBase{
	public:
		Entity* operator->(){
			if(isValid(){

			} else{

			}
		}
	};*/
}

#include "Manager.inl"