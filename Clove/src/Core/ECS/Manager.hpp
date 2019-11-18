#pragma once

#include "Core/ECS/ECSTypes.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/ECS/ComponentManager.hpp"
#include "Core/ECS/System.hpp"
#include "Core/Utils/DeltaTime.hpp"

namespace clv::ecs{
	class Entity;
	class System;
}

namespace clv::ecs{
	class Manager{
		//VARIABLES
	private:
		ComponentManager componentManager;

		std::array<std::unique_ptr<System>, 8> systems;

		static EntityID nextID;

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
		Entity getEntity(EntityID ID);
		void destroyEntity(EntityID ID);

		template<typename ComponentType, typename ...ConstructArgs>
		ComponentType* addComponent(EntityID entityID, ConstructArgs&& ...args);

		template<typename ComponentType>
		ComponentType* getComponent(EntityID entityID);

		template<typename ...ComponentTypes>
		std::vector<std::tuple<std::add_pointer_t<ComponentTypes>...>> getComponentSets();

	private:
		enum class FoundState{ NullptrFound, EndOfTuple };
		template<std::size_t index, typename ...ComponentTypes>
		FoundState checkForNullptr(const std::tuple<std::add_pointer_t<ComponentTypes>...>& tuple, typename std::enable_if_t<(index == sizeof...(ComponentTypes)), int> = 0);

		template<std::size_t index, typename ...ComponentTypes>
		FoundState checkForNullptr(const std::tuple<std::add_pointer_t<ComponentTypes>...>& tuple, typename std::enable_if_t<(index < sizeof...(ComponentTypes)), int> = 0);
	};
}

#include "Manager.inl"