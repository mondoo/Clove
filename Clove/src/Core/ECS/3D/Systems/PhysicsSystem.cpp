#include "PhysicsSystem.hpp"

#include <btBulletDynamicsCommon.h>

namespace clv::ecs::_3D{
	PhysicsSystem::PhysicsSystem(){
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		overlappingPairCache = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver;

		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	}

	PhysicsSystem::PhysicsSystem(PhysicsSystem&& other) noexcept = default;

	PhysicsSystem& PhysicsSystem::operator=(PhysicsSystem&& other) noexcept = default;

	PhysicsSystem::~PhysicsSystem(){
		delete dynamicsWorld;
		delete solver;
		delete overlappingPairCache;
		delete dispatcher;

		delete collisionConfiguration;
	}

	void PhysicsSystem::update(utl::DeltaTime deltaTime){
		const auto updateTransform = [](const ComponentTuple& tuple){
			RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(tuple);
			TransformComponent* transform = std::get<TransformComponent*>(tuple);

			btTransform btTrans;
			rigidBody->rigidBody->getMotionState()->getWorldTransform(btTrans);

			transform->setPosition({ btTrans.getOrigin().getX(), btTrans.getOrigin().getY(), btTrans.getOrigin().getZ() });
			//TODO: Rotation
		};

		dynamicsWorld->stepSimulation(deltaTime.getDeltaSeconds());
		std::for_each(components.begin(), components.end(), updateTransform);
	}

	void PhysicsSystem::handleEntityCreation(const ComponentTuple& componentTuple){
		RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(componentTuple);
		dynamicsWorld->addRigidBody(rigidBody->rigidBody);
	}

	void PhysicsSystem::handleEntityDestruction(const ComponentTuple& componentTuple){
		RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(componentTuple);
		dynamicsWorld->removeCollisionObject(rigidBody->rigidBody);
	}
}