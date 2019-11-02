#pragma once

#include "Core/ECS/Component.hpp"

#include "Core/Utils/HashString.hpp"

class btCollisionShape;
class btRigidBody;

namespace clv::ecs::_2D{
	class RigidBodyComponent : public Component{
		friend class PhysicsSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = HASH_CLASS(_2D::RigidBodyComponent);

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

		//Note: Only supporting collision squares for now
		RigidBodyComponent(float mass, bool isKinematic, const math::Vector2f& squareSize);

	private:
		void initialise(const math::Vector2f& cubeSize);
	};
}