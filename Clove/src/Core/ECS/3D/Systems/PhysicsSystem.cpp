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
		const auto updateRigidBody = [](const ComponentTuple& tuple){
			RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(tuple);
			TransformComponent* transform = std::get<TransformComponent*>(tuple);

			const auto pos = transform->getPosition();
			const auto rot = transform->getRotation();

			btTransform btTrans = rigidBody->body->getWorldTransform();
			btTrans.setOrigin({ pos.x, pos.y, pos.z });
			//GLM is pitch yaw roll while Bullet is yaw pitch roll
			btTrans.setRotation({ rot.x, rot.y, rot.z, rot.w });

			rigidBody->body->setWorldTransform(btTrans);
		};

		const auto updateTransform = [](const ComponentTuple& tuple){
			RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(tuple);
			TransformComponent* transform = std::get<TransformComponent*>(tuple);

			btTransform btTrans = rigidBody->body->getWorldTransform();
			const auto pos = btTrans.getOrigin();
			const auto rot = btTrans.getRotation();

			transform->setPosition({ pos.getX(), pos.getY(), pos.getZ() });
			//GLM is pitch yaw roll while Bullet is yaw pitch roll
			transform->setRotation({ rot.getW(), rot.getX(), rot.getY(), rot.getZ() });
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

	void PhysicsSystem::handleEntityCreation(const ComponentTuple& componentTuple){
		RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(componentTuple);
		dynamicsWorld->addRigidBody(rigidBody->body.get());
	}

	void PhysicsSystem::handleEntityDestruction(const ComponentTuple& componentTuple){
		RigidBodyComponent* rigidBody = std::get<RigidBodyComponent*>(componentTuple);
		dynamicsWorld->removeCollisionObject(rigidBody->body.get());
	}
}