#include "Tunic/ECS/3D/Systems/PhysicsSystem.hpp"

#include "Tunic/ECS/Core/World.hpp"
#include "Tunic/ECS/3D/Components/TransformComponent.hpp"
#include "Tunic/ECS/3D/Components/RigidBodyComponent.hpp"
#include "Tunic/Physics/World.hpp"
#include "Tunic/Physics/RigidBody.hpp"

using namespace clv;

namespace tnc::ecs::_3D{
	PhysicsSystem::PhysicsSystem(){
		physicsWorld = std::make_unique<phy::World>();
	}

	PhysicsSystem::PhysicsSystem(PhysicsSystem&& other) noexcept = default;

	PhysicsSystem& PhysicsSystem::operator=(PhysicsSystem&& other) noexcept = default;

	PhysicsSystem::~PhysicsSystem() = default;

	void PhysicsSystem::update(World& world, utl::DeltaTime deltaTime){
		CLV_PROFILE_FUNCTION();

		using ComponentTuple = std::tuple<TransformComponent*, RigidBodyComponent*>;

		const auto updateRigidBody = [](const ComponentTuple& tuple){
			RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(tuple);
			TransformComponent* transform = std::get<TransformComponent*>(tuple);

			rigidBody->rigidBody->setWorldPosition(transform->getPosition());
			rigidBody->rigidBody->setWorldRotation(transform->getRotation());
		};

		const auto updateTransform = [](const ComponentTuple& tuple){
			RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(tuple);
			TransformComponent* transform = std::get<TransformComponent*>(tuple);

			transform->setPosition(rigidBody->rigidBody->getPhysicsPosition());
			transform->setRotation(rigidBody->rigidBody->getPhysicsRotation());
		};

		auto componentTuples = world.getComponentSets<TransformComponent, RigidBodyComponent>();

		std::for_each(componentTuples.begin(), componentTuples.end(), updateRigidBody);
		physicsWorld->stepSimulation(deltaTime.getDeltaSeconds());
		std::for_each(componentTuples.begin(), componentTuples.end(), updateTransform);

		for (const auto& manifold : physicsWorld->getCollisionManifolds()){
			auto* compA = static_cast<RigidBodyComponent*>(manifold.bodyA->getUserPointer());
			auto* compB = static_cast<RigidBodyComponent*>(manifold.bodyB->getUserPointer());

			compA->onBodyCollision.broadcast(compB);
			compB->onBodyCollision.broadcast(compB);
		}
	}

	void PhysicsSystem::onComponentCreated(ComponentInterface* component){
		if(component->getID() == RigidBodyComponent::id()){
			physicsWorld->addRigidBody(static_cast<RigidBodyComponent*>(component)->rigidBody.get());
		}
	}

	void PhysicsSystem::onComponentDestroyed(ComponentInterface* component){
		if(component->getID() == RigidBodyComponent::id()){
			physicsWorld->removeRigidBody(static_cast<RigidBodyComponent*>(component)->rigidBody.get());
		}
	}
}