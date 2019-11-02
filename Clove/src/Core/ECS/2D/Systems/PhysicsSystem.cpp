#include "PhysicsSystem.hpp"

#include <btBulletDynamicsCommon.h>

namespace clv::ecs::_2D{
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
		const auto updateRigidBody = [](const ComponentTuple& tuple){
			RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(tuple);
			TransformComponent* transform = std::get<TransformComponent*>(tuple);

			const auto pos = transform->getPosition();
			const float rot = transform->getRotation();

			btTransform btTrans = rigidBody->body->getWorldTransform();
			btTrans.setOrigin({ pos.x, pos.y, 0.0f });
			btTrans.setRotation({ btVector3{ 0.0f, 0.0f, 1.0f }, rot });

			rigidBody->body->setWorldTransform(btTrans);
		};

		const auto updateTransform = [](const ComponentTuple& tuple){
			RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(tuple);
			TransformComponent* transform = std::get<TransformComponent*>(tuple);

			btTransform btTrans = rigidBody->body->getWorldTransform();
			const auto pos = btTrans.getOrigin();
			const auto rot = btTrans.getRotation();
			float x;
			float y;
			float z;
			rot.getEulerZYX(z, y, x);

			transform->setPosition({ pos.getX(), pos.getY() });
			transform->setRotation(z);
		};

		std::for_each(components.begin(), components.end(), updateRigidBody);
		dynamicsWorld->stepSimulation(deltaTime.getDeltaSeconds());
		std::for_each(components.begin(), components.end(), updateTransform);

		int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
		for(int i = 0; i < numManifolds; ++i){
			btPersistentManifold* manifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			int numContacts = manifold->getNumContacts();
			if(numContacts > 0){
				const btCollisionObject* obA = manifold->getBody0();
				const btCollisionObject* obB = manifold->getBody1();

				RigidBodyComponent* bodyA = static_cast<RigidBodyComponent*>(obA->getUserPointer());
				RigidBodyComponent* bodyB = static_cast<RigidBodyComponent*>(obB->getUserPointer());

				bodyA->onBodyCollision.broadcast(bodyB);
				bodyB->onBodyCollision.broadcast(bodyA);
			}
		}
	}

	void PhysicsSystem::handleEntityCreation(const ComponentTuple & componentTuple){
		RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(componentTuple);
		dynamicsWorld->addRigidBody(rigidBody->body.get());
	}

	void PhysicsSystem::handleEntityDestruction(const ComponentTuple & componentTuple){
		RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(componentTuple);
		dynamicsWorld->removeCollisionObject(rigidBody->body.get());
	}
}