#pragma once

#include "Bulb/ECS/Component.hpp"
#include "Bulb/ECS/ECSTypes.hpp"

#include <Clove/Event/EventHandle.hpp>
#include <Clove/Memory/PoolAllocator.hpp>

namespace clv {
	class EventDispatcher;
}

namespace blb::ecs {
	class ComponentContainerInterface {
		//FUNCTIONS
	public:
		virtual ~ComponentContainerInterface();

		virtual void cloneComponent(EntityID fromID, EntityID toID) = 0;
		virtual void removeComponent(EntityID entityID) = 0;
	};

	template<typename ComponentType>
	class ComponentContainer : public ComponentContainerInterface {
		//VARIABLES
	private:
		clv::mem::PoolAllocator<ComponentType> componentAllocator;

		std::unordered_map<EntityID, size_t> entityIDToIndex;
		std::vector<ComponentType*> components;

		clv::EventDispatcher* ecsEventDispatcher;

		//FUNCTIONS
	public:
		ComponentContainer() = delete;
		ComponentContainer(clv::EventDispatcher* dispatcher);

		ComponentContainer(const ComponentContainer& other) = delete;
		ComponentContainer(ComponentContainer&& other) noexcept;

		ComponentContainer& operator=(const ComponentContainer& other) = delete;
		ComponentContainer& operator=(ComponentContainer&& other) noexcept;

		~ComponentContainer();

		void cloneComponent(EntityID fromID, EntityID toID) final;
		void removeComponent(EntityID entityID) final;

		template<typename... ConstructArgs>
		ComponentPtr<ComponentType> addComponent(EntityID entityID, ConstructArgs&&... args);
		ComponentPtr<ComponentType> getComponent(EntityID entityID);
	};

	class ComponentManager {
		//VARIABLES
	private:
		std::unordered_map<ComponentID, std::unique_ptr<ComponentContainerInterface>> containers;

		clv::EventDispatcher* ecsEventDispatcher;

		//FUNCTIONS
	public:
		ComponentManager() = delete;
		ComponentManager(clv::EventDispatcher* dispatcher);

		ComponentManager(const ComponentManager& other) = delete;
		ComponentManager(ComponentManager&& other) noexcept = delete;

		ComponentManager& operator=(const ComponentManager& other) = delete;
		ComponentManager& operator=(ComponentManager&& other) noexcept = delete;

		~ComponentManager();

		template<typename ComponentType>
		ComponentContainer<ComponentType>& getComponentContainer();

		void cloneEntitiesComponents(EntityID fromID, EntityID toID);

		void onEntityDestroyed(EntityID entityID);
	};
}

#include "ComponentManager.inl"