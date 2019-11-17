#pragma once

#include "Core/ECS/ECSTypes.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Utils/DeltaTime.hpp"

namespace clv::ecs{
	class Entity;
	class Component;
	class System;

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

			template<typename ...ConstructArgs>
			ComponentType* addComponent(EntityID entityId, ConstructArgs&& ...args){
				auto comp = std::make_unique<ComponentType>(std::forward<ConstructArgs>(args)...);
				comp->entityID = entityId;
				components[entityId] = std::move(comp);
				return components[entityId].get();
			}

			ComponentType* getComponent(EntityID entityId){
				if(auto iter = components.find(entityId); iter != components.end()){
					return iter->second.get();
				} else{
					return nullptr;
				}
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
				return static_cast<ComponentContainer<ComponentType>*>(iter->second.get());
			} else{
				containers[componentId] = std::make_unique<ComponentContainer<ComponentType>>();
				return static_cast<ComponentContainer<ComponentType>*>(containers[componentId].get());
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
	public://Temp public
		ComponentManager componentManager;

		std::array<std::unique_ptr<System>, 8> systems;


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

		//Put inside component manager?
		enum class FoundState{
			Good,
			Bad,
			End
		};

		template<std::size_t index, typename ...ComponentTypes>
		FoundState checkForNullptr(const std::tuple<std::add_pointer_t<ComponentTypes>...>& tuple, typename std::enable_if_t<index == sizeof...(ComponentTypes), int> = 0){
			return FoundState::End;
		}

		template<std::size_t index, typename ...ComponentTypes>
		FoundState checkForNullptr(const std::tuple<std::add_pointer_t<ComponentTypes>...>& tuple, typename std::enable_if_t<index < sizeof...(ComponentTypes), int> = 0){
			if(std::get<index>(tuple)){
				return checkForNullptr<index + 1, ComponentTypes...>(tuple);
			} else{
				return FoundState::Bad;
			}
		}

		template<typename ...ComponentTypes>
		std::vector<std::tuple<std::add_pointer_t<ComponentTypes>...>> getComponentSets(){
			std::vector<std::tuple<std::add_pointer_t<ComponentTypes>...>> componentSets;
			for(int32 i = 0; i < nextID; ++i){
				std::tuple<std::add_pointer_t<ComponentTypes>...> tuple = std::make_tuple(componentManager.getComponentContainer<ComponentTypes>()->getComponent(i)...);
				if(checkForNullptr<0, ComponentTypes...>(tuple) != FoundState::Bad){
					componentSets.push_back(tuple);
				}
			}
			return componentSets;
		}
		//~~~~
	};
}