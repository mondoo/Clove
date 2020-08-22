#pragma once

#include "Bulb/ECS/Component.hpp"

#include "Bulb/Physics/RigidBody.hpp"

namespace blb::ecs {
    /**
	 * @brief Enables an entity to respond to physics events.
	 * @see RigidBody
	 */
	class RigidBodyComponent : public Component<RigidBodyComponent> {
		friend class PhysicsSystem;

		//VARIABLES
	public:
		clv::MultiCastDelegate<void(RigidBodyComponent*)> onBodyCollision;

	private:
		std::unique_ptr<phy::RigidBody> rigidBody;

		//FUNCTIONS
	public:
		RigidBodyComponent();
		RigidBodyComponent(phy::RigidBody::Descriptor initInfo, const clv::mth::vec3f& cubeSize);
		RigidBodyComponent(std::unique_ptr<phy::RigidBody> rigidBody);

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
		void initialiseRigidBody(phy::RigidBody* body);
	};
}
