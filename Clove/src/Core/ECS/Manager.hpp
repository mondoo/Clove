#pragma once

#include "Core/ECS/ECSTypes.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Utils/DeltaTime.hpp"

namespace clv::ecs{
	class Entity;
	class Component;
	class SystemBase;

	class Manager{
		friend class Entity;

		//VARIABLES
	private:
		std::unordered_map<EntityID, std::unordered_map<ComponentID, std::unique_ptr<Component>>> components;
		std::array<std::unique_ptr<SystemBase>, 9> systems;

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

	private:
		void onEntityCreateComponent(EntityID entityID, ComponentID componentID, std::unique_ptr<Component> component);
		Component* getComponentForEntity(EntityID entityID, ComponentID componentID);
		bool isEntityValid(EntityID entityID);

		void bindEntity(Entity& entity);
	};
}