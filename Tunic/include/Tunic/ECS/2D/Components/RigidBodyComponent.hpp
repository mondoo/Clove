#pragma once

#include "Tunic/ECS/Core/Component.hpp"

class btCollisionShape;
class btRigidBody;

namespace tnc::ecs::_2D{
	class RigidBodyComponent : public Component<_2D::RigidBodyComponent>{
		friend class PhysicsSystem;

		//VARIABLES
	public:
		clv::utl::MultiCastDelegate<void(RigidBodyComponent*)> onBodyCollision;

	private:
		std::unique_ptr<btCollisionShape> collisionShape;
		std::unique_ptr<btRigidBody> body;

		clv::mth::vec2f squareSize{};

		float mass = 0.0f;
		bool isKinematic = false;
		bool respondToCollision = true;

		//FUNCTIONS
	public:
		RigidBodyComponent();
		//Note: Only supporting collision squares for now
		RigidBodyComponent(float mass, bool isKinematic, bool respondToCollision, const clv::mth::vec2f& squareSize);

		RigidBodyComponent(const RigidBodyComponent& other);
		RigidBodyComponent(RigidBodyComponent&& other) noexcept;

		RigidBodyComponent& operator=(const RigidBodyComponent& other);
		RigidBodyComponent& operator=(RigidBodyComponent&& other) noexcept;

		virtual ~RigidBodyComponent();

	private:
		void initialise(const clv::mth::vec2f& cubeSize);
	};
}