#include "Bulb/ECS/Components/CubeColliderComponent.hpp"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <btBulletDynamicsCommon.h>

namespace blb::ecs {
    CubeColliderComponent::CubeColliderComponent()
        : CubeColliderComponent(clv::mth::vec3f{ 1.0f }) {
    }

    CubeColliderComponent::CubeColliderComponent(clv::mth::vec3f cubeSize)
        : cubeSize(std::move(cubeSize)) {
        initialiseCubeCollider();
    }

    CubeColliderComponent::CubeColliderComponent(const CubeColliderComponent& other)
        : cubeSize(other.cubeSize) {
        initialiseCubeCollider();
    }

    CubeColliderComponent::CubeColliderComponent(CubeColliderComponent&& other) noexcept = default;

    CubeColliderComponent& CubeColliderComponent::operator=(const CubeColliderComponent& other) {
        cubeSize = other.cubeSize;

        initialiseCubeCollider();

        return *this;
    }

    CubeColliderComponent& CubeColliderComponent::operator=(CubeColliderComponent&& other) noexcept = default;

    CubeColliderComponent::~CubeColliderComponent() = default;

    void CubeColliderComponent::setCubeSize(clv::mth::vec3f cubeSize) {
        collisionShape->setLocalScaling(btVector3{ cubeSize.x, cubeSize.y, cubeSize.z });
        this->cubeSize = std::move(cubeSize);
    }

    const clv::mth::vec3f& CubeColliderComponent::getCubeSize() const {
        return cubeSize;
    }

    void CubeColliderComponent::initialiseCubeCollider() {
        collisionShape = std::make_unique<btBoxShape>(btVector3{ 1.0f, 1.0f, 1.0f });
        collisionShape->setLocalScaling(btVector3{ cubeSize.x, cubeSize.y, cubeSize.z });

        constructCollisionObject();
    }

    void CubeColliderComponent::constructCollisionObject() {
        collisionObject = std::make_unique<btGhostObject>();
        collisionObject->setCollisionShape(collisionShape.get());
        collisionObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }
}