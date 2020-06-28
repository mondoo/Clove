#pragma once

//TODO: Remove
#include <vulkan/vulkan.hpp>

namespace clv::gfx {
    using UsageType = uint8_t;
    enum class BufferUsageMode : UsageType {
        TransferSource      = 1 << 0,
        TransferDestination = 1 << 1,
        VertexBuffer        = 1 << 2,
    };
    // clang-format off
    inline constexpr BufferUsageMode operator&(BufferUsageMode l, BufferUsageMode r) { return static_cast<BufferUsageMode>(static_cast<UsageType>(l) & static_cast<UsageType>(r)); }
    inline constexpr BufferUsageMode operator|(BufferUsageMode l, BufferUsageMode r) { return static_cast<BufferUsageMode>(static_cast<UsageType>(l) | static_cast<UsageType>(r)); }
    inline constexpr BufferUsageMode operator^(BufferUsageMode l, BufferUsageMode r) { return static_cast<BufferUsageMode>(static_cast<UsageType>(l) ^ static_cast<UsageType>(r)); }

    inline constexpr BufferUsageMode operator&=(BufferUsageMode l, BufferUsageMode r) { l = l & r; return l; }
    inline constexpr BufferUsageMode operator|=(BufferUsageMode l, BufferUsageMode r) { l = l | r; return l; }
    inline constexpr BufferUsageMode operator^=(BufferUsageMode l, BufferUsageMode r) { l = l ^ r; return l; }

    inline constexpr bool operator==(BufferUsageMode l, UsageType r){ return static_cast<UsageType>(l) == r; }
    inline constexpr bool operator!=(BufferUsageMode l, UsageType r){ return !(l == r); }
    // clang-format on

    enum class BufferSharingMode {
        Exclusive,
        Concurrent
    };

    struct BufferDescriptor {
        size_t size = 0;
        BufferUsageMode usageFlags = 0;
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