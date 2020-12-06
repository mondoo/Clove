#include "Clove/Components/CollisionShapeComponent.hpp"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <btBulletDynamicsCommon.h>

template<typename... Ts>
struct match : Ts... { using Ts::operator()...; };
template<typename... Ts>
match(Ts...) -> match<Ts...>;

namespace garlic::clove {
    CollisionShapeComponent::CollisionShapeComponent(ShapeVariant shape)
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

    void CollisionShapeComponent::setShape(ShapeVariant shape) {
        shape = std::move(shape);
        initialiseCollisionShape();
    }

    ShapeVariant const &CollisionShapeComponent::getShape() const {
        return shape;
    }

    void CollisionShapeComponent::initialiseCollisionShape() {
        std::visit(match{
           [&](SphereShape sphere) { collisionShape = std::make_unique<btSphereShape>(sphere.radius); },
           [&](CubeShape cube) { collisionShape = std::make_unique<btBoxShape>(btVector3{ cube.halfExtents.x, cube.halfExtents.y, cube.halfExtents.z }); }
        }, shape);

        constructCollisionObject();
    }

    void CollisionShapeComponent::constructCollisionObject() {
        collisionObject = std::make_unique<btGhostObject>();
        collisionObject->setCollisionShape(collisionShape.get());
        collisionObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }
}