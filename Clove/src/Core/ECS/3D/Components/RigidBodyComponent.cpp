#include "RigidBodyComponent.hpp"

namespace clv::ecs::_3D{
	RigidBodyComponent::RigidBodyComponent() = default;

	RigidBodyComponent::RigidBodyComponent(RigidBodyComponent &&other) noexcept = default;

	RigidBodyComponent &RigidBodyComponent::operator=(RigidBodyComponent &&other) noexcept = default;

	RigidBodyComponent::~RigidBodyComponent() = default;

	RigidBodyComponent::RigidBodyComponent(float mass)
		: mass(mass){
	}

	void RigidBodyComponent::applyForce(math::Vector3f force){
		this->force = std::move(force);
	}
}