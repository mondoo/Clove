#pragma once

class btCollisionShape;
class btRigidBody;

namespace tnc::phy{
	class RigidBody{
		friend class World;

		//VARIABLES
	public:
		clv::utl::MultiCastDelegate<void(RigidBody*)> onBodyCollision;

	private:
		std::unique_ptr<btCollisionShape> collisionShape;
		std::unique_ptr<btRigidBody> body;

		clv::mth::vec3f cubeSize{};

		float mass = 0.0f;
		bool isKinematic = false;
		bool respondToCollision = true;

		//FUNCTIONS
	public:
		RigidBody() = delete;
		//Only supporting box shapes
		RigidBody(float mass, bool isKinematic, bool respondToCollision, const clv::mth::vec3f& cubeSize);

		RigidBody(const RigidBody& other) = delete;
		RigidBody(RigidBody&& other) noexcept;

		RigidBody& operator=(const RigidBody& other) = delete;
		RigidBody& operator=(RigidBody&& other) noexcept;

		~RigidBody();
	};
}