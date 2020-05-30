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

		virtual bool hasComponent(EntityID entityId) const = 0;

		virtual void cloneComponent(EntityID fromId, EntityID toId) = 0;
		virtual void removeComponent(EntityID entityId) = 0;
	};

	template<typename ComponentType>
	class ComponentContainer : public ComponentContainerInterface {
		//VARIABLES
	private:
		clv::mem::PoolAllocator<ComponentType> componentAllocator;

		std::unordered_map<EntityID, size_t> entityIDToIndex;
		std::vector<ComponentType*> components; //TODO: It might be better to iterator over the allocators storage rather than having this vector

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

		bool hasComponent(EntityID entityId) const final;

		void cloneComponent(EntityID fromId, EntityID toId) final;
		void removeComponent(EntityID entityId) final;

		template<typename... ConstructArgs>
		ComponentPtr<ComponentType> addComponent(EntityID entityId, ConstructArgs&&... args);
		ComponentPtr<ComponentType> getComponent(EntityID entityId);
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