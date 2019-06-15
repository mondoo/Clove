#pragma once

#include "Clove/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class Component;

	class Entity{
		//VARIABLES
	protected:
		std::unordered_map<ComponentID, std::unique_ptr<Component>> components;

	private:
		EntityID ID;

		//FUNCTIONS
	public:
		Entity() = delete;
		Entity(const Entity& other) = delete;
		Entity(Entity&& other) noexcept;
		Entity& operator=(const Entity& other) = delete;
		Entity& operator=(Entity&& other) noexcept;
		virtual ~Entity();

		Entity(EntityID ID);

		EntityID getID() const;

		template<typename EntityType>
		EntityType* getComponent() const;

		const std::unordered_map<ComponentID, std::unique_ptr<Component>>& getComponents() const;

	protected:
		template<typename EntityType>
		EntityType* addComponent();
		
		template<typename EntityType, typename ...ConstructTypes>
		EntityType* addComponent(ConstructTypes&&... args);
	};
}

#include "Entity.inl"