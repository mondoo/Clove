#pragma once

#include <Clove/Core/Utils/DeltaTime.hpp>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace tnc::phy{
	class RigidBody;

	struct CollisionManifold{
		RigidBody* bodyA = nullptr;
		RigidBody* bodyB = nullptr;
	};
}

namespace tnc::phy{
	class World{
		//VARIABLES
	private:
		btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
		btCollisionDispatcher* dispatcher = nullptr;
		btBroadphaseInterface* overlappingPairCache = nullptr;
		btSequentialImpulseConstraintSolver* solver = nullptr;

		btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

		std::vector<CollisionManifold> collisionManifolds;

		//FUNCTIONS
	public:
		World();

		World(const World& other) = delete;
		World(World&& other) noexcept;

		World& operator=(const World& other) = delete;
		World& operator=(World&& other) noexcept;

		~World();

		void stepSimulation(clv::utl::DeltaTime deltaTime);

		const std::vector<CollisionManifold>& getCollisionManifolds() const;

		RigidBody* rayCast(const clv::mth::vec3f& begin, const clv::mth::vec3f& end);

		void addRigidBody(RigidBody* rigidBody);
		void removeRigidBody(RigidBody* rigidBody);
	};
}