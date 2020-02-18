#pragma once

#include <Clove/Core/Utils/DeltaTime.hpp>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace tnc::phy{
	class World{
		//VARIABLES
	private:
		btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
		btCollisionDispatcher* dispatcher = nullptr;
		btBroadphaseInterface* overlappingPairCache = nullptr;
		btSequentialImpulseConstraintSolver* solver = nullptr;

		btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

		//FUNCTIONS
	public:
		World();

		World(const World& other) = delete;
		World(World&& other) noexcept;

		World& operator=(const World& other) = delete;
		World& operator=(World&& other) noexcept;

		~World();

		void stepSimulation(clv::utl::DeltaTime deltaTime);
	};
}