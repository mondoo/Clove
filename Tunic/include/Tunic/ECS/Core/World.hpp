#pragma once

#include "Tunic/ECS/Core/ECSTypes.hpp"
#include "Tunic/ECS/Core/ComponentManager.hpp"
#include "Tunic/ECS/Core/System.hpp"
#include "Clove/Core/Utils/DeltaTime.hpp"

#include <queue>

namespace tnc::ecs{
	class Entity;
	class System;
}

namespace tnc::ecs{
	class World{
		//VARIABLES
	private:
		ComponentManager componentManager;

		std::vector<std::unique_ptr<System>> systems;

		static EntityID nextID;
		static std::queue<EntityID> recycledIDs;

		std::vector<EntityID> activeIDs;

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
		Entity getEntity(EntityID ID);
		void destroyEntity(EntityID ID);

		template<typename ComponentType, typename ...ConstructArgs>
		ComponentType* addComponent(EntityID entityID, ConstructArgs&& ...args);
		template<typename ComponentType>
		ComponentType* getComponent(EntityID entityID);
		template<typename ComponentType>
		void removeComponent(EntityID entityID);

		template<typename ...ComponentTypes>
		std::vector<std::tuple<std::add_pointer_t<ComponentTypes>...>> getComponentSets();

		template<typename SystemType, typename ...ConstructArgs>
		void addSystem(ConstructArgs&& ...args);

	private:
		void onComponentAdded(ComponentInterface* component);
		void onComponentRemoved(ComponentInterface* component);

		enum class FoundState{ NullptrFound, EndOfTuple };
		template<std::size_t index, typename ...ComponentTypes>
		FoundState checkForNullptr(const std::tuple<std::add_pointer_t<ComponentTypes>...>& tuple, typename std::enable_if_t<(index == sizeof...(ComponentTypes)), int> = 0);

		template<std::size_t index, typename ...ComponentTypes>
		FoundState checkForNullptr(const std::tuple<std::add_pointer_t<ComponentTypes>...>& tuple, typename std::enable_if_t<(index < sizeof...(ComponentTypes)), int> = 0);
	};
}

#include "World.inl"
