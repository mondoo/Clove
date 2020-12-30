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
    namespace {
        template<typename... Ts>
        struct match : Ts... { using Ts::operator()...; };
        template<typename... Ts>
        match(Ts...) -> match<Ts...>;

        /**
         * @brief Added to an entity when the system knows about it. Contains the Bullet types.
         */
        struct PhysicsProxyComponent {
            std::unique_ptr<btCollisionObject> collisionObject;
            std::unique_ptr<btCollisionShape> collisionShape;
        };

        /**
         * @brief Tag component for entities that physics knows only about it's shape.
         */
        struct ShapeOnlyComponent {};
        /**
         * @brief Tag component for entities that physics knows only about it's rigid body.
         */
        struct BodyOnlyComponent {};

        /**
         * @brief Initialises proxy with the correct shape.
         */
        void createProxyShape(PhysicsProxyComponent &proxy, CollisionShapeComponent const &shape) {
            std::visit(match{
                           [&](CollisionShapeComponent::Sphere const &sphere) { proxy.collisionShape = std::make_unique<btSphereShape>(sphere.radius); },
                           [&](CollisionShapeComponent::Cube const &cube) { proxy.collisionShape = std::make_unique<btBoxShape>(btVector3{ cube.halfExtents.x, cube.halfExtents.y, cube.halfExtents.z }); },
                       },
                       shape.shape);
        }

        /**
         * @brief Initialises proxy with a btRigidBody.
         * @return The created body so it can be added to the world.
         */
        btRigidBody *createProxyBody(PhysicsProxyComponent &proxy, RigidBodyComponent const &body) {
            btVector3 localInertia{ 0.0f, 0.0f, 0.0f };
            if(body.mass > 0.0f) {
                proxy.collisionShape->calculateLocalInertia(body.mass, localInertia);
            }

            auto proxyBody{ std::make_unique<btRigidBody>(btRigidBody::btRigidBodyConstructionInfo{ body.mass, nullptr, proxy.collisionShape.get(), localInertia }) };
            btRigidBody *rawBody{ proxyBody.get() };

            proxy.collisionObject = std::move(proxyBody);

            return rawBody;
        }

        btVector3 toBt(vec3f const &vec) {
            return btVector3{ vec.x, vec.y, vec.z };
        }
    }

    PhysicsLayer::PhysicsLayer()
        : Layer("Physics")
        , entityManager{ Application::get().getEntityManager() } {
        collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
        dispatcher             = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
        broadphase             = std::make_unique<btDbvtBroadphase>();
        solver                 = std::make_unique<btSequentialImpulseConstraintSolver>();

        dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), broadphase.get(), solver.get(), collisionConfiguration.get());
    }

    PhysicsLayer::PhysicsLayer(PhysicsLayer &&other) noexcept = default;

    PhysicsLayer &PhysicsLayer::operator=(PhysicsLayer &&other) noexcept = default;

    PhysicsLayer::~PhysicsLayer() = default;

    void PhysicsLayer::onUpdate(DeltaTime const deltaTime) {
        CLOVE_PROFILE_FUNCTION();

        //If any component compositions have changed then these need to be reset
        entityManager->forEach([&](Entity entity, CollisionShapeComponent const &shape, RigidBodyComponent const &body, PhysicsProxyComponent const &proxy, ShapeOnlyComponent const &shapeOnly) {
            dynamicsWorld->removeCollisionObject(proxy.collisionObject.get());
            entityManager->removeComponent<PhysicsProxyComponent>(entity);
            entityManager->removeComponent<ShapeOnlyComponent>(entity);
        });
        entityManager->forEach([&](Entity entity, CollisionShapeComponent const &shape, RigidBodyComponent const &body, PhysicsProxyComponent const &proxy, BodyOnlyComponent const &shapeOnly) {
            dynamicsWorld->removeCollisionObject(proxy.collisionObject.get());
            entityManager->removeComponent<PhysicsProxyComponent>(entity);
            entityManager->removeComponent<BodyOnlyComponent>(entity);
        });
        entityManager->forEach([&](Entity entity, PhysicsProxyComponent const &proxy) {
            if(!entityManager->hasComponent<CollisionShapeComponent>(entity) || !entityManager->hasComponent<RigidBodyComponent>(entity)) {
                dynamicsWorld->removeCollisionObject(proxy.collisionObject.get());
                entityManager->removeComponent<PhysicsProxyComponent>(entity);
            }
        },
                               Exclude<ShapeOnlyComponent, BodyOnlyComponent>{});

        //Create proxies for any new / modified entities
        entityManager->forEach(&PhysicsLayer::initialiseCollisionShape, this, Exclude<PhysicsProxyComponent, RigidBodyComponent>{});
        entityManager->forEach(&PhysicsLayer::initialiseRigidBody, this, Exclude<PhysicsProxyComponent, CollisionShapeComponent>{});
        entityManager->forEach(&PhysicsLayer::initialiseRigidBodyShape, this, Exclude<PhysicsProxyComponent>{});

        //Apply any updates to the rigid body
        entityManager->forEach([&](RigidBodyComponent const &body, PhysicsProxyComponent const &proxy) {
            auto *btBody{ static_cast<btRigidBody*>(proxy.collisionObject.get()) };

            if(body.linearVelocity.has_value()) {
                btBody->setLinearVelocity(toBt(*body.linearVelocity));
            }

            btBody->setRestitution(body.restitution);

            btBody->setAngularFactor(toBt(body.angularFactor));
            btBody->setLinearFactor(toBt(body.linearFactor));

            if(body.appliedForce.has_value()) {
                btBody->applyForce(toBt(body.appliedForce->amount), toBt(body.appliedForce->offset));
            }

            if(body.appliedImpulse.has_value()) {
                btBody->applyForce(toBt(body.appliedImpulse->amount), toBt(body.appliedImpulse->offset));
            }
        });

        //Notify Bullet of the location of the colliders
        entityManager->forEach([&](TransformComponent const &transform, PhysicsProxyComponent const &proxy) {
            auto const &pos{ transform.position };
            auto const &rot{ transform.rotation };

            btTransform btTrans = proxy.collisionObject->getWorldTransform();
            btTrans.setOrigin({ pos.x, pos.y, pos.z });
            btTrans.setRotation({ rot.x, rot.y, rot.z, rot.w });
            proxy.collisionObject->setWorldTransform(btTrans);
        });

        //Step physics world
        dynamicsWorld->stepSimulation(deltaTime.getDeltaSeconds());

        //Apply any simulation updates
        entityManager->forEach([&](TransformComponent &transform, PhysicsProxyComponent const &proxy) {
            btTransform const &btTrans{ proxy.collisionObject->getWorldTransform() };
            btVector3 const &pos{ btTrans.getOrigin() };
            btQuaternion const &rot{ btTrans.getRotation() };

            transform.position = vec3f{ pos.x(), pos.y(), pos.z() };
            transform.rotation = quatf{ rot.getW(), rot.getX(), rot.getY(), rot.getZ() };
        });

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

    void PhysicsLayer::initialiseCollisionShape(Entity entity, CollisionShapeComponent const &shape) {
        PhysicsProxyComponent proxy{
            .collisionObject = std::make_unique<btGhostObject>(),
        };

        createProxyShape(proxy, shape);

        proxy.collisionObject->setCollisionShape(proxy.collisionShape.get());
        proxy.collisionObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
        proxy.collisionObject->setUserIndex(entity);

        dynamicsWorld->addCollisionObject(proxy.collisionObject.get(), ~0, ~0);//Add the collider to every group and collide with every other group

        entityManager->addComponent<PhysicsProxyComponent>(entity, std::move(proxy));
        entityManager->addComponent<ShapeOnlyComponent>(entity);
    }

    void PhysicsLayer::initialiseRigidBody(Entity entity, RigidBodyComponent const &body) {
        PhysicsProxyComponent proxy{
            .collisionShape = std::make_unique<btSphereShape>(0.1f),
        };

        btRigidBody *rawBody{ createProxyBody(proxy, body) };

        dynamicsWorld->addRigidBody(rawBody, body.collisionGroup, body.collisionMask);

        entityManager->addComponent<PhysicsProxyComponent>(entity, std::move(proxy));
        entityManager->addComponent<BodyOnlyComponent>(entity);
    }

    void PhysicsLayer::initialiseRigidBodyShape(Entity entity, CollisionShapeComponent const &shape, RigidBodyComponent const &body) {
        PhysicsProxyComponent proxy{};

        createProxyShape(proxy, shape);
        btRigidBody *rawBody{ createProxyBody(proxy, body) };

        dynamicsWorld->addRigidBody(rawBody, body.collisionGroup, body.collisionMask);

        entityManager->addComponent<PhysicsProxyComponent>(entity, std::move(proxy));
    }
}