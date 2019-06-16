#pragma once

#include "Clove/ECS/ECSTypes.hpp"
#include "Clove/ECS/System.hpp"
#include "Clove/ECS/Entity.hpp"
#include "Clove/ECS/ComponentPtr.hpp"

namespace clv::gfx{
	class Renderer;
}

namespace clv::ecs{
	class Entity;
	class Component;
	class SystemBase;

	class Manager{
		//TODO: Delete, me being lazy
		friend class Entity;
		friend class ComponentPtrBase;

		//VARIABLES
	private:
		std::unordered_map<EntityID, std::unordered_map<ComponentID, std::unique_ptr<Component>>> components;
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
		T* getSystem(); //TODO: I don't really want people accessing systems (but we need to for the renderer)

		template<typename... EntityComponents>
		Entity createEntity();
		void destroyEntity(EntityID ID);
		Entity getEntity(EntityID ID);

	private:
		template<size_t index, typename EntityComponent, typename... EntityComponents>
		void buildComponentMap(std::unordered_map<ComponentID, std::unique_ptr<Component>>& map);
		template<size_t index>
		void buildComponentMap(std::unordered_map<ComponentID, std::unique_ptr<Component>>& map);
	};
}

#include "Manager.inl"