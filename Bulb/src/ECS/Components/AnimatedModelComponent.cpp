#include "Bulb/ECS/Components/AnimatedModelComponent.hpp"

namespace blb::ecs {
    AnimedModelComponent::AnimedModelComponent(rnd::AnimatedModel model) 
        : model(std::move(model)){
    }

    AnimedModelComponent::AnimedModelComponent(const AnimedModelComponent& other) = default;

    AnimedModelComponent::AnimedModelComponent(AnimedModelComponent&& other) noexcept = default;

    AnimedModelComponent& AnimedModelComponent::operator=(const AnimedModelComponent& other) = default;

    AnimedModelComponent& AnimedModelComponent::operator=(AnimedModelComponent&& other) noexcept = default;

    AnimedModelComponent::~AnimedModelComponent() = default;

    rnd::AnimatedModel& AnimedModelComponent::getModel() {
        return model;
    }
}