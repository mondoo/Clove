#include "Stem/Systems/PhysicsSystem.hpp"

#include "Stem/Components/CollisionResponseComponent.hpp"
#include "Stem/Components/CubeColliderComponent.hpp"
#include "Stem/Components/RigidBodyComponent.hpp"
#include "Stem/Components/TransformComponent.hpp"

#include <Bulb/ECS/World.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Root/Event/EventDispatcher.hpp>
#include <btBulletDynamicsCommon.h>

using namespace clv;

namespace garlic::inline stem {
    PhysicsSystem::PhysicsSystem() {
        collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
        dispatcher             = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
        overlappingPairCache   = std::make_unique<btDbvtBroadphase>();
        solver                 = std::make_unique<btSequentialImpulseConstraintSolver>();

        dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), overlappingPairCache.get(), solver.get(), collisionConfiguration.get());
    }

    PhysicsSystem::PhysicsSystem(PhysicsSystem &&other) noexcept {
        collisionConfiguration = std::move(other.collisionConfiguration);
        dispatcher             = std::move(other.dispatcher);
        overlappingPairCache   = std::move(other.overlappingPairCache);
        solver                 = std::move(other.solver);

        dynamicsWorld = std::move(other.dynamicsWorld);

        cubeColliderAddedHandle   = std::move(other.cubeColliderAddedHandle);
        cubeColliderRemovedHandle = std::move(other.cubeColliderRemovedHandle);

        rigidBodyAddedHandle   = std::move(other.rigidBodyAddedHandle);
        rigidBodyRemovedHandle = std::move(other.rigidBodyRemovedHandle);

        currentCollisions = std::move(other.currentCollisions);
    }

    PhysicsSystem &PhysicsSystem::operator=(PhysicsSystem &&other) noexcept = default;

    PhysicsSystem::~PhysicsSystem() = default;

    void PhysicsSystem::registerToEvents(garlic::EventDispatcher &dispatcher) {
        cubeColliderAddedHandle = dispatcher.bindToEvent<blb::ecs::ComponentAddedEvent<CubeColliderComponent>>([this](blb::ecs::ComponentAddedEvent<CubeColliderComponent> const &event) {
            onCubeColliderAdded(event);
        });

        cubeColliderRemovedHandle = dispatcher.bindToEvent<blb::ecs::ComponentRemovedEvent<CubeColliderComponent>>([this](blb::ecs::ComponentRemovedEvent<CubeColliderComponent> const &event) {
            onCubeColliderRemoved(event);
        });

        rigidBodyAddedHandle = dispatcher.bindToEvent<blb::ecs::ComponentAddedEvent<RigidBodyComponent>>([this](blb::ecs::ComponentAddedEvent<RigidBodyComponent> const &event) {
            onRigidBodyAdded(event);
        });

        rigidBodyRemovedHandle = dispatcher.bindToEvent<blb::ecs::ComponentRemovedEvent<RigidBodyComponent>>([this](blb::ecs::ComponentRemovedEvent<RigidBodyComponent> const &event) {
            onRigidBodyRemoved(event);
        });
    }

    void PhysicsSystem::preUpdate(blb::ecs::World &world) {
        CLV_PROFILE_FUNCTION();

        //Make sure any colliders are properly paired with their rigid body
        for(auto &&[cubeCollider, rigidBody] : world.getComponentSets<CubeColliderComponent, RigidBodyComponent>()) {
            if(cubeCollider->collisionObject != nullptr) {
                dynamicsWorld->removeCollisionObject(cubeCollider->collisionObject.get());
                cubeCollider->collisionObject.reset();
            }

            if(rigidBody->standInShape != nullptr) {
                dynamicsWorld->removeCollisionObject(rigidBody->body.get());
                rigidBody->body->setCollisionShape(cubeCollider->collisionShape.get());
                addBodyToWorld(*rigidBody);

                rigidBody->standInShape.reset();
            }
        }

        //Make sure any recently un-paired colliders are updated
        for(auto &&[cubeCollider] : world.getComponentSets<CubeColliderComponent>()) {
            if(world.hasComponent<RigidBodyComponent>(cubeCollider->getEntityID())) {
                continue;
            }

            if(cubeCollider->collisionObject == nullptr) {
                cubeCollider->constructCollisionObject();
                addColliderToWorld(*cubeCollider);
            }
        }

        //Make sure any recently un-paired rigid bodies are updated
        for(auto &&[rigidBody] : world.getComponentSets<RigidBodyComponent>()) {
            if(world.hasComponent<CubeColliderComponent>(rigidBody->getEntityID())) {
                continue;
            }

            if(rigidBody->standInShape == nullptr) {
                btRigidBody *body = rigidBody->body.get();

                dynamicsWorld->removeCollisionObject(body);
                rigidBody->standInShape = RigidBodyComponent::createStandInShape();
                body->setCollisionShape(rigidBody->standInShape.get());
                addBodyToWorld(*rigidBody);
            }
        }
    }

    void PhysicsSystem::update(blb::ecs::World &world, utl::DeltaTime deltaTime) {
        CLV_PROFILE_FUNCTION();

        auto const cubeColliders = world.getComponentSets<TransformComponent, CubeColliderComponent>();
        auto const rigidBodies   = world.getComponentSets<TransformComponent, RigidBodyComponent>();

        auto const updateCollider = [](TransformComponent const &transform, btCollisionObject &collisionObject) {
            auto const pos = transform.getPosition(TransformSpace::World);
            auto const rot = transform.getRotation(TransformSpace::World);

            btTransform btTrans = collisionObject.getWorldTransform();
            btTrans.setOrigin({ pos.x, pos.y, pos.z });
            btTrans.setRotation({ rot.x, rot.y, rot.z, rot.w });
            collisionObject.setWorldTransform(btTrans);
        };

        //Notify Bullet of the location of the colliders
        for(auto &&[transform, cubeCollider] : cubeColliders) {
            if(world.hasComponent<RigidBodyComponent>(cubeCollider->getEntityID())) {
                continue;
            }

            updateCollider(*transform, *cubeCollider->collisionObject);
        }
        for(auto &&[transform, rigidBody] : rigidBodies) {
            updateCollider(*transform, *rigidBody->body);
        }

        //Step physics world
        dynamicsWorld->stepSimulation(deltaTime.getDeltaSeconds());

        auto const updateTransform = [](TransformComponent &transform, btCollisionObject const &collisionObject) {
            btTransform const &btTrans = collisionObject.getWorldTransform();
            btVector3 const &pos       = btTrans.getOrigin();
            btQuaternion const &rot    = btTrans.getRotation();

            transform.setPosition({ pos.x(), pos.y(), pos.z() }, TransformSpace::World);
            transform.setRotation({ rot.getW(), rot.getX(), rot.getY(), rot.getZ() }, TransformSpace::World);
        };

        //Apply any simulation updates
        for(auto &&[transform, cubeCollider] : cubeColliders) {
            if(world.hasComponent<RigidBodyComponent>(cubeCollider->getEntityID())) {
                continue;
            }

            updateTransform(*transform, *cubeCollider->collisionObject);
        }
        for(auto &&[transform, rigidBody] : rigidBodies) {
            updateTransform(*transform, *rigidBody->body);
        }
    }

    void PhysicsSystem::postUpdate(blb::ecs::World &world) {
        //Gather collision manifolds
        int const numManifolds = dispatcher->getNumManifolds();
        std::unordered_set<CollisionManifold, ManifoldHasher> newCollisions;

        for(int i = 0; i < numManifolds; ++i) {
            btPersistentManifold *manifold = dispatcher->getManifoldByIndexInternal(i);
            if(manifold->getNumContacts() > 0) {
                btCollisionObject const *obA{ manifold->getBody0() };
                btCollisionObject const *obB{ manifold->getBody1() };

                auto const entityA{ static_cast<blb::ecs::EntityID>(obA->getUserIndex()) };
                auto const entityB{ static_cast<blb::ecs::EntityID>(obB->getUserIndex()) };

                newCollisions.emplace(CollisionManifold{ entityA, entityB });
            }
        }

        std::vector<CollisionManifold> collisionBeginManifolds;
        std::vector<CollisionManifold> collisionEndManifolds;

        //Copy any elements not in our cached set
        collisionBeginManifolds.reserve(std::size(newCollisions));
        std::copy_if(std::begin(newCollisions), std::end(newCollisions), std::back_inserter(collisionBeginManifolds), [this](CollisionManifold const &manifold) {
            return !currentCollisions.contains(manifold);
        });

        //Copy any elements not in our new array
        collisionEndManifolds.reserve(std::size(currentCollisions));
        std::copy_if(std::begin(currentCollisions), std::end(currentCollisions), std::back_inserter(collisionEndManifolds), [&](CollisionManifold const &manifold) {
            return !newCollisions.contains(manifold);
        });

        //Broadcast collision begin events
        for(auto const &manifold : collisionBeginManifolds) {
            blb::ecs::Entity const entityA{ world.getEntity(manifold.entityA) };
            blb::ecs::Entity const entityB{ world.getEntity(manifold.entityB) };

            if(auto entityAComp = entityA.getComponent<CollisionResponseComponent>()) {
                entityAComp->onCollisionBegin.broadcast(Collision{ entityA, entityB });
            }

            if(auto entityBComp = entityB.getComponent<CollisionResponseComponent>()) {
                entityBComp->onCollisionBegin.broadcast(Collision{ entityB, entityA });
            }

            currentCollisions.emplace(manifold);
        }

        //Broadcast collision end events
        for(auto const &manifold : collisionEndManifolds) {
            blb::ecs::Entity const entityA{ world.getEntity(manifold.entityA) };
            blb::ecs::Entity const entityB{ world.getEntity(manifold.entityB) };

            if(auto entityAComp = entityA.getComponent<CollisionResponseComponent>()) {
                entityAComp->onCollisionEnd.broadcast(Collision{ entityA, entityB });
            }

            if(auto entityBComp = entityB.getComponent<CollisionResponseComponent>()) {
                entityBComp->onCollisionEnd.broadcast(Collision{ entityB, entityA });
            }

            currentCollisions.erase(manifold);
        }
    }

    blb::ecs::EntityID PhysicsSystem::rayCast(clv::mth::vec3f const &begin, clv::mth::vec3f const &end) {
        return rayCast(begin, end, ~0, ~0);
    }

    blb::ecs::EntityID PhysicsSystem::rayCast(clv::mth::vec3f const &begin, clv::mth::vec3f const &end, uint32_t const collisionGroup, uint32_t const collisionMask) {
        btVector3 const btBegin{ begin.x, begin.y, begin.z };
        btVector3 const btEnd{ end.x, end.y, end.z };

        btCollisionWorld::ClosestRayResultCallback callBack{ btBegin, btEnd };
        callBack.m_collisionFilterGroup = collisionGroup;
        callBack.m_collisionFilterMask  = collisionMask;
        dynamicsWorld->rayTest(btBegin, btEnd, callBack);

        if(callBack.m_collisionObject != nullptr) {
            return callBack.m_collisionObject->getUserIndex();
        } else {
            return blb::ecs::INVALID_ENTITY_ID;
        }
    }

    void PhysicsSystem::onCubeColliderAdded(blb::ecs::ComponentAddedEvent<CubeColliderComponent> const &event) {
        addColliderToWorld(*event.component);
    }

    void PhysicsSystem::onCubeColliderRemoved(blb::ecs::ComponentRemovedEvent<CubeColliderComponent> const &event) {
        if(btCollisionObject *object = event.component->collisionObject.get()) {
            dynamicsWorld->removeCollisionObject(object);
        }
    }

    void PhysicsSystem::onRigidBodyAdded(blb::ecs::ComponentAddedEvent<RigidBodyComponent> const &event) {
        addBodyToWorld(*event.component);
    }

    void PhysicsSystem::onRigidBodyRemoved(blb::ecs::ComponentRemovedEvent<RigidBodyComponent> const &event) {
        dynamicsWorld->removeCollisionObject(event.component->body.get());
    }

    void PhysicsSystem::addBodyToWorld(RigidBodyComponent const &rigidBodyComponent) {
        dynamicsWorld->addRigidBody(rigidBodyComponent.body.get(), rigidBodyComponent.descriptor.collisionGroup, rigidBodyComponent.descriptor.collisionMask);
        rigidBodyComponent.body->setUserIndex(rigidBodyComponent.getEntityID());
    }

    void PhysicsSystem::addColliderToWorld(CubeColliderComponent const &colliderComponent) {
        dynamicsWorld->addCollisionObject(colliderComponent.collisionObject.get(), ~0, ~0);//Add the collider to every group and collide with every other group
        colliderComponent.collisionObject->setUserIndex(colliderComponent.getEntityID());
    }
}