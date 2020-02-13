#pragma once

#include "Tunic/ECS/Core/Component.hpp"

class btCollisionShape;
class btRigidBody;

namespace tnc::ecs::_3D{
	class RigidBodyComponent : public Component<_3D::RigidBodyComponent>{
		friend class PhysicsSystem;

		//VARIABLES
	public:
		clv::utl::MultiCastDelegate<void(RigidBodyComponent*)> onBodyCollision;

	private:
		std::unique_ptr<btCollisionShape> collisionShape;
		std::unique_ptr<btRigidBody> body;

		float mass = 0.0f;
		bool isKinematic = false;
		bool respondToCollision = true;

		clv::mth::vec3f cubeSize{};

		//FUNCTIONS
	public:
		RigidBodyComponent();
		//Note: Only supporting collision cubes for now
		RigidBodyComponent(float mass, bool isKinematic, bool respondToCollision, const clv::mth::vec3f& cubeSize);

		RigidBodyComponent(const RigidBodyComponent& other);
		RigidBodyComponent(RigidBodyComponent&& other) noexcept;

		RigidBodyComponent& operator=(const RigidBodyComponent& other);
		RigidBodyComponent& operator=(RigidBodyComponent&& other) noexcept;

		virtual ~RigidBodyComponent();

	private:
		void initialise(const clv::mth::vec3f& cubeSize);
	};
}
