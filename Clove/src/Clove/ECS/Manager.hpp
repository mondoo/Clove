#pragma once

#include "Clove/ECS/ECSTypes.hpp"
#include "Clove/ECS/System.hpp"

namespace clv::gfx{
	class Renderer;
}

namespace clv::ecs{
	class Entity;
	class Component;
	class SystemBase;

	class Manager{
		//VARIABLES
	private:
		std::unordered_map<EntityID, std::unique_ptr<Entity>> entities;
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
		EntityID createEntity(); //TODO: should this return the entity or the ID? is the ID even relevant? (in this instance it allows for fast look up)
		void destroyEntity(EntityID ID);
		Entity* getEntity(EntityID ID);
	};
}

#include "Manager.inl"