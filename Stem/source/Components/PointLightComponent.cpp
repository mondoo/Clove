#include "Stem/Components/PointLightComponent.hpp"

using namespace clv;

namespace garlic::inline stem {
    PointLightComponent::PointLightComponent() {
        lightData = PointLight{
            .data = {
                .constant  = 1.0f,
                .ambient   = { 0.01f, 0.01f, 0.01f },
                .linear    = 0.0014f,
                .diffuse   = { 0.75f, 0.75f, 0.75f },
                .quadratic = 0.000007f,
                .specular  = { 1.0f, 1.0f, 1.0f },
                .farPlane  = farDist,
            },
        };
    }

    PointLightComponent::PointLightComponent(const PointLightComponent &other) = default;

    PointLightComponent::PointLightComponent(PointLightComponent &&other) noexcept = default;

    PointLightComponent &PointLightComponent::operator=(const PointLightComponent &other) = default;

    PointLightComponent &PointLightComponent::operator=(PointLightComponent &&) noexcept = default;

    PointLightComponent::~PointLightComponent() = default;
}