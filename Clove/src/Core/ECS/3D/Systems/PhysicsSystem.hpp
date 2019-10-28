#pragma once

#include "Core/ECS/System.hpp"

#include "Core/ECS/3D/Components/RigidBodyComponent.hpp"
#include "Core/ECS/3D/Components/TransformComponent.hpp"

namespace clv::ecs::_3D{
	class PhysicsSystem : public System<RigidBodyComponent, TransformComponent>{
		//FUNCTIONS
	public:
		PhysicsSystem();
		PhysicsSystem(const PhysicsSystem& other) = delete;
		PhysicsSystem(PhysicsSystem&& other) noexcept;
		PhysicsSystem& operator=(const PhysicsSystem& other) = delete;
		PhysicsSystem& operator=(PhysicsSystem&& other) noexcept;
		virtual ~PhysicsSystem();

		virtual void update(utl::DeltaTime deltaTime) override;
	};
}