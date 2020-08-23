#include "Bulb/ECS/Components/RigidBodyComponent.hpp"

#include <btBulletDynamicsCommon.h>

using namespace clv;

namespace blb::ecs {
    RigidBodyComponent::RigidBodyComponent() {
        initialiseRigidBody();
    }

    RigidBodyComponent::RigidBodyComponent(float mass, bool isKinematic)
        : mass(mass)
        , isKinematic(isKinematic) {
        initialiseRigidBody();
    }

    RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent& other)
        : mass(other.mass)
        , isKinematic(other.isKinematic) {
        initialiseRigidBody();
    }

    RigidBodyComponent::RigidBodyComponent(RigidBodyComponent&& other) noexcept = default;

    RigidBodyComponent& RigidBodyComponent::operator=(const RigidBodyComponent& other) {
        mass        = other.mass;
        isKinematic = other.isKinematic;

        initialiseRigidBody();

        return *this;
    }

    RigidBodyComponent& RigidBodyComponent::operator=(RigidBodyComponent&& other) noexcept = default;

    RigidBodyComponent::~RigidBodyComponent() = default;

    void RigidBodyComponent::setLinearVelocity(const clv::mth::vec3f& velocity) {
        const btVector3 btvel{ velocity.x, velocity.y, velocity.z };
        body->setLinearVelocity(btvel);

        body->activate();
    }

    void RigidBodyComponent::applyForce(const clv::mth::vec3f& force, const clv::mth::vec3f& relativeOffset) {
        const btVector3 btForce{ force.x, force.y, force.z };
        const btVector3 btOffset{ relativeOffset.x, relativeOffset.y, relativeOffset.z };
        body->applyForce(btForce, btOffset);

        body->activate();
    }

    void RigidBodyComponent::applyImpulse(const clv::mth::vec3f& impulse, const clv::mth::vec3f& relativeOffset) {
        const btVector3 btImpulse{ impulse.x, impulse.y, impulse.z };
        const btVector3 btOffset{ relativeOffset.x, relativeOffset.y, relativeOffset.z };
        body->applyForce(btImpulse, btOffset);

        body->activate();
    }

    void RigidBodyComponent::setRestitution(float restitution) {
        body->setRestitution(restitution);
    }

    void RigidBodyComponent::setAngularFactor(const clv::mth::vec3f& factor) {
        body->setAngularFactor({ factor.x, factor.y, factor.z });
    }

    clv::mth::vec3f RigidBodyComponent::getLinearVelocity() const {
        const btVector3 btvel = body->getLinearVelocity();
        return { btvel.x(), btvel.y(), btvel.z() };
    }

    float RigidBodyComponent::getRestitution() const {
        return body->getRestitution();
    }

    clv::mth::vec3f RigidBodyComponent::getAngularFactor() const {
        const auto factor = body->getAngularFactor();
        return { factor.x(), factor.y(), factor.z() };
    }

    void RigidBodyComponent::initialiseRigidBody() {
        standInShape = createStandInShape();

        btVector3 localInertia(0, 0, 0);
        btTransform startTransform;
        startTransform.setIdentity();

        if(isKinematic && mass > 0.0f) {
            GARLIC_LOG(garlicLogContext, clv::Log::Level::Debug, "Kinematic RigidBody has non 0 mass. Kinematic takes precedence");
        } else {
            standInShape->calculateLocalInertia(mass, localInertia);
        }

        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, standInShape.get(), localInertia);

        body = std::make_unique<btRigidBody>(rbInfo);
        body->setUserPointer(this);

        int flags = body->getCollisionFlags();
        if(isKinematic) {
            flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
        }
        body->setCollisionFlags(flags);
    }

    std::unique_ptr<btSphereShape> RigidBodyComponent::createStandInShape() {
        return std::make_unique<btSphereShape>(0.1f);
    }
}