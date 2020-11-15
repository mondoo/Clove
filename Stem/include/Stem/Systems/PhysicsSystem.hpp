#pragma once

#include <Bulb/ECS/ECSEvents.hpp>
#include <Bulb/ECS/Entity.hpp>
#include <Bulb/ECS/System.hpp>
#include <Root/Event/EventHandle.hpp>
#include <unordered_set>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace garlic::inline stem {
    class CubeColliderComponent;
    class RigidBodyComponent;
}

namespace garlic::inline stem {
    class PhysicsSystem : public blb::ecs::System {
        //TYPES
    private:
        struct CollisionManifold {
            blb::ecs::EntityID entityA;
            blb::ecs::EntityID entityB;

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
                size_t a = std::hash<blb::ecs::EntityID>{}(manifold.entityA);
                size_t b = std::hash<blb::ecs::EntityID>{}(manifold.entityB);
                return a ^ (b << 1);
            }
        };

        //VARIABLES
    private:
        std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
        std::unique_ptr<btCollisionDispatcher> dispatcher;
        std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
        std::unique_ptr<btSequentialImpulseConstraintSolver> solver;

        std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;

        garlic::EventHandle cubeColliderAddedHandle;
        garlic::EventHandle cubeColliderRemovedHandle;

        garlic::EventHandle rigidBodyAddedHandle;
        garlic::EventHandle rigidBodyRemovedHandle;

        std::unordered_set<CollisionManifold, ManifoldHasher> currentCollisions;

        //FUNCTIONS
    public:
        PhysicsSystem();

        PhysicsSystem(PhysicsSystem const &other) = delete;
        PhysicsSystem(PhysicsSystem &&other) noexcept;

        PhysicsSystem &operator=(PhysicsSystem const &other) = delete;
        PhysicsSystem &operator=(PhysicsSystem &&other) noexcept;

        ~PhysicsSystem();

        void registerToEvents(garlic::EventDispatcher &dispatcher) override;

        void preUpdate(blb::ecs::World &world) override;
        void update(blb::ecs::World &world, clv::utl::DeltaTime deltaTime) override;
        void postUpdate(blb::ecs::World &world) override;

        /**
         * @brief Fires a ray into the world.
         * @returns The ID of the first Entity hit.
         */
        blb::ecs::EntityID rayCast(clv::mth::vec3f const &begin, clv::mth::vec3f const &end);
        /**
         * @brief Fires a ray into the world.
         * @param collisionGroup Bit flag of the collision groups the ray belongs to.
         * @param collisionMask Bit flag of the collision groups the ray collides with.
         * @return The ID of the first Entity hit.
         */
        blb::ecs::EntityID rayCast(clv::mth::vec3f const &begin, clv::mth::vec3f const &end, uint32_t const collisionGroup, uint32_t const collisionMask);

    private:
        void onCubeColliderAdded(blb::ecs::ComponentAddedEvent<CubeColliderComponent> const &event);
        void onCubeColliderRemoved(blb::ecs::ComponentRemovedEvent<CubeColliderComponent> const &event);

        void onRigidBodyAdded(blb::ecs::ComponentAddedEvent<RigidBodyComponent> const &event);
        void onRigidBodyRemoved(blb::ecs::ComponentRemovedEvent<RigidBodyComponent> const &event);

        void addBodyToWorld(RigidBodyComponent const &rigidBodyComponent);
        void addColliderToWorld(CubeColliderComponent const &colliderComponent);
    };
}