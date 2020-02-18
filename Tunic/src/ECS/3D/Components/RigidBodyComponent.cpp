#include "Tunic/ECS/3D/Components/RigidBodyComponent.hpp"

#include "Tunic/Physics/RigidBody.hpp"

using namespace clv;

namespace tnc::ecs::_3D{
	RigidBodyComponent::RigidBodyComponent(){
		rigidBody = std::make_unique<phy::RigidBody>(0.0f, false, true, mth::vec3f{ 1.0f, 1.0f, 1.0f });
	}

	RigidBodyComponent::RigidBodyComponent(float mass, bool isKinematic, bool respondToCollision, const mth::vec3f& cubeSize){
		rigidBody = std::make_unique<phy::RigidBody>(mass, isKinematic, respondToCollision, cubeSize);
	}

	RigidBodyComponent::RigidBodyComponent(std::unique_ptr<phy::RigidBody> rigidBody)
		: rigidBody(std::move(rigidBody)){
	}

	RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent& other){
		rigidBody = std::make_unique<phy::RigidBody>(other.rigidBody.get());
	}

	RigidBodyComponent::RigidBodyComponent(RigidBodyComponent&& other) noexcept = default;

	RigidBodyComponent& RigidBodyComponent::operator=(const RigidBodyComponent& other){
		rigidBody = std::make_unique<phy::RigidBody>(other.rigidBody.get());

		return *this;
	}

	RigidBodyComponent& RigidBodyComponent::operator=(RigidBodyComponent&& other) noexcept = default;

	RigidBodyComponent::~RigidBodyComponent() = default;
}