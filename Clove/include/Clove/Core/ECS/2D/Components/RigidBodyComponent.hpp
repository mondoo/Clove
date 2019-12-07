#pragma once

#include "Clove/Core/ECS/Component.hpp"

class btCollisionShape;
class btRigidBody;

namespace clv::ecs::_2D{
	class RigidBodyComponent : public Component<_2D::RigidBodyComponent>{
		friend class PhysicsSystem;

		//VARIABLES
	public:
		utl::MultiCastDelegate<void(RigidBodyComponent*)> onBodyCollision;

	private:
		std::unique_ptr<btCollisionShape> collisionShape;
		std::unique_ptr<btRigidBody> body;

		mth::vec2f squareSize{};

		float mass = 0.0f;
		bool isKinematic = false;

		//FUNCTIONS
	public:
		RigidBodyComponent();
		//Note: Only supporting collision squares for now
		RigidBodyComponent(float mass, bool isKinematic, const mth::vec2f& squareSize);

		RigidBodyComponent(const RigidBodyComponent& other);
		RigidBodyComponent(RigidBodyComponent&& other) noexcept;

		RigidBodyComponent& operator=(const RigidBodyComponent& other);
		RigidBodyComponent& operator=(RigidBodyComponent&& other) noexcept;

		virtual ~RigidBodyComponent();

	private:
		void initialise(const mth::vec2f& cubeSize);
	};
}