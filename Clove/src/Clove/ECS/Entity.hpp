#pragma once

#include "Clove/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class Component;

	class Entity{
		//VARIABLES
	/*protected:*/
	public: //TEMP PUBLIC
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

		template<typename ComponentType>
		ComponentType* getComponent() const;

		const std::unordered_map<ComponentID, std::unique_ptr<Component>>& getComponents() const;

	protected:
		template<typename ComponentType>
		ComponentType* addComponent();
		
		template<typename ComponentType, typename ...ConstructTypes>
		ComponentType* addComponent(ConstructTypes&&... args);
	};
}

#include "Entity.inl"