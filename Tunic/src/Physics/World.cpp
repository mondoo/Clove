#include "Tunic/Physics/World.hpp"

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
		//...
	}
}