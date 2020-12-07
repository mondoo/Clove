#include "Clove/Systems/PhysicsSystem.hpp"

#include "Clove/Components/CollisionResponseComponent.hpp"
#include "Clove/Components/CollisionShapeComponent.hpp"
#include "Clove/Components/RigidBodyComponent.hpp"
#include "Clove/Components/TransformComponent.hpp"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Clove/Delegate/MultiCastDelegate.hpp>
#include <Clove/ECS/World.hpp>
#include <Clove/Event/EventDispatcher.hpp>
#include <btBulletDynamicsCommon.h>

namespace garlic::clove {
    PhysicsSystem::PhysicsSystem() {
        collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
        dispatcher             = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
        broadphase             = std::make_unique<btDbvtBroadphase>();
        solver                 = std::make_unique<btSequentialImpulseConstraintSolver>();

        dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), broadphase.get(), solver.get(), collisionConfiguration.get());
    }

    PhysicsSystem::PhysicsSystem(PhysicsSystem &&other) noexcept {
        collisionConfiguration = std::move(other.collisionConfiguration);
        dispatcher             = std::move(other.dispatcher);
        broadphase             = std::move(other.broadphase);
        solver                 = std::move(other.solver);

        dynamicsWorld = std::move(other.dynamicsWorld);

        collisionShapeAddedHandle   = std::move(other.collisionShapeAddedHandle);
        collisionShapeRemovedHandle = std::move(other.collisionShapeRemovedHandle);

        rigidBodyAddedHandle   = std::move(other.rigidBodyAddedHandle);
        rigidBodyRemovedHandle = std::move(other.rigidBodyRemovedHandle);

        currentCollisions = std::move(other.currentCollisions);
    }

    PhysicsSystem &PhysicsSystem::operator=(PhysicsSystem &&other) noexcept = default;

    PhysicsSystem::~PhysicsSystem() = default;

    void PhysicsSystem::registerToEvents(EventDispatcher &dispatcher) {
        collisionShapeAddedHandle   = dispatcher.bindToEvent<ComponentAddedEvent<CollisionShapeComponent>>([this](ComponentAddedEvent<CollisionShapeComponent> const &event) {
            onCollisionShapeAdded(event);
        });
        collisionShapeRemovedHandle = dispatcher.bindToEvent<ComponentRemovedEvent<CollisionShapeComponent>>([this](ComponentRemovedEvent<CollisionShapeComponent> const &event) {
            onCollisionShapeRemoved(event);
        });

        rigidBodyAddedHandle   = dispatcher.bindToEvent<ComponentAddedEvent<RigidBodyComponent>>([this](ComponentAddedEvent<RigidBodyComponent> const &event) {
            onRigidBodyAdded(event);
        });
        rigidBodyRemovedHandle = dispatcher.bindToEvent<ComponentRemovedEvent<RigidBodyComponent>>([this](ComponentRemovedEvent<RigidBodyComponent> const &event) {
            onRigidBodyRemoved(event);
        });
    }

    void PhysicsSystem::preUpdate(World &world) {
        CLOVE_PROFILE_FUNCTION();

        //Make sure any colliders are properly paired with their rigid body
        for(auto &&[collider, rigidBody] : world.getComponentSets<CollisionShapeComponent, RigidBodyComponent>()) {
            if(collider->collisionObject != nullptr) {
                dynamicsWorld->removeCollisionObject(collider->collisionObject.get());
                collider->collisionObject.reset();
            }

            if(rigidBody->standInShape != nullptr) {
                dynamicsWorld->removeCollisionObject(rigidBody->body.get());
                rigidBody->standInShape.reset();

                rigidBody->body->setCollisionShape(collider->collisionShape.get());

                btVector3 inertia{ 0.0f, 0.0f, 0.0f };
                if(rigidBody->descriptor.mass > 0.0f) {
                    collider->collisionShape->calculateLocalInertia(rigidBody->descriptor.mass, inertia);
                }
                rigidBody->body->setMassProps(rigidBody->descriptor.mass, inertia);
                rigidBody->body->updateInertiaTensor();

                addBodyToWorld(*rigidBody);
            }
        }

        //Make sure any recently un-paired colliders are updated
        for(auto &&[collider] : world.getComponentSets<CollisionShapeComponent>()) {
            if(world.hasComponent<RigidBodyComponent>(collider->getEntity())) {
                continue;
            }

            if(collider->collisionObject == nullptr) {
                collider->constructCollisionObject();
                addColliderToWorld(*collider);
            }
        }

        //Make sure any recently un-paired rigid bodies are updated
        for(auto &&[rigidBody] : world.getComponentSets<RigidBodyComponent>()) {
            if(world.hasComponent<CollisionShapeComponent>(rigidBody->getEntity())) {
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

    void PhysicsSystem::update(World &world, DeltaTime deltaTime) {
        CLOVE_PROFILE_FUNCTION();

        auto const collisionShapes{ world.getComponentSets<TransformComponent, CollisionShapeComponent>() };
        auto const rigidBodies{ world.getComponentSets<TransformComponent, RigidBodyComponent>() };

        auto const updateCollider = [](TransformComponent const &transform, btCollisionObject &collisionObject) {
            auto const pos{ transform.getPosition(TransformSpace::World) };
            auto const rot{ transform.getRotation(TransformSpace::World) };

            btTransform btTrans = collisionObject.getWorldTransform();
            btTrans.setOrigin({ pos.x, pos.y, pos.z });
            btTrans.setRotation({ rot.x, rot.y, rot.z, rot.w });
            collisionObject.setWorldTransform(btTrans);
        };

        //Notify Bullet of the location of the colliders
        for(auto &&[transform, collider] : collisionShapes) {
            if(world.hasComponent<RigidBodyComponent>(collider->getEntity())) {
                continue;
            }

            updateCollider(*transform, *collider->collisionObject);
        }
        for(auto &&[transform, rigidBody] : rigidBodies) {
            updateCollider(*transform, *rigidBody->body);
        }

        //Step physics world
        dynamicsWorld->stepSimulation(deltaTime.getDeltaSeconds());

        auto const updateTransform = [](TransformComponent &transform, btCollisionObject const &collisionObject) {
            btTransform const &btTrans{ collisionObject.getWorldTransform() };
            btVector3 const &pos{ btTrans.getOrigin() };
            btQuaternion const &rot{ btTrans.getRotation() };

            transform.setPosition({ pos.x(), pos.y(), pos.z() }, TransformSpace::World);
            transform.setRotation({ rot.getW(), rot.getX(), rot.getY(), rot.getZ() }, TransformSpace::World);
        };

        //Apply any simulation updates
        for(auto &&[transform, collider] : collisionShapes) {
            if(world.hasComponent<RigidBodyComponent>(collider->getEntity())) {
                continue;
            }

            updateTransform(*transform, *collider->collisionObject);
        }
        for(auto &&[transform, rigidBody] : rigidBodies) {
            updateTransform(*transform, *rigidBody->body);
        }
    }

    void PhysicsSystem::postUpdate(World &world) {
        //Gather collision manifolds
        int const numManifolds = dispatcher->getNumManifolds();
        std::unordered_set<CollisionManifold, ManifoldHasher> newCollisions;

        for(int i = 0; i < numManifolds; ++i) {
            btPersistentManifold *manifold = dispatcher->getManifoldByIndexInternal(i);
            if(manifold->getNumContacts() > 0) {
                btCollisionObject const *obA{ manifold->getBody0() };
                btCollisionObject const *obB{ manifold->getBody1() };

                auto const entityA{ static_cast<Entity>(obA->getUserIndex()) };
                auto const entityB{ static_cast<Entity>(obB->getUserIndex()) };

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
            Entity const entityA{ manifold.entityA };
            Entity const entityB{ manifold.entityB };

            if(auto entityAComp = world.getComponent<CollisionResponseComponent>(entityA)) {
                entityAComp->onCollisionBegin.broadcast(Collision{ entityA, entityB });
            }

            if(auto entityBComp = world.getComponent<CollisionResponseComponent>(entityB)) {
                entityBComp->onCollisionBegin.broadcast(Collision{ entityB, entityA });
            }

            currentCollisions.emplace(manifold);
        }

        //Broadcast collision end events
        for(auto const &manifold : collisionEndManifolds) {
            Entity const entityA{ manifold.entityA };
            Entity const entityB{ manifold.entityB };

            if(auto entityAComp = world.getComponent<CollisionResponseComponent>(entityA)) {
                entityAComp->onCollisionEnd.broadcast(Collision{ entityA, entityB });
            }

            if(auto entityBComp = world.getComponent<CollisionResponseComponent>(entityB)) {
                entityBComp->onCollisionEnd.broadcast(Collision{ entityB, entityA });
            }

            currentCollisions.erase(manifold);
        }
    }

    void PhysicsSystem::setGravity(vec3f const &gravity) {
        dynamicsWorld->setGravity({ gravity.x, gravity.y, gravity.z });
    }

    Entity PhysicsSystem::rayCast(vec3f const &begin, vec3f const &end) {
        return rayCast(begin, end, ~0, ~0);
    }

    Entity PhysicsSystem::rayCast(vec3f const &begin, vec3f const &end, uint32_t const collisionGroup, uint32_t const collisionMask) {
        btVector3 const btBegin{ begin.x, begin.y, begin.z };
        btVector3 const btEnd{ end.x, end.y, end.z };

        btCollisionWorld::ClosestRayResultCallback callBack{ btBegin, btEnd };
        callBack.m_collisionFilterGroup = collisionGroup;
        callBack.m_collisionFilterMask  = collisionMask;
        dynamicsWorld->rayTest(btBegin, btEnd, callBack);

        if(callBack.m_collisionObject != nullptr) {
            return callBack.m_collisionObject->getUserIndex();
        } else {
            return NullEntity;
        }
    }

    void PhysicsSystem::onCollisionShapeAdded(ComponentAddedEvent<CollisionShapeComponent> const &event) {
        addColliderToWorld(*event.component);
    }

    void PhysicsSystem::onCollisionShapeRemoved(ComponentRemovedEvent<CollisionShapeComponent> const &event) {
        if(btCollisionObject *object = event.component->collisionObject.get()) {
            dynamicsWorld->removeCollisionObject(object);
        }
    }

    void PhysicsSystem::onRigidBodyAdded(ComponentAddedEvent<RigidBodyComponent> const &event) {
        addBodyToWorld(*event.component);
    }

    void PhysicsSystem::onRigidBodyRemoved(ComponentRemovedEvent<RigidBodyComponent> const &event) {
        dynamicsWorld->removeCollisionObject(event.component->body.get());
    }

    void PhysicsSystem::addBodyToWorld(RigidBodyComponent const &rigidBodyComponent) {
        dynamicsWorld->addRigidBody(rigidBodyComponent.body.get(), rigidBodyComponent.descriptor.collisionGroup, rigidBodyComponent.descriptor.collisionMask);
        rigidBodyComponent.body->setUserIndex(rigidBodyComponent.getEntity());
    }

    void PhysicsSystem::addColliderToWorld(CollisionShapeComponent const &colliderComponent) {
        dynamicsWorld->addCollisionObject(colliderComponent.collisionObject.get(), ~0, ~0);//Add the collider to every group and collide with every other group
        colliderComponent.collisionObject->setUserIndex(colliderComponent.getEntity());
    }
}