namespace garlic::clove {
    void DirectionalLightComponent::setDirection(vec3f direction) {
        lightData.data.direction = std::move(direction);
    }

    vec3f const &DirectionalLightComponent::getDirection() const {
        return lightData.data.direction;
    }
}