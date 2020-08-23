#include "Bulb/ECS/Components/CubeColliderComponent.hpp"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace blb::ecs {
    CubeColliderComponent::CubeColliderComponent(const clv::mth::vec3f& cubeSize) {
        collisionShape = std::make_unique<btBoxShape>(btVector3{ cubeSize.x, cubeSize.y, cubeSize.z });
        collisionObject = std::make_shared<btGhostObject>();

        collisionObject->setCollisionShape(collisionShape.get());
    }
}