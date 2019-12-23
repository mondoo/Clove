#pragma once

#include "Component.hpp"

#include <Clove/Core/ECS/Entity.hpp>

namespace Bulb::Core{
	public ref class Entity{
		//VARIABLES
	private:
		//clv::ecs::Entity nativeEntity;

		//This will basically just need the ID

	public:
		//All temp
		System::String^ name = "Test entity";

	private:
		clv::ecs::EntityID ID;

		//FUNCTIONS
	public:
		Entity(clv::ecs::EntityID ID);

		array<Component^>^ getComponents();

	};
}