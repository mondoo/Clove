#include "Bulb/Physics/World.hpp"

#include "Bulb/Physics/RigidBody.hpp"

#include <btBulletDynamicsCommon.h>

namespace blb::phy{
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
		CLV_PROFILE_FUNCTION();

		dynamicsWorld->stepSimulation(deltaTime.getDeltaSeconds(), 0);

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

	RigidBody* World::rayCast(const clv::mth::vec3f& begin, const clv::mth::vec3f& end){
		btVector3 btBegin{ begin.x, begin.y, begin.z };
		btVector3 btEnd{ end.x, end.y, end.z };

		btCollisionWorld::ClosestRayResultCallback callBack{ btBegin, btEnd };
		dynamicsWorld->rayTest(btBegin, btEnd, callBack);

		if (callBack.m_collisionObject != nullptr){
			return static_cast<RigidBody*>(callBack.m_collisionObject->getUserPointer());
		} else{
			return nullptr;
		}
	}

	void World::addRigidBody(RigidBody* rigidBody){
		dynamicsWorld->addRigidBody(rigidBody->body.get());
	}

	void World::removeRigidBody(RigidBody* rigidBody){
		dynamicsWorld->removeCollisionObject(rigidBody->body.get());
	}
}