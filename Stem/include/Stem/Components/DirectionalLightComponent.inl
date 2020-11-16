namespace garlic::inline stem {
    void DirectionalLightComponent::setDirection(clv::mth::vec3f direction) {
        lightData.data.direction = std::move(direction);
    }

    clv::mth::vec3f const &DirectionalLightComponent::getDirection() const {
        return lightData.data.direction;
    }
}