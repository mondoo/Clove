#pragma once

#include "Clove/Layer.hpp"

#include <Clove/ECS/ECSEvents.hpp>
#include <Clove/ECS/Entity.hpp>
#include <Clove/Event/EventHandle.hpp>
#include <Clove/Maths/Vector.hpp>
#include <Clove/DeltaTime.hpp>
#include <unordered_set>
#include <memory>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace garlic::clove {
    class CollisionShapeComponent;
    class RigidBodyComponent;
    class World;
}

namespace garlic::clove {
    class PhysicsLayer : public Layer {
        //TYPES
    private:
        struct CollisionManifold {
            Entity entityA;
            Entity entityB;

            constexpr friend bool operator==(CollisionManifold const &lhs, CollisionManifold const &rhs) {
                return (lhs.entityA == rhs.entityA && lhs.entityB == rhs.entityB) ||
                    (lhs.entityA == rhs.entityB && lhs.entityB == rhs.entityA);
            }
            constexpr friend bool operator!=(CollisionManifold const &lhs, CollisionManifold const &rhs) {
                return !(lhs == rhs);
            }
        };

        struct ManifoldHasher {
            size_t operator()(CollisionManifold const &manifold) const {
                size_t a = std::hash<Entity>{}(manifold.entityA);
                size_t b = std::hash<Entity>{}(manifold.entityB);
                return a ^ (b << 1);
            }
        };

        //VARIABLES
    private:
        World *ecsWorld{ nullptr };

        std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
        std::unique_ptr<btCollisionDispatcher> dispatcher;
        std::unique_ptr<btBroadphaseInterface> broadphase;
        std::unique_ptr<btSequentialImpulseConstraintSolver> solver;

        std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;

        EventHandle collisionShapeAddedHandle;
        EventHandle collisionShapeRemovedHandle;

        EventHandle rigidBodyAddedHandle;
        EventHandle rigidBodyRemovedHandle;

        std::unordered_set<CollisionManifold, ManifoldHasher> currentCollisions;

        //FUNCTIONS
    public:
        PhysicsLayer();

        PhysicsLayer(PhysicsLayer const &other) = delete;
        PhysicsLayer(PhysicsLayer &&other) noexcept;

        PhysicsLayer &operator=(PhysicsLayer const &other) = delete;
        PhysicsLayer &operator=(PhysicsLayer &&other) noexcept;

        ~PhysicsLayer();

        void registerToEvents(World &world);

        void onUpdate(DeltaTime const deltaTime) override;

        void setGravity(vec3f const &gravity);

        /**
         * @brief Fires a ray into the world.
         * @returns The ID of the first Entity hit.
         */
        Entity rayCast(vec3f const &begin, vec3f const &end);
        /**
         * @brief Fires a ray into the world.
         * @param collisionGroup Bit flag of the collision groups the ray belongs to.
         * @param collisionMask Bit flag of the collision groups the ray collides with.
         * @return The ID of the first Entity hit.
         */
        Entity rayCast(vec3f const &begin, vec3f const &end, uint32_t const collisionGroup, uint32_t const collisionMask);

    private:
        void onCollisionShapeAdded(ComponentAddedEvent<CollisionShapeComponent> const &event);
        void onCollisionShapeRemoved(ComponentRemovedEvent<CollisionShapeComponent> const &event);

        void onRigidBodyAdded(ComponentAddedEvent<RigidBodyComponent> const &event);
        void onRigidBodyRemoved(ComponentRemovedEvent<RigidBodyComponent> const &event);

        void addBodyToWorld(RigidBodyComponent const &rigidBodyComponent);
        void addColliderToWorld(CollisionShapeComponent const &colliderComponent);
    };
}