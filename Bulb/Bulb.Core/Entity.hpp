#pragma once

#include "Component.hpp"

#include <Clove/Core/ECS/Entity.hpp>

namespace Bulb::Core{
	public ref class Entity{
		//VARIABLES
	private:
		//clv::ecs::Entity nativeEntity;

	public:
		//All temp
		System::String^ name = "Test entity";

		//FUNCTIONS
	public:
		array<Component^>^ getComponents();
	};
}