#pragma once

#include "Clove/ECS/ECSTypes.hpp"
#include "Clove/ECS/System.hpp"

//Entities are just IDs (with some helper functions)
//This guy manages everything

//The problem I'm trying to fix is that currently entities and components can be dangling 


//TEMP: nneed this header for the ID in the pointer
#include "Clove/ECS/Component.hpp"


namespace clv::gfx{
	class Renderer;
}

namespace clv::ecs{
	class Entity;
	class Component;
	class SystemBase;

	class Manager{
		//Me being lazy
		friend struct EntityPtr;
		friend struct ComponentPtrBase;

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
		EntityPtr createEntity(); //Should this just take a list of components to create the entity with? 
		void destroyEntity(EntityID ID);
		EntityPtr getEntity(EntityID ID); //
	};

	//Temp move to own header / cpp ect.
	struct ComponentPtrBase{
	protected:
		Manager* manager = nullptr;
		EntityID entityID = INVALID_ENTITY_ID;

	public:
		ComponentPtrBase() = default;
		ComponentPtrBase(Manager* manager, EntityID entityID)
			: manager(manager)
			, entityID(entityID){

		}

	protected:
		bool isValid(ComponentID ID) const{
			if(entityID == INVALID_ENTITY_ID){
				return false;
			}

			const auto it = manager->entities.find(entityID);
			if(it == manager->entities.end()){
				return false;
			}

			return manager->entities[entityID]->components.find(ID) != manager->entities[entityID]->components.end();
		}

		Component* getComponent(ComponentID ID){
			return manager->entities[entityID]->components[ID].get();
		}
	};

	template<typename T>
	struct ComponentPtr : public ComponentPtrBase{
	public:
		ComponentPtr() = default;
		ComponentPtr(Manager* manager, EntityID entityID)
			: ComponentPtrBase(manager, entityID){

		}
	public:
		bool isValid() const{
			return ComponentPtrBase::isValid(T::ID);
		}

		T* operator->(){
			if(ComponentPtrBase::isValid(T::ID)){
				static_cast<T*>(getComponent(T::ID));
			} else{
				return nullptr;
			}
		}

		//Temp: This is for the add child thing but I want avoid people having access to the pointers underneath
		T* get(){
			if(ComponentPtrBase::isValid(T::ID)){
				static_cast<T*>(getComponent(T::ID));
			} else{
				return nullptr;
			}
		}
	};

	struct EntityPtr{
	private:
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
		ComponentPtr<ComponentType> getComponent() const{
			if(isValid()){
				if(manager->entities[entityID]->getComponent<ComponentType>() != nullptr){
					return { manager, entityID };
				}
			} else{
				return {};
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