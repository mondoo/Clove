#include "Tunic/ECS/Systems/PhysicsSystem.hpp"

#include "Tunic/ECS/Components/RigidBodyComponent.hpp"
#include "Tunic/ECS/Components/TransformComponent.hpp"
#include "Tunic/ECS/World.hpp"
#include "Tunic/Physics/RigidBody.hpp"
#include "Tunic/Physics/World.hpp"

using namespace clv;

namespace tnc::ecs {
	PhysicsSystem::PhysicsSystem() {
		physicsWorld = std::make_unique<phy::World>();
	}

	PhysicsSystem::PhysicsSystem(std::unique_ptr<phy::World> physicsWorld)
		: physicsWorld(std::move(physicsWorld)) {
	}

	PhysicsSystem::PhysicsSystem(PhysicsSystem&& other) noexcept = default;

	PhysicsSystem& PhysicsSystem::operator=(PhysicsSystem&& other) noexcept = default;

	PhysicsSystem::~PhysicsSystem() = default;

	void PhysicsSystem::update(World& world, utl::DeltaTime deltaTime) {
		CLV_PROFILE_FUNCTION();

		const auto componentTuples = world.getComponentSets<TransformComponent, RigidBodyComponent>();

		//Update rigid bodies
		for(auto [transform, rigidBody] : componentTuples) {
			rigidBody->rigidBody->setWorldPosition(transform->getPosition());
			rigidBody->rigidBody->setWorldRotation(transform->getRotation());
		}

		//Step physics world
		physicsWorld->stepSimulation(deltaTime.getDeltaSeconds());

		//Update transforms
		for(auto [transform, rigidBody] : componentTuples) {
			transform->setPosition(rigidBody->rigidBody->getPhysicsPosition());
			transform->setRotation(rigidBody->rigidBody->getPhysicsRotation());
		}

		//Handle collisions
		for(const phy::CollisionManifold& manifold : physicsWorld->getCollisionManifolds()) {
			auto* compA = static_cast<RigidBodyComponent*>(manifold.bodyA->getUserPointer());
			auto* compB = static_cast<RigidBodyComponent*>(manifold.bodyB->getUserPointer());

			compA->onBodyCollision.broadcast(compB);
			compB->onBodyCollision.broadcast(compA);
		}
	}

	void PhysicsSystem::onComponentCreated(ComponentInterface* component) {
		if(component->getComponentID() == RigidBodyComponent::id()) {
			physicsWorld->addRigidBody(static_cast<RigidBodyComponent*>(component)->rigidBody.get());
		}
	}

	void PhysicsSystem::onComponentDestroyed(ComponentInterface* component) {
		if(component->getComponentID() == RigidBodyComponent::id()) {
			physicsWorld->removeRigidBody(static_cast<RigidBodyComponent*>(component)->rigidBody.get());
		}
	}
}