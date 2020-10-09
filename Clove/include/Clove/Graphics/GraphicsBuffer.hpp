#pragma once

#include "Clove/Graphics/GraphicsResource.hpp"

#include <Root/Definitions.hpp>

namespace clv::gfx {
    /**
     * @brief Represents an arbitrary section of memory. Used in graphics operations.
     */
    class GraphicsBuffer {
        //TYPES
    public:
        using UsageModeType = uint8_t;
        enum class UsageMode : UsageModeType {
            TransferSource      = 1 << 0,
            TransferDestination = 1 << 1,
            VertexBuffer        = 1 << 2,
            IndexBuffer         = 1 << 3,
            UniformBuffer       = 1 << 4,
        };

        struct Descriptor {
            size_t size{ 0 };
            UsageMode usageFlags;
            SharingMode sharingMode;
            MemoryType memoryType;
        };

        //FUNCTIONS
    public:
        virtual ~GraphicsBuffer() = default;

        virtual void map(const void* data, const size_t offset, const size_t size) = 0;
    };

    GARLIC_ENUM_BIT_FLAG_OPERATORS(GraphicsBuffer::UsageMode, GraphicsBuffer::UsageModeType)
}