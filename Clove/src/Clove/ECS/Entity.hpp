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
		//TODO: is funny about the map when copying (probably from the unique_ptrs)

		Entity() = delete;
		Entity(const Entity& other) = delete;
		Entity(Entity&& other) noexcept;
		Entity& operator=(const Entity& other) = delete;
		Entity& operator=(Entity&& other) noexcept;
		virtual ~Entity();

		Entity(EntityID ID);

		EntityID getID() const;

		//TODO: .inl
		template<typename T>
		T* getComponent() const{
			if(const auto foundComp = components.find(T::ID); foundComp != components.end()){
				return foundComp->second;
			}
			return nullptr;
		}

		const std::unordered_map<ComponentID, std::unique_ptr<Component>>& getComponents() const;
	};
}
