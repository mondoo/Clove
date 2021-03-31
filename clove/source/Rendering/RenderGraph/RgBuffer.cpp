#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"

#include <Clove/Graphics/GhaFactory.hpp>

namespace garlic::clove {
    RgBuffer::RgBuffer(size_t bufferSize) {
        ghaDescriptor.size = bufferSize;
    }

    RgBuffer::RgBuffer(RgBuffer const &other) {
        //TODO
    }

    RgBuffer::RgBuffer(RgBuffer &&other) noexcept {
        //TODO: Does usage needed to be tracked here?
    }

    RgBuffer &RgBuffer::operator=(RgBuffer const &other) {
        //TODO
        return *this;
    }

    RgBuffer &RgBuffer::operator=(RgBuffer &&other) noexcept {
        //TODO: Does usage needed to be tracked here?
        return *this;
    }

    RgBuffer::~RgBuffer() = default;

    std::unique_ptr<GhaBuffer> RgBuffer::createGhaBuffer(GhaFactory &factory) const {
        return *factory.createBuffer(ghaDescriptor);
    }
}