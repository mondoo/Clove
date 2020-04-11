#pragma once

#include "Tunic/ECS/Core/ECSTypes.hpp"
#include "Clove/Memory/PoolAllocator.hpp"
#include "Tunic/ECS/Core/Component.hpp"

namespace tnc::ecs{
	class ComponentManager{
	private:
		class ComponentContainerInterface{
			//VARIABLES
		public:
			clv::utl::SingleCastDelegate<void(ComponentInterface*)> componentAddedDelegate;
			clv::utl::SingleCastDelegate<void(ComponentInterface*)> componentRemovedDelegate;

			//FUNCTIONS
		public:
			ComponentContainerInterface();

			ComponentContainerInterface(const ComponentContainerInterface& other) = delete;
			ComponentContainerInterface(ComponentContainerInterface&& other) noexcept;

			ComponentContainerInterface& operator=(const ComponentContainerInterface& other) = delete;
			ComponentContainerInterface& operator=(ComponentContainerInterface&& other) noexcept;

			virtual ~ComponentContainerInterface();

			virtual void cloneComponent(EntityID fromID, EntityID toID) = 0;

			virtual void removeComponent(EntityID entityID) = 0;
		};

		template<typename ComponentType>
		class ComponentContainer : public ComponentContainerInterface{
			//VARIABLES
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

			template<typename ...ConstructArgs>
			ComponentPtr<ComponentType> addComponent(EntityID entityID, ConstructArgs&& ...args);
			virtual void cloneComponent(EntityID fromID, EntityID toID) override;
			ComponentPtr<ComponentType> getComponent(EntityID entityID);
			virtual void removeComponent(EntityID entityID) override;
		};

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
		ComponentContainer<ComponentType>* getComponentContainer();

		void cloneEntitiesComponents(EntityID fromID, EntityID toID);

		void onEntityDestroyed(EntityID entityID);

	private:
		void onContainerAddedComponent(ComponentInterface* component);
		void onContainerRemovedComponent(ComponentInterface* component);
	};
}

#include "ComponentManager.inl"