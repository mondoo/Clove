#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"

namespace garlic::clove {
    RgBuffer::RgBuffer() = default;

    RgBuffer::RgBuffer(RgBuffer const &other) = default;

    RgBuffer::RgBuffer(RgBuffer &&other) noexcept = default;

    RgBuffer &RgBuffer::operator=(RgBuffer const &other) = default;

    RgBuffer &RgBuffer::operator=(RgBuffer &&other) noexcept = default;

    RgBuffer::~RgBuffer() = default;

    RgBuffer::RgBuffer(ResourceIdType id)
        : id{ id } {
    }

    RgBuffer::RgBuffer(ResourceIdType id, size_t offset, size_t size)
        : id{ id }
        , offset{ offset }
        , size{ size } {
    }
}