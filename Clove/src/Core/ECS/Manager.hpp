#pragma once

#include "Core/ECS/ECSTypes.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Utils/DeltaTime.hpp"

namespace clv::ecs{
	class Entity;
	class Component;
	class SystemBase;

	//TODO: Move out
	class ComponentManager{
	private:
		class IComponentContainer{

		};

		template<typename ComponentType>
		class ComponentContainer : public IComponentContainer{
		//private:
		public:

			std::unordered_map<EntityID, std::unique_ptr<ComponentType>> components;

			ComponentType* getComponent(EntityID entityId){
				if(auto iter = components.find(entityId); iter != components.end()){
					return iter->second.get();
				}else{
					return nullptr;
				}
			}

			template<typename ...ConstructArgs>
			ComponentType* addComponent(EntityID entityId, ConstructArgs&& ...args){
				components[entityId] = std::make_unique<ComponentType>(std::forward<ConstructArgs>(args)...);
				return components[entityId].get();
			}
		};

		//VARIABLES
	public: //private
		//std::vector<ComponentType*> components;
		std::unordered_map<ComponentID, std::unique_ptr<IComponentContainer>> containers;

		//FUNCTIONS
	public:
		//TODO: ctors

		template<typename ComponentType>
		ComponentContainer<ComponentType>* getComponentContainer(){
			const ComponentID componentId = ComponentType::ID;
			if(auto iter = containers.find(componentId); iter != containers.end()){
				return iter->second.get();
			}else{
				containers[componentId] = std::make_unique<ComponentContainer<ComponentType>>();
				return containers[componentId].get();
			}
		}
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

		//std::unordered_map<EntityID, std::unordered_map<ComponentID, Component*>> components;
		ComponentManager componentManager;

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

		template<typename ...ComponentTypes>
		std::vector<std::tuple<std::add_pointer_t<ComponentTypes>...>> getComponentSets(){
			//std::array<ComponentID> componentIDs = { Components::ID... };
			std::vector<std::tuple<std::add_pointer_t<ComponentTypes>...>> componentSets;
			for(int32 i = 0; i < nextID; ++i){
				auto set = getComponentsForEntity()<ComponentTypes...>(i);
				if(set){
					componentSets.push_back(set.value());
				}
			}
			return componentSets;
		}

		template<typename ...ComponentTypes>
		std::optional<std::tuple<std::add_pointer_t<ComponentTypes>...>> getComponentsForEntity(EntityID){

		}

		template<size_t index, typename ComponentType, typename... ComponentTypes>
		bool proccessEntityComponent(std::tuple<std::add_pointer_t<ComponentTypes>...>& tupleToFill){
			if(ComponentType::ID == componentID){
				//std::get<index>(tupleToFill) = static_cast<ComponentType*>(component);
				return true;
			} else{
				return proccessEntityComponent<index + 1, ComponentArgs...>(componentID, component, tupleToFill);
			}
		}

		template<size_t index>
		bool proccessEntityComponent(ComponentID componentID, Component* component, ComponentTuple& tupleToFill){
			return false;
		}
	private:
		void onEntityCreateComponent(EntityID entityID, ComponentID componentID, std::unique_ptr<Component> component);
		Component* getComponentForEntity(EntityID entityID, ComponentID componentID);
		bool isEntityValid(EntityID entityID);

		void bindEntity(Entity& entity);
	};
}