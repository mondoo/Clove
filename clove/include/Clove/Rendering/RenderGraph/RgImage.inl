namespace clove {
    GhaImage::Format RgImage::getFormat() const {
        return ghaImageDescriptor.format;
    }

    vec2ui const &RgImage::getDimensions() const {
        return ghaImageDescriptor.dimensions;
    }

    GhaImage::Layout RgImage::getInitialLayout() const {
        return ghaImageDescriptor.initialLayout;
    }

    bool RgImage::isExternalImage() const {
        return externalImage;
    }
}