namespace garlic::clove {
    void Material::setColour(vec4f colour) {
        this->colour = std::move(colour);
    }

    void Material::setShininess(float shininess) {
        this->shininess = shininess;
    }
}