#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"

#include <Clove/Graphics/GhaFactory.hpp>

namespace garlic::clove {
    RgBuffer::RgBuffer() = default;

    RgBuffer::RgBuffer(size_t bufferSize) {
        ghaDescriptor.size = bufferSize;
    }

    RgBuffer::RgBuffer(std::shared_ptr<GhaBuffer> buffer, size_t offset, size_t size) {
        //TODO: Keep track of offset and size used for binding later.
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

    void RgBuffer::write(void const *data, size_t const offset, size_t const size) {
        //TODO
        //Block writes to buffers created directly from GHA?
    }

    std::unique_ptr<GhaBuffer> RgBuffer::createGhaBuffer(GhaFactory &factory) const {
        return *factory.createBuffer(ghaDescriptor);
    }
}