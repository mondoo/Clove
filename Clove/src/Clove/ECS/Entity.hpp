#pragma once

//does not have functionality

//#include "Clove/Scene/SceneNode.hpp"

//Entity is a thing that has components

#include "Clove/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class Component;

	class Entity/* : public scene::SceneNode*/{
		//VARIABLES
	private:
		EntityID id;

		std::unordered_map<ComponentID, Component*> components; //map?? unique_ptr?

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

		template<typename T>
		T* getComponent() const{
			//Find the nreturn etc.
		}

		/*virtual void update(float deltaSeconds) override;*/
	};
}
