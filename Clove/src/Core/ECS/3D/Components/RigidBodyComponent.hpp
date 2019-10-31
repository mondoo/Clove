#pragma once

#include "Core/ECS/Component.hpp"

#include "Core/Utils/HashString.hpp"

class btCollisionShape;
class btRigidBody;

namespace clv::ecs::_3D{
	class RigidBodyComponent : public Component{
		friend class PhysicsSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = HASH_CLASS(RigidBodyComponent);

	private:
		btCollisionShape* collisionShape = nullptr;
		btRigidBody* rigidBody = nullptr;

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

	private:
		void initialise();
	};
}
