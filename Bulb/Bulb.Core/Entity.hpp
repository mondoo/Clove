#pragma once

#include "Component.hpp"

#include <Tunic/ECS/Core/Entity.hpp>

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
		tnc::ecs::EntityID ID;
		System::Collections::Generic::List<Component^>^ components;

		//FUNCTIONS
	public:
		Entity(tnc::ecs::EntityID ID);

		System::Collections::Generic::List<Component^>^ getComponents();

		TransformComponent^ addTransformComponent();
		Component^ addMeshComponent();
	};
}