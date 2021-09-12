#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"

#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"

#include <Clove/Log/Log.hpp>

namespace clove {
    RgBuffer::RgBuffer(size_t size)
        : size{ size } {
        ghaBufferDescriptor = GhaBuffer::Descriptor{
            .size        = size,
            .usageFlags  = static_cast<GhaBuffer::UsageMode>(0),//Will be built when executing the graph
            .sharingMode = SharingMode::Exclusive,              //Assume exclusive to begin with
            .memoryType  = MemoryType::VideoMemory,             //Assume video memory until it has been written to from the host
        };
    }

    RgBuffer::RgBuffer(GhaBuffer *ghaBuffer, size_t offset, size_t size)
        : ghaBuffer{ ghaBuffer }
        , offset{ offset }
        , size{ size } {
        externalBuffer = true;
    }

    RgBuffer::RgBuffer(RgBuffer &&other) noexcept = default;

    RgBuffer &RgBuffer::operator=(RgBuffer &&other) noexcept = default;

    RgBuffer::~RgBuffer() = default;

    GhaBuffer *RgBuffer::getGhaBuffer(RgFrameCache &cache) {
        if(ghaBuffer == nullptr) {
            CLOVE_ASSERT_MSG(!externalBuffer, "RgBuffer is registered as an external buffer but does not have a valid GhaBuffer.");
            ghaBuffer = cache.allocateBuffer(ghaBufferDescriptor);
        }

        return ghaBuffer;
    }

    void RgBuffer::addBufferUsage(GhaBuffer::UsageMode usage) {
        CLOVE_ASSERT_MSG(!externalBuffer, "Cannot change usage mode. RgBuffer is registered as an external buffer.");
        ghaBufferDescriptor.usageFlags |= usage;
    }

    void RgBuffer::makeCpuAccessable() {
        CLOVE_ASSERT_MSG(!externalBuffer, "Cannot change memory type. RgBuffer is registered as an external buffer.");
        ghaBufferDescriptor.memoryType = MemoryType::SystemMemory;
    }

    void RgBuffer::setSharingMode(SharingMode mode) {
        CLOVE_ASSERT_MSG(!externalBuffer, "Cannot change sharing mode. RgBuffer is registered as an external buffer.");
        ghaBufferDescriptor.sharingMode = mode;
    }
}