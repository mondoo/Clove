#include "Clove/Components/DirectionalLightComponent.hpp"

namespace garlic::clove {
    DirectionalLightComponent::DirectionalLightComponent() {
        lightData = DirectionalLight{
            .data = {
                .direction = { 0.0f, 0.0f, 0.0f },
                .ambient   = { 0.01f, 0.01f, 0.01f },
                .diffuse   = { 0.75f, 0.75f, 0.75f },
                .specular  = { 1.0f, 1.0f, 1.0f },
            },
        };
    }

    DirectionalLightComponent::DirectionalLightComponent(DirectionalLightComponent const &other) = default;

    DirectionalLightComponent::DirectionalLightComponent(DirectionalLightComponent &&other) noexcept = default;

    DirectionalLightComponent &DirectionalLightComponent::operator=(DirectionalLightComponent const &other) = default;

    DirectionalLightComponent &DirectionalLightComponent::operator=(DirectionalLightComponent &&) noexcept = default;

    DirectionalLightComponent::~DirectionalLightComponent() = default;
}