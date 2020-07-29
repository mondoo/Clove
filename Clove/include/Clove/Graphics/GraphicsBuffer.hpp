#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include <Root/Definitions.hpp>

namespace clv::gfx {
    class GraphicsBuffer {
        //TYPES
    public:
        using UsageType = uint8_t;
        enum class UsageMode : UsageType {
            TransferSource      = 1 << 0,
            TransferDestination = 1 << 1,
            VertexBuffer        = 1 << 2,
            IndexBuffer         = 1 << 3,
            UniformBuffer       = 1 << 4,
        };

        struct Descriptor {
            size_t size{ 0 };
            UsageMode usageFlags{ 0 };
            SharingMode sharingMode{ 0 };
            MemoryType memoryType{ 0 };
        };

        //FUNCTIONS
    public:
        virtual ~GraphicsBuffer() = default;

        virtual void map(const void* data, const size_t size) = 0;
    };

    GARLIC_ENUM_BIT_FLAG_OPERATORS(GraphicsBuffer::UsageMode, GraphicsBuffer::UsageType)
}