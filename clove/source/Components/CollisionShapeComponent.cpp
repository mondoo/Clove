#include "Clove/Components/CollisionShapeComponent.hpp"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <btBulletDynamicsCommon.h>

namespace garlic::clove {
    CollisionShapeComponent::CollisionShapeComponent(CollisionShape shape)
        : shape(std::move(shape)) {
        initialiseCollisionShape();
    }

    CollisionShapeComponent::CollisionShapeComponent(CollisionShapeComponent const &other)
        : shape(other.shape) {
        initialiseCollisionShape();
    }

    CollisionShapeComponent::CollisionShapeComponent(CollisionShapeComponent &&other) noexcept = default;

    CollisionShapeComponent &CollisionShapeComponent::operator=(CollisionShapeComponent const &other) {
        shape = other.shape;

        initialiseCollisionShape();

        return *this;
    }

    CollisionShapeComponent &CollisionShapeComponent::operator=(CollisionShapeComponent &&other) noexcept = default;

    CollisionShapeComponent::~CollisionShapeComponent() = default;

    void CollisionShapeComponent::setShape(CollisionShape shape) {
        shape = std::move(shape);
        initialiseCollisionShape();
    }

    CollisionShape const &CollisionShapeComponent::getShape() const {
        return shape;
    }

    void CollisionShapeComponent::initialiseCollisionShape() {
        switch(shape.type) {
            case ShapeType::Sphere: {
                auto const sphere{ std::get<SphereShape>(shape.shape) };
                collisionShape = std::make_unique<btSphereShape>(sphere.radius);
            } break;
            case ShapeType::Cube: {
                auto const cube{ std::get<CubeShape>(shape.shape) };
                collisionShape = std::make_unique<btBoxShape>(btVector3{ cube.halfExtents.x, cube.halfExtents.y, cube.halfExtents.z });
            } break;
            default:
                break;
        }
        constructCollisionObject();
    }

    void CollisionShapeComponent::constructCollisionObject() {
        collisionObject = std::make_unique<btGhostObject>();
        collisionObject->setCollisionShape(collisionShape.get());
        collisionObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }
}