#include "Bulb/ECS/Systems/PhysicsSystem.hpp"

#include "Bulb/ECS/Components/CubeColliderComponent.hpp"
#include "Bulb/ECS/Components/RigidBodyComponent.hpp"
#include "Bulb/ECS/Components/TransformComponent.hpp"
#include "Bulb/ECS/World.hpp"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
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
        cubeColliderAddedHandle = dispatcher.bindToEvent<ComponentAddedEvent<CubeColliderComponent>>([this](const ComponentAddedEvent<CubeColliderComponent>& event) {
            onCubeColliderAdded(event);
        });

        cubeColliderRemovedHandle = dispatcher.bindToEvent<ComponentRemovedEvent<CubeColliderComponent>>([this](const ComponentRemovedEvent<CubeColliderComponent>& event) {
            onCubeColliderRemoved(event);
        });

        rigidBodyAddedHandle = dispatcher.bindToEvent<ComponentAddedEvent<RigidBodyComponent>>([this](const ComponentAddedEvent<RigidBodyComponent>& event) {
            onRigidBodyAdded(event);
        });

        rigidBodyRemovedHandle = dispatcher.bindToEvent<ComponentRemovedEvent<RigidBodyComponent>>([this](const ComponentRemovedEvent<RigidBodyComponent>& event) {
            onRigidBodyRemoved(event);
        });
    }

    void PhysicsSystem::preUpdate(World& world) {
        CLV_PROFILE_FUNCTION();

        //Make sure any colliders are properly paired with their rigid body
        for(auto&& [cubeCollider, rigidBody] : world.getComponentSets<CubeColliderComponent, RigidBodyComponent>()) {
            if(cubeCollider->collisionObject != nullptr) {
                dynamicsWorld->removeCollisionObject(cubeCollider->collisionObject.get());
                cubeCollider->collisionObject.reset();
            }

            if(rigidBody->standInShape != nullptr) {
                dynamicsWorld->removeCollisionObject(rigidBody->body.get());
                rigidBody->body->setCollisionShape(cubeCollider->collisionShape.get());
                dynamicsWorld->addRigidBody(rigidBody->body.get());

                rigidBody->standInShape.reset();
            }
        }

        //Make sure any recently un-paired colliders are updated
        for(auto&& [cubeCollider] : world.getComponentSets<CubeColliderComponent>()) {
            if(world.hasComponent<RigidBodyComponent>(cubeCollider->getEntityID())) {
                continue;
            }

            if(cubeCollider->collisionObject == nullptr) {
                cubeCollider->collisionObject = std::make_unique<btGhostObject>();
                cubeCollider->collisionObject->setCollisionShape(cubeCollider->collisionShape.get());
                dynamicsWorld->addCollisionObject(cubeCollider->collisionObject.get());
            }
        }

        //Make sure any recently un-paired rigid bodies are updated
        for(auto&& [rigidBody] : world.getComponentSets<RigidBodyComponent>()) {
            if(world.hasComponent<CubeColliderComponent>(rigidBody->getEntityID())) {
                continue;
            }

            if(rigidBody->standInShape == nullptr) {
                btRigidBody* body = rigidBody->body.get();

                dynamicsWorld->removeCollisionObject(body);
                rigidBody->standInShape = RigidBodyComponent::createStandInShape();
                body->setCollisionShape(rigidBody->standInShape.get());
                dynamicsWorld->addRigidBody(body);
            }
        }
    }

    void PhysicsSystem::update(World& world, utl::DeltaTime deltaTime) {
        CLV_PROFILE_FUNCTION();

        const auto cubeColliders = world.getComponentSets<TransformComponent, CubeColliderComponent>();
        const auto rigidBodies   = world.getComponentSets<TransformComponent, RigidBodyComponent>();

        const auto updateCollider = [](const TransformComponent& transform, btCollisionObject& collisionObject) {
            const auto pos = transform.getPosition(TransformSpace::World);
            const auto rot = transform.getRotation(TransformSpace::World);

            btTransform btTrans = collisionObject.getWorldTransform();
            btTrans.setOrigin({ pos.x, pos.y, pos.z });
            btTrans.setRotation({ rot.x, rot.y, rot.z, rot.w });
           collisionObject.setWorldTransform(btTrans);
        };

        //Notify Bullet of the location of the colliders
        for(auto&& [transform, cubeCollider] : cubeColliders) {
            if(world.hasComponent<RigidBodyComponent>(cubeCollider->getEntityID())) {
                continue;
            }

            updateCollider(*transform, *cubeCollider->collisionObject);
        }
        for(auto&& [transform, rigidBody] : rigidBodies) {
            updateCollider(*transform, *rigidBody->body);

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

        const auto updateTransform = [](TransformComponent& transform, const btCollisionObject& collisionObject) {
            const btTransform& btTrans = collisionObject.getWorldTransform();
            const btVector3& pos       = btTrans.getOrigin();
            const btQuaternion& rot    = btTrans.getRotation();

            transform.setPosition({ pos.x(), pos.y(), pos.z() }, TransformSpace::World);
            transform.setRotation({ rot.getW(), rot.getX(), rot.getY(), rot.getZ() }, TransformSpace::World);
        };

        //Apply any simulation updates
        for(auto&& [transform, cubeCollider] : cubeColliders) {
            if(world.hasComponent<RigidBodyComponent>(cubeCollider->getEntityID())) {
                continue;
            }

            updateTransform(*transform, *cubeCollider->collisionObject);
        }
        for(auto&& [transform, rigidBody] : rigidBodies) {
            updateTransform(*transform, *rigidBody->body);
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

    void PhysicsSystem::onCubeColliderAdded(const ComponentAddedEvent<CubeColliderComponent>& event) {
        dynamicsWorld->addCollisionObject(event.component->collisionObject.get());
    }

    void PhysicsSystem::onCubeColliderRemoved(const ComponentRemovedEvent<CubeColliderComponent>& event) {
        if(btCollisionObject* object = event.component->collisionObject.get()) {
            dynamicsWorld->removeCollisionObject(object);
        }
    }

    void PhysicsSystem::onRigidBodyAdded(const ComponentAddedEvent<RigidBodyComponent>& event) {
        dynamicsWorld->addRigidBody(event.component->body.get());
    }

    void PhysicsSystem::onRigidBodyRemoved(const ComponentRemovedEvent<RigidBodyComponent>& event) {
        dynamicsWorld->removeCollisionObject(event.component->body.get());
    }
}