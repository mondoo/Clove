#pragma once

#include "Clove/ECS/ECSTypes.hpp"
#include "Clove/ECS/Entity.hpp"

namespace clv::gfx{
	class Renderer;
}

namespace clv::ecs{
	class Entity;
	class Component;
	class SystemBase;

	class Manager{
		friend class Entity;
		template<typename T> friend class ComponentPtr;

		//VARIABLES
	private:
		std::unordered_map<EntityID, std::unordered_map<ComponentID, std::unique_ptr<Component>>> components;
		std::vector<std::unique_ptr<SystemBase>> systems;

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

		template<typename... EntityComponents>
		Entity createEntity();
		void destroyEntity(EntityID ID);
		Entity getEntity(EntityID ID);

	private:
		template<size_t index, typename EntityComponent, typename... EntityComponents>
		void buildComponentMap(std::unordered_map<ComponentID, std::unique_ptr<Component>>& map);
		template<size_t index>
		void buildComponentMap(std::unordered_map<ComponentID, std::unique_ptr<Component>>& map);

		Component* getComponentForEntity(EntityID entityID, ComponentID componentID);
		bool isEntityValid(EntityID entityID);
	};
}

#include "Manager.inl"