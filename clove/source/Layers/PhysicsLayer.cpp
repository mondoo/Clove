#include "Clove/Layers/PhysicsLayer.hpp"

#include "Clove/Application.hpp"
#include "Clove/Components/CollisionResponseComponent.hpp"
#include "Clove/Components/CollisionShapeComponent.hpp"
#include "Clove/Components/RigidBodyComponent.hpp"
#include "Clove/Components/TransformComponent.hpp"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Clove/Delegate/MultiCastDelegate.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <Clove/Event/EventDispatcher.hpp>
#include <btBulletDynamicsCommon.h>

namespace garlic::clove {
    PhysicsLayer::PhysicsLayer()
        : Layer("Physics")
        , entityManager{ Application::get().getEntityManager() } {
        collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
        dispatcher             = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
        broadphase             = std::make_unique<btDbvtBroadphase>();
        solver                 = std::make_unique<btSequentialImpulseConstraintSolver>();

        dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), broadphase.get(), solver.get(), collisionConfiguration.get());

        registerToEvents(*entityManager);
    }

    PhysicsLayer::PhysicsLayer(PhysicsLayer &&other) noexcept {
        entityManager = other.entityManager;

        collisionConfiguration = std::move(other.collisionConfiguration);
        dispatcher             = std::move(other.dispatcher);
        broadphase             = std::move(other.broadphase);
        solver                 = std::move(other.solver);

        dynamicsWorld = std::move(other.dynamicsWorld);

        currentCollisions = std::move(other.currentCollisions);

        registerToEvents(*entityManager);
    }

    PhysicsLayer &PhysicsLayer::operator=(PhysicsLayer &&other) noexcept {
        entityManager = other.entityManager;

        collisionConfiguration = std::move(other.collisionConfiguration);
        dispatcher             = std::move(other.dispatcher);
        broadphase             = std::move(other.broadphase);
        solver                 = std::move(other.solver);

        dynamicsWorld = std::move(other.dynamicsWorld);

        currentCollisions = std::move(other.currentCollisions);

        registerToEvents(*entityManager);

        return *this;
    }

    PhysicsLayer::~PhysicsLayer() = default;

    void PhysicsLayer::registerToEvents(EntityManager &entityManager) {
        EventDispatcher &dispatcher{ entityManager.getDispatcher() };

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

    void PhysicsLayer::onUpdate(DeltaTime const deltaTime) {
        CLOVE_PROFILE_FUNCTION();

        //Make sure any colliders are properly paired with their rigid body
        entityManager->forEach([this](CollisionShapeComponent &shape, RigidBodyComponent &body) {
            if(shape.collisionObject != nullptr) {
                dynamicsWorld->removeCollisionObject(shape.collisionObject.get());
                shape.collisionObject.reset();
            }

            if(body.standInShape != nullptr) {
                dynamicsWorld->removeCollisionObject(body.body.get());
                body.standInShape.reset();

                body.body->setCollisionShape(shape.collisionShape.get());

                btVector3 inertia{ 0.0f, 0.0f, 0.0f };
                if(body.descriptor.mass > 0.0f) {
                    shape.collisionShape->calculateLocalInertia(body.descriptor.mass, inertia);
                }
                body.body->setMassProps(body.descriptor.mass, inertia);
                body.body->updateInertiaTensor();

                addBodyToWorld(body);
            }
        });

        //Make sure any recently un-paired colliders are updated
        entityManager->forEach([this](CollisionShapeComponent &shape) {
            if(!entityManager->hasComponent<RigidBodyComponent>(shape.getEntity())) {
                if(shape.collisionObject == nullptr) {
                    shape.constructCollisionObject();
                    addColliderToWorld(shape);
                }
            }
        });

        //Make sure any recently un-paired rigid bodies are updated
        entityManager->forEach([this](RigidBodyComponent &body) {
            if(!entityManager->hasComponent<CollisionShapeComponent>(body.getEntity())) {
                if(body.standInShape == nullptr) {
                    btRigidBody *const btBody{ body.body.get() };

                    dynamicsWorld->removeCollisionObject(btBody);
                    body.standInShape = RigidBodyComponent::createStandInShape();
                    btBody->setCollisionShape(body.standInShape.get());
                    addBodyToWorld(body);
                }
            }
        });

        auto const updateCollider = [](TransformComponent const &transform, btCollisionObject &collisionObject) {
            auto const pos{ transform.position };
            auto const rot{ transform.rotation };

            btTransform btTrans = collisionObject.getWorldTransform();
            btTrans.setOrigin({ pos.x, pos.y, pos.z });
            btTrans.setRotation({ rot.x, rot.y, rot.z, rot.w });
            collisionObject.setWorldTransform(btTrans);
        };

        //Notify Bullet of the location of the colliders
        entityManager->forEach([&](TransformComponent const &transform, CollisionShapeComponent &shape) {
            if(!entityManager->hasComponent<RigidBodyComponent>(shape.getEntity())) {
                updateCollider(transform, *shape.collisionObject);
            }
        });
        entityManager->forEach([&](TransformComponent const &transform, RigidBodyComponent &body) {
            updateCollider(transform, *body.body);
        });

        //Step physics world
        dynamicsWorld->stepSimulation(deltaTime.getDeltaSeconds());

        auto const updateTransform = [](TransformComponent &transform, btCollisionObject const &collisionObject) {
            btTransform const &btTrans{ collisionObject.getWorldTransform() };
            btVector3 const &pos{ btTrans.getOrigin() };
            btQuaternion const &rot{ btTrans.getRotation() };

            transform.position = vec3f{ pos.x(), pos.y(), pos.z() };
            transform.rotation = quatf{ rot.getW(), rot.getX(), rot.getY(), rot.getZ() };
        };

        //Apply any simulation updates
        entityManager->forEach([&](TransformComponent &transform, CollisionShapeComponent const &shape) {
            if(!entityManager->hasComponent<RigidBodyComponent>(shape.getEntity())) {
                updateTransform(transform, *shape.collisionObject);
            }
        });
        entityManager->forEach([&](TransformComponent &transform, RigidBodyComponent const &body) {
            updateTransform(transform, *body.body);
        });

        //TODO: Move collision broadcasting to it's own layer to be handled last

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

            if(entityManager->hasComponent<CollisionResponseComponent>(entityA)) {
                entityManager->getComponent<CollisionResponseComponent>(entityA).onCollisionBegin.broadcast(Collision{ entityA, entityB });
            }

            if(entityManager->hasComponent<CollisionResponseComponent>(entityB)) {
                entityManager->getComponent<CollisionResponseComponent>(entityB).onCollisionBegin.broadcast(Collision{ entityB, entityA });
            }

            currentCollisions.emplace(manifold);
        }

        //Broadcast collision end events
        for(auto const &manifold : collisionEndManifolds) {
            Entity const entityA{ manifold.entityA };
            Entity const entityB{ manifold.entityB };

            if(entityManager->hasComponent<CollisionResponseComponent>(entityA)) {
                entityManager->getComponent<CollisionResponseComponent>(entityA).onCollisionEnd.broadcast(Collision{ entityA, entityB });
            }

            if(entityManager->hasComponent<CollisionResponseComponent>(entityB)) {
                entityManager->getComponent<CollisionResponseComponent>(entityB).onCollisionEnd.broadcast(Collision{ entityB, entityA });
            }

            currentCollisions.erase(manifold);
        }
    }

    void PhysicsLayer::setGravity(vec3f const &gravity) {
        dynamicsWorld->setGravity({ gravity.x, gravity.y, gravity.z });
    }

    Entity PhysicsLayer::rayCast(vec3f const &begin, vec3f const &end) {
        return rayCast(begin, end, ~0, ~0);
    }

    Entity PhysicsLayer::rayCast(vec3f const &begin, vec3f const &end, uint32_t const collisionGroup, uint32_t const collisionMask) {
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

    void PhysicsLayer::onCollisionShapeAdded(ComponentAddedEvent<CollisionShapeComponent> const &event) {
        addColliderToWorld(*event.component);
    }

    void PhysicsLayer::onCollisionShapeRemoved(ComponentRemovedEvent<CollisionShapeComponent> const &event) {
        if(btCollisionObject *object = event.component->collisionObject.get()) {
            dynamicsWorld->removeCollisionObject(object);
        }
    }

    void PhysicsLayer::onRigidBodyAdded(ComponentAddedEvent<RigidBodyComponent> const &event) {
        addBodyToWorld(*event.component);
    }

    void PhysicsLayer::onRigidBodyRemoved(ComponentRemovedEvent<RigidBodyComponent> const &event) {
        dynamicsWorld->removeCollisionObject(event.component->body.get());
    }

    void PhysicsLayer::addBodyToWorld(RigidBodyComponent const &rigidBodyComponent) {
        dynamicsWorld->addRigidBody(rigidBodyComponent.body.get(), rigidBodyComponent.descriptor.collisionGroup, rigidBodyComponent.descriptor.collisionMask);
        rigidBodyComponent.body->setUserIndex(rigidBodyComponent.getEntity());
    }

    void PhysicsLayer::addColliderToWorld(CollisionShapeComponent const &colliderComponent) {
        dynamicsWorld->addCollisionObject(colliderComponent.collisionObject.get(), ~0, ~0);//Add the collider to every group and collide with every other group
        colliderComponent.collisionObject->setUserIndex(colliderComponent.getEntity());
    }
}