#pragma once

#include "Clove/Core/ECS/Component.hpp"

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

		mth::vec3f cubeSize{};

		//FUNCTIONS
	public:
		RigidBodyComponent();
		//Note: Only supporting collision cubes for now
		RigidBodyComponent(float mass, bool isKinematic, const mth::vec3f& cubeSize);

		RigidBodyComponent(const RigidBodyComponent& other);
		RigidBodyComponent(RigidBodyComponent&& other) noexcept;

		RigidBodyComponent& operator=(const RigidBodyComponent& other);
		RigidBodyComponent& operator=(RigidBodyComponent&& other) noexcept;

		virtual ~RigidBodyComponent();

	private:
		void initialise(const mth::vec3f& cubeSize);
	};
}
