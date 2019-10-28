#pragma once

#include "Core/ECS/System.hpp"

#include "Core/ECS/3D/Components/RigidBodyComponent.hpp"
#include "Core/ECS/3D/Components/TransformComponent.hpp"

namespace clv::ecs::_3D{
	class PhysicsSystem : public System<RigidBodyComponent, TransformComponent>{
		//TODO:
	};
}