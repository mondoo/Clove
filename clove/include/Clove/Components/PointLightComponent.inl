namespace garlic::clove {
    void PointLightComponent::setAmbientColour(vec3f colour) {
        lightData.data.ambient = std::move(colour);
    }

    vec3f const &PointLightComponent::getAmbientColour() const {
        return lightData.data.ambient;
    }

    void PointLightComponent::setDiffuseColour(vec3f colour) {
        lightData.data.diffuse = std::move(colour);
    }

    vec3f const &PointLightComponent::getDiffuseColour() const {
        return lightData.data.diffuse;
    }

    void PointLightComponent::setSpecularColour(vec3f colour) {
        lightData.data.specular = std::move(colour);
    }

    vec3f const &PointLightComponent::getSpecularColour() const {
        return lightData.data.specular;
    }

    float PointLightComponent::getConstant() const {
        return lightData.data.constant;
    }

    void PointLightComponent::setLinear(float linear) {
        lightData.data.linear = linear;
    }

    float PointLightComponent::getLinear() const {
        return lightData.data.linear;
    }

    void PointLightComponent::setQuadratic(float quadratic) {
        lightData.data.quadratic = quadratic;
    }

    float PointLightComponent::getQuadratic() const {
        return lightData.data.quadratic;
    }
}