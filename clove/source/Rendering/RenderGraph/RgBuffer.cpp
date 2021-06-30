#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"

#include <Clove/Graphics/GhaFactory.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    RgBuffer::RgBuffer(ResourceIdType id, size_t size)
        : RgResource{ id }
        , size{ size } {
        ghaBufferDescriptor = GhaBuffer::Descriptor{
            .size        = size,
            .usageFlags  = static_cast<GhaBuffer::UsageMode>(0),//Will be built when executing the graph
            .sharingMode = SharingMode::Exclusive,              //Assume exclusive to begin with
            .memoryType  = MemoryType::VideoMemory,             //Assume video memory until it has been written to from the host
        };
    }

    RgBuffer::RgBuffer(ResourceIdType id, std::shared_ptr<GhaBuffer> ghaBuffer, size_t offset, size_t size)
        : RgResource{ id }
        , ghaBuffer{ std::move(ghaBuffer) }
        , offset{ offset }
        , size{ size } {
        externalBuffer = true;
    }

    RgBuffer::RgBuffer(RgBuffer &&other) noexcept = default;

    RgBuffer &RgBuffer::operator=(RgBuffer &&other) noexcept = default;

    RgBuffer::~RgBuffer() = default;

    std::shared_ptr<GhaBuffer> RgBuffer::getGhaBuffer(GhaFactory &ghaFactory) {
        if(ghaBuffer == nullptr) {
            CLOVE_ASSERT(!externalBuffer, "RgBuffer is registered as an external buffer but does not have a valid GhaBuffer.");
            ghaBuffer = ghaFactory.createBuffer(ghaBufferDescriptor).getValue();
        }

        return ghaBuffer;
    }

    void RgBuffer::addBufferUsage(GhaBuffer::UsageMode usage) {
        CLOVE_ASSERT(!externalBuffer, "Cannot change usage mode. RgBuffer is registered as an external buffer.");
        ghaBufferDescriptor.usageFlags |= usage;
    }
}