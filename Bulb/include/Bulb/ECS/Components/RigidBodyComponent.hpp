#pragma once

#include "Bulb/ECS/Component.hpp"

#include "Bulb/Physics/RigidBody.hpp"

namespace blb::ecs {
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
		RigidBodyComponent(phy::RigidBodyDescriptor initInfo, const clv::mth::vec3f& cubeSize);
		RigidBodyComponent(std::unique_ptr<phy::RigidBody> rigidBody);

		RigidBodyComponent(const RigidBodyComponent& other);
		RigidBodyComponent(RigidBodyComponent&& other) noexcept;

		RigidBodyComponent& operator=(const RigidBodyComponent& other);
		RigidBodyComponent& operator=(RigidBodyComponent&& other) noexcept;

		~RigidBodyComponent();

		void setLinearVelocity(const clv::mth::vec3f& velocity);
        clv::mth::vec3f getLinearVelocity() const;

	private:
		void initialiseRigidBody(phy::RigidBody* body);
	};
}
