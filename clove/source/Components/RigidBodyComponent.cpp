#include "Clove/Components/RigidBodyComponent.hpp"

namespace garlic::clove{
    void RigidBodyComponent::applyForce(vec3f force, vec3f relativeOffset) {
        appliedForce = ForceApplication{ .amount = std::move(force), .offset = std::move(relativeOffset) };
    }

    void RigidBodyComponent::applyImpulse(vec3f impulse, vec3f relativeOffset) {
        appliedImpulse = ForceApplication{ .amount = std::move(impulse), .offset = std::move(relativeOffset) };
    }
}