#pragma once

#include "Core/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class ComponentManager{
	private:
		class ComponentContainerInterface{
			//FUNCTIONS
		public:
			ComponentContainerInterface();

			ComponentContainerInterface(const ComponentContainerInterface& other);
			ComponentContainerInterface(ComponentContainerInterface&& other) noexcept;

			ComponentContainerInterface& operator=(const ComponentContainerInterface& other);
			ComponentContainerInterface& operator=(ComponentContainerInterface&& other);

			virtual ~ComponentContainerInterface();
		};

		template<typename ComponentType>
		class ComponentContainer : public ComponentContainerInterface{
			//VARIABLES
		private:
			std::unordered_map<EntityID, std::unique_ptr<ComponentType>> components;

			//FUNCTIONS
		public:
			ComponentContainer();

			ComponentContainer(const ComponentContainer& other);
			ComponentContainer(ComponentContainer&& other) noexcept;

			ComponentContainer& operator=(const ComponentContainer& other);
			ComponentContainer& operator=(ComponentContainer&& other) noexcept;

			~ComponentContainer();

			template<typename ...ConstructArgs>
			ComponentType* addComponent(EntityID entityID, ConstructArgs&& ...args);

			ComponentType* getComponent(EntityID entityId);
		};

		//VARIABLES
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
	};
}

#include "ComponentManager.inl"