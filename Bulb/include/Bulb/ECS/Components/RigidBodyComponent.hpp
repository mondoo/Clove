#pragma once

#include "Bulb/ECS/Component.hpp"

class btRigidBody;

namespace blb::ecs {
    /**
	 * @brief Enables an entity to respond to physics events.
	 * @details Entities with a RigidBodyComponent can collide with other
	 * entities and be affected by gravity. If the entity also has a
	 * CubeColliderComponent or similar the rigid body will use that shape
	 * to detect collisions.
	 */
	class RigidBodyComponent : public Component<RigidBodyComponent> {
		friend class PhysicsSystem;

		//VARIABLES
	private:
        float mass        = 1.0f;
        float isKinematic = false; /**< If true, stops the body being affected by gravity */

		//TODO
		//uint32_t collisionGroup = 0; /**< Bit flag of the collision groups this body is a part of */
        //uint32_t collisionMask  = 0; /**< Bit flag of which collision groups this body collides with */

        std::shared_ptr<btRigidBody> body;

		//FUNCTIONS
	public:
        RigidBodyComponent();

		RigidBodyComponent(const RigidBodyComponent& other);
		RigidBodyComponent(RigidBodyComponent&& other) noexcept;

		RigidBodyComponent& operator=(const RigidBodyComponent& other);
		RigidBodyComponent& operator=(RigidBodyComponent&& other) noexcept;

		~RigidBodyComponent();

		void setLinearVelocity(const clv::mth::vec3f& velocity);

		void applyForce(const clv::mth::vec3f& force, const clv::mth::vec3f& relativeOffset = { 0.0f, 0.0f, 0.0f });
        void applyImpulse(const clv::mth::vec3f& impulse, const clv::mth::vec3f& relativeOffset = { 0.0f, 0.0f, 0.0f });

		void setRestitution(float restitution);

        void setAngularFactor(const clv::mth::vec3f& factor);

        clv::mth::vec3f getLinearVelocity() const;

		float getRestitution() const;

        clv::mth::vec3f getAngularFactor() const;

	private:
		void initialiseRigidBody(/*phy::RigidBody* body*/);
	};
}
