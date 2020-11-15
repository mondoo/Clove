#include "Stem/Components/StaticModelComponent.hpp"

#include "Stem/Rendering/Renderables/Mesh.hpp"

namespace garlic::inline stem {
    StaticModelComponent::StaticModelComponent(StaticModel model)
        : model(std::move(model)) {
    }

    StaticModelComponent::StaticModelComponent(StaticModelComponent const &other) = default;

    StaticModelComponent::StaticModelComponent(StaticModelComponent &&other) noexcept
        : model(std::move(other.model)) {
    }

    StaticModelComponent &StaticModelComponent::operator=(StaticModelComponent const &other) = default;

    StaticModelComponent &StaticModelComponent::operator=(StaticModelComponent &&other) noexcept {
        model = other.model;
        return *this;
    }

    StaticModelComponent::~StaticModelComponent() = default;
}