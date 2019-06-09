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
		//TODO: I'm not sure mapping it is really any better
		//But considering we only really want a single instance of each system
		//it kind of works

	
		//FUNCTIONS
	public:
		Manager();
		Manager(const Manager& other) = delete;
		Manager(Manager&& other) noexcept = delete;
		Manager& operator=(const Manager& other) = delete;
		Manager& operator=(Manager&& other) noexcept = delete;
		~Manager();

		void update(float deltaTime);

		//TODO: .inl
		template<typename T>
		T* getSystem(){
			if(auto foundSystem = systems.find(T::ID); foundSystem != systems.end()){
				return static_cast<T*>(foundSystem->second.get());
			}
			return nullptr;
		}

		//TODO: .inl
		template<typename T>
		EntityID createEntity(){
			static EntityID nextID = 0; //TODO: have a better system for generating and reusing IDs

			EntityID ID = ++nextID;

			std::unique_ptr<T> entity = std::make_unique<T>(ID);
			for(const auto& [sysID, system] : systems){
				system->onEntityCreated(*entity);
			}
			entities[ID] = std::move(entity);

			return ID;
		}

		void destroyEntity(EntityID ID);
	};
}
