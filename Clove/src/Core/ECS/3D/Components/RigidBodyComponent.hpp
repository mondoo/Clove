#pragma once

#include "Core/ECS/Component.hpp"

namespace clv::ecs::_3D{
	class RigidBodyComponent : public Component{
		friend class PhysicsSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 555555; //Temp - needs VS GUID

	private:
		float mass = 0.0f;
		math::Vector3f force = {};

		math::Vector3f velocity = {};

		//FUNCTIONS
	public:
		RigidBodyComponent();
		RigidBodyComponent(const RigidBodyComponent& other) = delete;
		RigidBodyComponent(RigidBodyComponent&& other) noexcept;
		RigidBodyComponent& operator=(const RigidBodyComponent& other) = delete;
		RigidBodyComponent& operator=(RigidBodyComponent&& other) noexcept;
		virtual ~RigidBodyComponent();

		RigidBodyComponent(float mass);

		void applyForce(math::Vector3f force);
	};
}
