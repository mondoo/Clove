#pragma once

class btCollisionShape;
class btRigidBody;

namespace blb::phy{
	class RigidBody{
		friend class World;

		//VARIABLES
	private:
		std::unique_ptr<btCollisionShape> collisionShape;
		std::unique_ptr<btRigidBody> body;

		float mass = 0.0f;
		bool isKinematic = false;
		bool respondToCollision = true;

		clv::mth::vec3f cubeSize{};

		void* userPointer = nullptr;

		//FUNCTIONS
	public:
		RigidBody() = delete;
		//Only supporting box shapes
		RigidBody(float mass, bool isKinematic, bool respondToCollision, const clv::mth::vec3f& cubeSize);

		RigidBody(const RigidBody& other);
		RigidBody(RigidBody&& other) noexcept;

		RigidBody& operator=(const RigidBody& other);
		RigidBody& operator=(RigidBody&& other) noexcept;

		~RigidBody();

		void setWorldPosition(const clv::mth::vec3f& position);
		void setWorldRotation(const clv::mth::quatf& rotation);

		void setLinearVelocity(const clv::mth::vec3f& velocity);

		clv::mth::vec3f getPhysicsPosition() const;
		clv::mth::quatf getPhysicsRotation() const;

		void setUserPointer(void* pointer);
		void* getUserPointer() const;

	private:
		void initialise(float mass, bool isKinematic, bool respondToCollision, const clv::mth::vec3f& cubeSize);
	};
}