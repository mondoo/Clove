#pragma once

#include "Bulb/ECS/ECSEvents.hpp"
#include "Bulb/ECS/Entity.hpp"
#include "Bulb/ECS/System.hpp"

#include <Clove/Event/EventHandle.hpp>

#include <unordered_set>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace blb::ecs {
    class CubeColliderComponent;
    class RigidBodyComponent;
}

namespace blb::ecs {
    class PhysicsSystem : public System {
        //TYPES
    private:
        struct CollisionManifold {
            EntityID entityA;
            EntityID entityB;

            constexpr friend bool operator==(const CollisionManifold& lhs, const CollisionManifold& rhs) {
                return  (lhs.entityA == rhs.entityA && lhs.entityB == rhs.entityB) ||
                        (lhs.entityA == rhs.entityB && lhs.entityB == rhs.entityA);
            }
            constexpr friend bool operator!=(const CollisionManifold& lhs, const CollisionManifold& rhs) {
                return !(lhs == rhs);
            }
        };

        struct ManifoldHasher {
            size_t operator()(const CollisionManifold& manifold) const {
                size_t a = std::hash<EntityID>{}(manifold.entityA);
                size_t b = std::hash<EntityID>{}(manifold.entityB);
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

        clv::EventHandle cubeColliderAddedHandle;
        clv::EventHandle cubeColliderRemovedHandle;

        clv::EventHandle rigidBodyAddedHandle;
        clv::EventHandle rigidBodyRemovedHandle;

        std::unordered_set<CollisionManifold, ManifoldHasher> currentCollisions;

        //FUNCTIONS
    public:
        PhysicsSystem();

        PhysicsSystem(const PhysicsSystem& other) = delete;
        PhysicsSystem(PhysicsSystem&& other) noexcept;

        PhysicsSystem& operator=(const PhysicsSystem& other) = delete;
        PhysicsSystem& operator=(PhysicsSystem&& other) noexcept;

        ~PhysicsSystem();

        void registerToEvents(clv::EventDispatcher& dispatcher) override;

        void preUpdate(World& world) override;
        void update(World& world, clv::utl::DeltaTime deltaTime) override;
        void postUpdate(World& world) override;

        /**
         * @brief Fires a ray into the world.
         * @returns The ID of the first Entity hit.
         */
        EntityID rayCast(const clv::mth::vec3f& begin, const clv::mth::vec3f& end);

    private:
        void onCubeColliderAdded(const ComponentAddedEvent<CubeColliderComponent>& event);
        void onCubeColliderRemoved(const ComponentRemovedEvent<CubeColliderComponent>& event);

        void onRigidBodyAdded(const ComponentAddedEvent<RigidBodyComponent>& event);
        void onRigidBodyRemoved(const ComponentRemovedEvent<RigidBodyComponent>& event);

        void addBodyToWorld(const RigidBodyComponent& rigidBodyComponent);
        void addColliderToWorld(const CubeColliderComponent& colliderComponent);
    };
}