#pragma once

#include "Bulb/ECS/ComponentManager.hpp"
#include "Bulb/ECS/ECSTypes.hpp"
#include "Bulb/ECS/System.hpp"

#include <Clove/Event/EventDispatcher.hpp>

namespace blb::ecs {
	class Entity;
	class System;
}

namespace blb::ecs {
	class World {
		//VARIABLES
	private:
		clv::EventDispatcher ecsEventdispatcher;

		ComponentManager componentManager;

		std::vector<std::unique_ptr<System>> systems;

		static EntityID nextID;
		std::vector<EntityID> activeIDs;
		std::set<EntityID> pendingDestroyIDs;

		//FUNCTIONS
	public:
		World();

		World(const World& other) = delete;
		World(World&& other) noexcept = delete;

		World& operator=(const World& other) = delete;
		World& operator=(World&& other) noexcept = delete;

		~World();

		void update(clv::utl::DeltaTime deltaTime);

		Entity createEntity();
		Entity cloneEntitiesComponents(EntityID ID);

		bool isEntityValid(EntityID ID);

		Entity getEntity(EntityID ID);
		std::vector<Entity> getActiveEntities();

		void destroyEntity(EntityID ID);
		void destroyAllEntites();

		template<typename ComponentType, typename... ConstructArgs>
		ComponentPtr<ComponentType> addComponent(EntityID entityID, ConstructArgs&&... args);
		template<typename ComponentType>
		ComponentPtr<ComponentType> getComponent(EntityID entityID);
		template<typename ComponentType>
		void removeComponent(EntityID entityID);

		template<typename... ComponentTypes>
		std::vector<std::tuple<ComponentPtr<ComponentTypes>...>> getComponentSets();

		template<typename SystemType, typename... ConstructArgs>
		void addSystem(ConstructArgs&&... args);

	private:
		enum class FoundState { NullptrFound, EndOfTuple };
		template<std::size_t index, typename... ComponentTypes>
		FoundState checkForNullptr(const std::tuple<ComponentPtr<ComponentTypes>...>& tuple);
	};
}

#include "World.inl"
