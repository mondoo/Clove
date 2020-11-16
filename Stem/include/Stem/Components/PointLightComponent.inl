namespace garlic::inline stem {
    void PointLightComponent::setAmbientColour(clv::mth::vec3f colour) {
        lightData.data.ambient = std::move(colour);
    }

    clv::mth::vec3f const &PointLightComponent::getAmbientColour() const {
        return lightData.data.ambient;
    }

    void PointLightComponent::setDiffuseColour(clv::mth::vec3f colour) {
        lightData.data.diffuse = std::move(colour);
    }

    clv::mth::vec3f const &PointLightComponent::getDiffuseColour() const {
        return lightData.data.diffuse;
    }

    void PointLightComponent::setSpecularColour(clv::mth::vec3f colour) {
        lightData.data.specular = std::move(colour);
    }

    clv::mth::vec3f const &PointLightComponent::getSpecularColour() const {
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