namespace clove {
    GhaImage::Format RgImage::getFormat() const {
        return ghaImageDescriptor.format;
    }

    vec2ui const &RgImage::getDimensions() const {
        return ghaImageDescriptor.dimensions;
    }

    uint32_t RgImage::getArrayTotalCount() const {
        return ghaImageDescriptor.type == GhaImage::Type::Cube ? ghaImageDescriptor.arrayCount * 6 : ghaImageDescriptor.arrayCount;
    }

    GhaImage::UsageMode RgImage::getCurrentUsage() const {
        return ghaImageDescriptor.usageFlags;
    }

    bool RgImage::isExternalImage() const {
        return externalImage;
    }
}