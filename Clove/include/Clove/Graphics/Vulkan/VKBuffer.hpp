#pragma once

//TODO: Remove
#include <vulkan/vulkan.hpp>

namespace clv::gfx {
    enum class BufferSharingMode{
        Exclusive,
        Concurrent
    };

    using UsageType = uint8_t;
    enum class BufferUsageMode : UsageType {
        TransferSource      = 1 << 0,
        TransferDestination = 1 << 1,
        VertexBuffer        = 1 << 2,
    };

    struct BufferDescriptor {
        size_t size = 0;
        UsageType usageFlags = 0;
        BufferSharingMode sharingMode;
    };
}

namespace clv::gfx::vk {
    class VKBuffer {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;
        VkBuffer buffer = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKBuffer() = delete;
        VKBuffer(VkDevice device, BufferDescriptor descriptor);

        ~VKBuffer();

        VkBuffer getBuffer() const;
    };
}