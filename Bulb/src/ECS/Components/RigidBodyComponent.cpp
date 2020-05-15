#include "Bulb/ECS/Components/RigidBodyComponent.hpp"

#include "Bulb/Physics/RigidBody.hpp"

using namespace clv;

namespace blb::ecs {
	RigidBodyComponent::RigidBodyComponent() {
		rigidBody = std::make_unique<phy::RigidBody>(phy::RigidBodyInitInfo{}, mth::vec3f{ 1.0f, 1.0f, 1.0f });
		initialiseRigidBody(rigidBody.get());
	}

	RigidBodyComponent::RigidBodyComponent(const phy::RigidBodyInitInfo& initInfo, const mth::vec3f& cubeSize) {
		rigidBody = std::make_unique<phy::RigidBody>(initInfo, cubeSize);
		initialiseRigidBody(rigidBody.get());
	}

	RigidBodyComponent::RigidBodyComponent(std::unique_ptr<phy::RigidBody> rigidBody)
		: rigidBody(std::move(rigidBody)) {
		initialiseRigidBody(rigidBody.get());
	}

	RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent& other) {
		rigidBody = std::make_unique<phy::RigidBody>(*other.rigidBody);
		initialiseRigidBody(rigidBody.get());
	}

	RigidBodyComponent::RigidBodyComponent(RigidBodyComponent&& other) noexcept = default;

	RigidBodyComponent& RigidBodyComponent::operator=(const RigidBodyComponent& other) {
		rigidBody = std::make_unique<phy::RigidBody>(*other.rigidBody);
		initialiseRigidBody(rigidBody.get());

		return *this;
	}

	RigidBodyComponent& RigidBodyComponent::operator=(RigidBodyComponent&& other) noexcept = default;

	RigidBodyComponent::~RigidBodyComponent() = default;

	void RigidBodyComponent::setLinearVelocity(const clv::mth::vec3f& velocity) {
		rigidBody->setLinearVelocity(velocity);
	}

	void RigidBodyComponent::initialiseRigidBody(phy::RigidBody* body) {
		body->setUserPointer(this);
	}
}