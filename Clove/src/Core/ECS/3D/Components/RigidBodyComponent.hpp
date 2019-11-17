#pragma once

#include "Core/ECS/Component.hpp"

class btCollisionShape;
class btRigidBody;

namespace clv::ecs::_3D{
	class RigidBodyComponent : public Component<_3D::RigidBodyComponent>{
		friend class PhysicsSystem;

		//VARIABLES
	public:
		utl::MultiCastDelegate<void(RigidBodyComponent*)> onBodyCollision;

	private:
		std::unique_ptr<btCollisionShape> collisionShape;
		std::unique_ptr<btRigidBody> body;

		float mass = 0.0f;
		bool isKinematic = false;

		//FUNCTIONS
	public:
		RigidBodyComponent();
		RigidBodyComponent(const RigidBodyComponent& other) = delete;
		RigidBodyComponent(RigidBodyComponent&& other) noexcept;
		RigidBodyComponent& operator=(const RigidBodyComponent& other) = delete;
		RigidBodyComponent& operator=(RigidBodyComponent&& other) noexcept;
		virtual ~RigidBodyComponent();

		//Note: Only supporting collision cubes for now
		RigidBodyComponent(float mass, bool isKinematic, const math::Vector3f& cubeSize);

	private:
		void initialise(const math::Vector3f& cubeSize);
	};
}
