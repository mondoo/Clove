#include "PhysicsSystem.hpp"

namespace clv::ecs::_3D{
	PhysicsSystem::PhysicsSystem() = default;

	PhysicsSystem::PhysicsSystem(PhysicsSystem &&other) noexcept = default;

	PhysicsSystem &PhysicsSystem::operator=(PhysicsSystem &&other) noexcept = default;

	PhysicsSystem::~PhysicsSystem() = default;

	void PhysicsSystem::update(utl::DeltaTime deltaTime){
		const auto updatePosition = [deltaTime](const ComponentTuple& tuple){
			RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(tuple);
			TransformComponent* transform = std::get<TransformComponent*>(tuple);

			if(rigidBody->mass == 0.0f){
				return;
			}

			math::Vector3f velocity = rigidBody->velocity + (rigidBody->force / rigidBody->mass) * deltaTime.getDeltaSeconds();
			math::Vector3f position = transform->getLocalPosition() + velocity * deltaTime.getDeltaSeconds();

			rigidBody->velocity = velocity;
			transform->setPosition(position);
		};

		std::for_each(components.begin(), components.end(), updatePosition);
	}
}