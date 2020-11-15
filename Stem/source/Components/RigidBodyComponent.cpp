#include "Stem/Components/RigidBodyComponent.hpp"

#include <Root/Log/Log.hpp>
#include <btBulletDynamicsCommon.h>

using namespace clv;

namespace garlic::inline stem {
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

    void RigidBodyComponent::setLinearVelocity(clv::mth::vec3f const &velocity) {
        btVector3 const btvel{ velocity.x, velocity.y, velocity.z };
        body->setLinearVelocity(btvel);

        body->activate();
    }

    void RigidBodyComponent::applyForce(clv::mth::vec3f const &force, clv::mth::vec3f const &relativeOffset) {
        btVector3 const btForce{ force.x, force.y, force.z };
        btVector3 const btOffset{ relativeOffset.x, relativeOffset.y, relativeOffset.z };
        body->applyForce(btForce, btOffset);

        body->activate();
    }

    void RigidBodyComponent::applyImpulse(clv::mth::vec3f const &impulse, clv::mth::vec3f const &relativeOffset) {
        btVector3 const btImpulse{ impulse.x, impulse.y, impulse.z };
        btVector3 const btOffset{ relativeOffset.x, relativeOffset.y, relativeOffset.z };
        body->applyForce(btImpulse, btOffset);

        body->activate();
    }

    void RigidBodyComponent::setRestitution(float restitution) {
        body->setRestitution(restitution);
    }

    void RigidBodyComponent::setAngularFactor(clv::mth::vec3f const &factor) {
        body->setAngularFactor({ factor.x, factor.y, factor.z });
    }

    clv::mth::vec3f RigidBodyComponent::getLinearVelocity() const {
        btVector3 const btvel = body->getLinearVelocity();
        return { btvel.x(), btvel.y(), btvel.z() };
    }

    float RigidBodyComponent::getRestitution() const {
        return body->getRestitution();
    }

    clv::mth::vec3f RigidBodyComponent::getAngularFactor() const {
        auto const factor = body->getAngularFactor();
        return { factor.x(), factor.y(), factor.z() };
    }

    void RigidBodyComponent::initialiseRigidBody() {
        standInShape = createStandInShape();

        btVector3 localInertia(0, 0, 0);
        btTransform startTransform;
        startTransform.setIdentity();

        if(descriptor.isKinematic && descriptor.mass > 0.0f) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Debug, "Kinematic RigidBody has non 0 mass. Kinematic takes precedence");
        } else {
            standInShape->calculateLocalInertia(descriptor.mass, localInertia);
        }

        motionState = std::make_unique<btDefaultMotionState>(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(descriptor.mass, motionState.get(), standInShape.get(), localInertia);

        body = std::make_unique<btRigidBody>(rbInfo);

        int flags = body->getCollisionFlags();
        if(descriptor.isKinematic) {
            flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
        }
        body->setCollisionFlags(flags);
    }

    std::unique_ptr<btSphereShape> RigidBodyComponent::createStandInShape() {
        return std::make_unique<btSphereShape>(0.1f);
    }
}