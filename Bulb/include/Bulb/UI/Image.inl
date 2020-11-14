namespace blb::ui {
    void Image::setPosition(clv::mth::vec2f position) {
        this->position = std::move(position);
    }

    void Image::setRotation(float rotation) {
        this->rotation = rotation;
    }

    void Image::setSize(clv::mth::vec2f size) {
        this->size = std::move(size);
    }

    clv::mth::vec2f const &Image::getPosition() const {
        return position;
    }

    float Image::getRotation() const {
        return rotation;
    }

    clv::mth::vec2f const &Image::getSize() const {
        return size;
    }
}