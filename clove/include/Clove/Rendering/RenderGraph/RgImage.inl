namespace clove {
    GhaImage::Format RgImageView::getFormat() const {
        return image->ghaImageDescriptor.format;
    }

    vec2ui const &RgImageView::getDimensions() const {
        return image->ghaImageDescriptor.dimensions;
    }

    bool RgImageView::isExternalImage() const {
        return image->externalImage;
    }

    uint32_t RgImageView::getArrayIndex() const {
        return arrayIndex;
    }

    uint32_t RgImageView::getArrayCount() const {
        return arrayCount;
    }
}