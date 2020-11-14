namespace blb::rnd{
    void Material::setColour(clv::mth::vec4f colour) {
        this->colour = std::move(colour);
    }

    void Material::setShininess(float shininess) {
        this->shininess = shininess;
    }
}