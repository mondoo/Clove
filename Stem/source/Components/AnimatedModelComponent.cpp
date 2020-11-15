#include "Stem/Components/AnimatedModelComponent.hpp"

namespace garlic::inline stem {
    AnimedModelComponent::AnimedModelComponent(blb::rnd::AnimatedModel model)
        : model(std::move(model)) {
    }

    AnimedModelComponent::AnimedModelComponent(AnimedModelComponent const &other) = default;

    AnimedModelComponent::AnimedModelComponent(AnimedModelComponent &&other) noexcept = default;

    AnimedModelComponent &AnimedModelComponent::operator=(AnimedModelComponent const &other) = default;

    AnimedModelComponent &AnimedModelComponent::operator=(AnimedModelComponent &&other) noexcept = default;

    AnimedModelComponent::~AnimedModelComponent() = default;
}