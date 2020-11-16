#include "Stem/Components/AnimatedModelComponent.hpp"

namespace garlic::inline stem {
    AnimatedModelComponent::AnimatedModelComponent(AnimatedModel model)
        : model(std::move(model)) {
    }

    AnimatedModelComponent::AnimatedModelComponent(AnimatedModelComponent const &other) = default;

    AnimatedModelComponent::AnimatedModelComponent(AnimatedModelComponent &&other) noexcept = default;

    AnimatedModelComponent &AnimatedModelComponent::operator=(AnimatedModelComponent const &other) = default;

    AnimatedModelComponent &AnimatedModelComponent::operator=(AnimatedModelComponent &&other) noexcept = default;

    AnimatedModelComponent::~AnimatedModelComponent() = default;
}