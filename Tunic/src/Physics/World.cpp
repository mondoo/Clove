#include "Tunic/Physics/World.hpp"

#include "Tunic/Physics/RigidBody.hpp"

#include <btBulletDynamicsCommon.h>

namespace tnc::phy{
	World::World(){
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		overlappingPairCache = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver;

		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	}

	World::World(World&& other) noexcept = default;

	World& World::operator=(World&& other) noexcept = default;

	World::~World(){
		delete dynamicsWorld;

		delete solver;
		delete overlappingPairCache;
		delete dispatcher;
		delete collisionConfiguration;
	}

	void World::stepSimulation(clv::utl::DeltaTime deltaTime){
		dynamicsWorld->stepSimulation(deltaTime.getDeltaSeconds());

		collisionManifolds.clear();
		//TODO: Just use dispatcher-> ?
		int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; ++i){
			btPersistentManifold* manifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			int numContacts = manifold->getNumContacts();
			if (numContacts > 0){
				const btCollisionObject* obA = manifold->getBody0();
				const btCollisionObject* obB = manifold->getBody1();

				RigidBody* bodyA = static_cast<RigidBody*>(obA->getUserPointer());
				RigidBody* bodyB = static_cast<RigidBody*>(obB->getUserPointer());

				collisionManifolds.emplace_back(CollisionManifold{ bodyA, bodyB });
			}
		}
	}

	const std::vector<CollisionManifold>& World::getCollisionManifolds() const{
		return collisionManifolds;
	}

	void World::addRigidBody(RigidBody* rigidBody){
		dynamicsWorld->addRigidBody(rigidBody->body.get());
	}

	void World::removeRigidBody(RigidBody* rigidBody){
		dynamicsWorld->removeCollisionObject(rigidBody->body.get());
	}
}