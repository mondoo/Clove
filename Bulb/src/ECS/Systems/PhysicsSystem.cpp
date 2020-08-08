#include "Bulb/ECS/Systems/PhysicsSystem.hpp"

#include "Bulb/ECS/Components/RigidBodyComponent.hpp"
#include "Bulb/ECS/Components/TransformComponent.hpp"
#include "Bulb/ECS/World.hpp"
#include "Bulb/Physics/RigidBody.hpp"
#include "Bulb/Physics/World.hpp"

#include <Clove/Event/EventDispatcher.hpp>

using namespace clv;

namespace blb::ecs {
	PhysicsSystem::PhysicsSystem() {
		physicsWorld = std::make_unique<phy::World>();
	}

	PhysicsSystem::PhysicsSystem(std::shared_ptr<phy::World> physicsWorld)
		: physicsWorld(std::move(physicsWorld)) {
	}

	PhysicsSystem::PhysicsSystem(PhysicsSystem&& other) noexcept = default;

	PhysicsSystem& PhysicsSystem::operator=(PhysicsSystem&& other) noexcept = default;

	PhysicsSystem::~PhysicsSystem() = default;

	void PhysicsSystem::registerToEvents(EventDispatcher& dispatcher) {
		componentAddedHandle = dispatcher.bindToEvent<ComponentAddedEvent<RigidBodyComponent>>(std::bind(&PhysicsSystem::onComponentAdded, this, std::placeholders::_1));
		componentRemovedHandle = dispatcher.bindToEvent<ComponentRemovedEvent<RigidBodyComponent>>(std::bind(&PhysicsSystem::onComponentRemoved, this, std::placeholders::_1));
	}

	void PhysicsSystem::update(World& world, utl::DeltaTime deltaTime) {
		CLV_PROFILE_FUNCTION();

		const auto componentTuples = world.getComponentSets<TransformComponent, RigidBodyComponent>();

		//Update rigid bodies
		for(auto&& [transform, rigidBody] : componentTuples) {
			rigidBody->rigidBody->setWorldPosition(transform->getPosition());
			rigidBody->rigidBody->setWorldRotation(transform->getRotation());
		}

		//Step physics world
		physicsWorld->stepSimulation(deltaTime.getDeltaSeconds());

		//Update transforms
		for(auto&& [transform, rigidBody] : componentTuples) {
			transform->setPosition(rigidBody->rigidBody->getPhysicsPosition());
			transform->setRotation(rigidBody->rigidBody->getPhysicsRotation());
		}

		//Handle collisions
		for(const phy::CollisionManifold& manifold : physicsWorld->getCollisionManifolds()) {
			auto* compA = std::any_cast<RigidBodyComponent*>(manifold.bodyA->getUserData());
            auto* compB = std::any_cast<RigidBodyComponent*>(manifold.bodyB->getUserData());

			compA->onBodyCollision.broadcast(compB);
			compB->onBodyCollision.broadcast(compA);
		}
	}

	void PhysicsSystem::onComponentAdded(const ComponentAddedEvent<RigidBodyComponent>& event) {
		physicsWorld->addRigidBody(event.component->rigidBody.get());
	}

	void PhysicsSystem::onComponentRemoved(const ComponentRemovedEvent<RigidBodyComponent>& event) {
		physicsWorld->removeRigidBody(event.component->rigidBody.get());
	}
}