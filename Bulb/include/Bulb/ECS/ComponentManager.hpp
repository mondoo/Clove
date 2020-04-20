#pragma once

#include "Bulb/ECS/Component.hpp"
#include "Bulb/ECS/ECSTypes.hpp"

#include <Clove/Memory/PoolAllocator.hpp>

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
	public:
		clv::utl::SingleCastDelegate<void(ComponentInterface*)> componentAddedDelegate;
		clv::utl::SingleCastDelegate<void(ComponentInterface*)> componentRemovedDelegate;

	private:
		clv::mem::PoolAllocator<ComponentType> componentAllocator;

		std::unordered_map<EntityID, size_t> entityIDToIndex;
		std::vector<ComponentType*> components;

		//FUNCTIONS
	public:
		ComponentContainer();

		ComponentContainer(const ComponentContainer& other);
		ComponentContainer(ComponentContainer&& other) noexcept;

		ComponentContainer& operator=(const ComponentContainer& other);
		ComponentContainer& operator=(ComponentContainer&& other) noexcept;

		~ComponentContainer();

		virtual void cloneComponent(EntityID fromID, EntityID toID) override;
		virtual void removeComponent(EntityID entityID) override;

		template<typename... ConstructArgs>
		ComponentPtr<ComponentType> addComponent(EntityID entityID, ConstructArgs&&... args);
		ComponentPtr<ComponentType> getComponent(EntityID entityID);
	};

	class ComponentManager {
		//VARIABLES
	public:
		clv::utl::SingleCastDelegate<void(ComponentInterface*)> componentAddedDelegate;
		clv::utl::SingleCastDelegate<void(ComponentInterface*)> componentRemovedDelegate;

	private:
		std::unordered_map<ComponentID, std::unique_ptr<ComponentContainerInterface>> containers;

		//FUNCTIONS
	public:
		ComponentManager();

		ComponentManager(const ComponentManager& other) = delete;
		ComponentManager(ComponentManager&& other) noexcept = delete;

		ComponentManager& operator=(const ComponentManager& other) = delete;
		ComponentManager& operator=(ComponentManager&& other) noexcept = delete;

		~ComponentManager();

		template<typename ComponentType>
		ComponentContainer<ComponentType>& getComponentContainer();

		void cloneEntitiesComponents(EntityID fromID, EntityID toID);

		void onEntityDestroyed(EntityID entityID);

	private:
		void onContainerAddedComponent(ComponentInterface* component);
		void onContainerRemovedComponent(ComponentInterface* component);
	};
}

#include "ComponentManager.inl"