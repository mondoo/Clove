#pragma once

//Entity is a thing that has components

#include "Clove/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class Component;

	class Entity{
		//VARIABLES
	private:
		EntityID id;

		std::unordered_map<ComponentID, Component*> components; //unique_ptr?

		//FUNCTIONS
	public:
		Entity() = delete;
		Entity(const Entity& other);
		Entity(Entity&& other) noexcept;
		Entity& operator=(const Entity& other);
		Entity& operator=(Entity&& other) noexcept;
		virtual ~Entity();

		Entity(EntityID id);

		EntityID getID() const;

		//TODO: .inl
		template<typename T>
		T* getComponent() const{
			if(const auto foundComp = components.find(T::ID); foundComp != components.end()){
				return foundComp->second;
			}
			return nullptr;
		}

		const std::unordered_map<ComponentID, Component*>& getComponents() const;
	};
}
