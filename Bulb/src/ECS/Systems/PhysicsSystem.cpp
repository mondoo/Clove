#include "Bulb/ECS/Systems/PhysicsSystem.hpp"

#include "Bulb/ECS/Components/RigidBodyComponent.hpp"
#include "Bulb/ECS/Components/TransformComponent.hpp"
#include "Bulb/ECS/Components/CubeColliderComponent.hpp"
#include "Bulb/ECS/World.hpp"

#include <Clove/Event/EventDispatcher.hpp>

#include <btBulletDynamicsCommon.h>

using namespace clv;

namespace blb::ecs {
    PhysicsSystem::PhysicsSystem() {
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher             = new btCollisionDispatcher(collisionConfiguration);
        overlappingPairCache   = new btDbvtBroadphase();
        solver                 = new btSequentialImpulseConstraintSolver;

        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    }

	PhysicsSystem::PhysicsSystem(PhysicsSystem&& other) noexcept = default;

	PhysicsSystem& PhysicsSystem::operator=(PhysicsSystem&& other) noexcept = default;

	PhysicsSystem::~PhysicsSystem() {
        delete dynamicsWorld;

        delete solver;
        delete overlappingPairCache;
        delete dispatcher;
        delete collisionConfiguration;
    }

	void PhysicsSystem::registerToEvents(EventDispatcher& dispatcher) {
		componentAddedHandle = dispatcher.bindToEvent<ComponentAddedEvent<RigidBodyComponent>>(std::bind(&PhysicsSystem::onComponentAdded, this, std::placeholders::_1));
		componentRemovedHandle = dispatcher.bindToEvent<ComponentRemovedEvent<RigidBodyComponent>>(std::bind(&PhysicsSystem::onComponentRemoved, this, std::placeholders::_1));
	}

	void PhysicsSystem::update(World& world, utl::DeltaTime deltaTime) {
		CLV_PROFILE_FUNCTION();

		//Update the location of the colliders
		for(auto&& [transform, rigidBody] : world.getComponentSets<TransformComponent, CubeColliderComponent>()) {
			
		}

		//Step physics world
        dynamicsWorld->stepSimulation(deltaTime.getDeltaSeconds());

        //TODO
        //collisionManifolds.clear();
        ////TODO: Just use dispatcher-> ?
        //int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
        //for(int i = 0; i < numManifolds; ++i) {
        //    btPersistentManifold* manifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        //    int numContacts                = manifold->getNumContacts();
        //    if(numContacts > 0) {
        //        const btCollisionObject* obA = manifold->getBody0();
        //        const btCollisionObject* obB = manifold->getBody1();

        //        RigidBody* bodyA = static_cast<RigidBody*>(obA->getUserPointer());
        //        RigidBody* bodyB = static_cast<RigidBody*>(obB->getUserPointer());

        //        collisionManifolds.emplace_back(CollisionManifold{ bodyA, bodyB });
        //    }
        //}

		//Retrieve the location of the rigid bodies
        for(auto&& [transform, rigidBody] : world.getComponentSets<TransformComponent, RigidBodyComponent>()) {
            
		}

		//TODO
		//Handle collisions
		/*for(const phy::CollisionManifold& manifold : physicsWorld->getCollisionManifolds()) {
			
		}*/
	}

    //TODO
    /*RigidBody* PhysicsSystem::rayCast(const clv::mth::vec3f& begin, const clv::mth::vec3f& end) {
        btVector3 btBegin{ begin.x, begin.y, begin.z };
        btVector3 btEnd{ end.x, end.y, end.z };

        btCollisionWorld::ClosestRayResultCallback callBack{ btBegin, btEnd };
        dynamicsWorld->rayTest(btBegin, btEnd, callBack);

        if(callBack.m_collisionObject != nullptr) {
            return static_cast<RigidBody*>(callBack.m_collisionObject->getUserPointer());
        } else {
            return nullptr;
        }
    }*/

	void PhysicsSystem::onComponentAdded(const ComponentAddedEvent<RigidBodyComponent>& event) {
		//physicsWorld->addRigidBody(event.component->rigidBody.get());
	}

	void PhysicsSystem::onComponentRemoved(const ComponentRemovedEvent<RigidBodyComponent>& event) {
		//physicsWorld->removeRigidBody(event.component->rigidBody.get());
	}
}