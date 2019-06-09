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
		std::vector<std::unique_ptr<SystemBase>> systems;
		
		//TODO: Why do we need? might just delete
		//really not sure why we need a vector of every component?
		//are these unique? do we duplicate them for each entity?
		//std::vector<std::vector<Component*>> components; //TODO: unique ptr


		//FUNCTIONS
	public:
		Manager();
		Manager(const Manager& other) = delete;
		Manager(Manager&& other) noexcept = delete;
		Manager& operator=(const Manager& other) = delete;
		Manager& operator=(Manager&& other) noexcept = delete;
		~Manager();

		void update(float deltaTime);

		//TODO: this is purely temp to hook in the renderer system
		std::unique_ptr<gfx::Renderer>& getRenderer();

		//TODO: .inl
		template<typename T>
		EntityID createEntity(){
			static EntityID nextID = 0; //TODO: have a better system for generating and reusing IDs

			EntityID ID = ++nextID;

			std::unique_ptr<T> entity = std::make_unique<T>(ID);
			for(const auto& system : systems){
				system->onEntityCreated(*entity);
			}
			entities[ID] = std::move(entity);

			return ID;
		}

		//TODO: get entity?

		void destroyEntity(EntityID ID);
	};
}
