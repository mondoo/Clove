namespace clove {
    GhaImage::Format RgImage::getFormat() const {
        return ghaImageDescriptor.format;
    }

    vec2ui const &RgImage::getDimensions() const {
        return ghaImageDescriptor.dimensions;
    }

    bool RgImage::isExternalImage() const {
        return externalImage;
    }
}