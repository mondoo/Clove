#include "Clove/Components/RigidBodyComponent.hpp"

#include <Clove/Log/Log.hpp>
#include <btBulletDynamicsCommon.h>

namespace garlic::clove {
    RigidBodyComponent::RigidBodyComponent() {
        initialiseRigidBody();
    }

    RigidBodyComponent::RigidBodyComponent(Descriptor descriptor)
        : descriptor(std::move(descriptor)) {
        initialiseRigidBody();
    }

    RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent &other)
        : descriptor(other.descriptor) {
        initialiseRigidBody();
    }

    RigidBodyComponent::RigidBodyComponent(RigidBodyComponent &&other) noexcept = default;

    RigidBodyComponent &RigidBodyComponent::operator=(const RigidBodyComponent &other) {
        descriptor = other.descriptor;

        initialiseRigidBody();

        return *this;
    }

    RigidBodyComponent &RigidBodyComponent::operator=(RigidBodyComponent &&other) noexcept = default;

    RigidBodyComponent::~RigidBodyComponent() = default;

    void RigidBodyComponent::setLinearVelocity(vec3f const &velocity) {
        body->setLinearVelocity({ velocity.x, velocity.y, velocity.z });

        body->activate();
    }

    void RigidBodyComponent::applyForce(vec3f const &force, vec3f const &relativeOffset) {
        btVector3 const btForce{ force.x, force.y, force.z };
        btVector3 const btOffset{ relativeOffset.x, relativeOffset.y, relativeOffset.z };
        body->applyForce(btForce, btOffset);

        body->activate();
    }

    void RigidBodyComponent::applyImpulse(vec3f const &impulse, vec3f const &relativeOffset) {
        btVector3 const btImpulse{ impulse.x, impulse.y, impulse.z };
        btVector3 const btOffset{ relativeOffset.x, relativeOffset.y, relativeOffset.z };
        body->applyForce(btImpulse, btOffset);

        body->activate();
    }

    void RigidBodyComponent::setRestitution(float restitution) {
        body->setRestitution(restitution);
    }

    void RigidBodyComponent::setAngularFactor(vec3f const &factor) {
        body->setAngularFactor({ factor.x, factor.y, factor.z });
    }

    void RigidBodyComponent::setLinearFactor(vec3f const &factor) {
        body->setLinearFactor({ factor.x, factor.y, factor.z });
    }

    vec3f RigidBodyComponent::getLinearVelocity() const {
        btVector3 const btvel{ body->getLinearVelocity() };
        return { btvel.x(), btvel.y(), btvel.z() };
    }

    float RigidBodyComponent::getRestitution() const {
        return body->getRestitution();
    }

    vec3f RigidBodyComponent::getAngularFactor() const {
        auto const factor{ body->getAngularFactor() };
        return { factor.x(), factor.y(), factor.z() };
    }

    vec3f RigidBodyComponent::getLinearFactor() const {
        auto const factor{ body->getLinearFactor() };
        return { factor.x(), factor.y(), factor.z() };
    }

    void RigidBodyComponent::initialiseRigidBody() {
        standInShape = createStandInShape();

        btVector3 localInertia{ 0, 0, 0 };
        if(descriptor.mass > 0.0f) {
            standInShape->calculateLocalInertia(descriptor.mass, localInertia);
        }

        body = std::make_unique<btRigidBody>(btRigidBody::btRigidBodyConstructionInfo{ descriptor.mass, nullptr, standInShape.get(), localInertia });
    }

    std::unique_ptr<btSphereShape> RigidBodyComponent::createStandInShape() {
        return std::make_unique<btSphereShape>(0.1f);
    }
}