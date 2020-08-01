#include "Bulb/ECS/Components/RigidBodyComponent.hpp"

#include "Bulb/Physics/RigidBody.hpp"

using namespace clv;

namespace blb::ecs {
	RigidBodyComponent::RigidBodyComponent() {
		rigidBody = std::make_unique<phy::RigidBody>(phy::RigidBodyDescriptor{}, mth::vec3f{ 1.0f, 1.0f, 1.0f });
		initialiseRigidBody(rigidBody.get());
	}

	RigidBodyComponent::RigidBodyComponent(phy::RigidBodyDescriptor initInfo, const mth::vec3f& cubeSize) {
		rigidBody = std::make_unique<phy::RigidBody>(std::move(initInfo), cubeSize);
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

    void RigidBodyComponent::applyForce(const clv::mth::vec3f& force, const clv::mth::vec3f& relativeOffset) {
        rigidBody->applyForce(force, relativeOffset);
    }

    void RigidBodyComponent::applyImpulse(const clv::mth::vec3f& impulse, const clv::mth::vec3f& relativeOffset) {
        rigidBody->applyImpulse(impulse, relativeOffset);
    }

    clv::mth::vec3f RigidBodyComponent::getLinearVelocity() const {
        return rigidBody->getLinearVelocity();
    }

	void RigidBodyComponent::initialiseRigidBody(phy::RigidBody* body) {
		body->setUserPointer(this);
	}
}