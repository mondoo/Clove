#pragma once

#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

//TODO: Remove
#include <vulkan/vulkan.hpp>

namespace clv::gfx {
    using UsageType = uint8_t;
    enum class BufferUsageMode : UsageType {
        TransferSource      = 1 << 0,
        TransferDestination = 1 << 1,
        VertexBuffer        = 1 << 2,
        IndexBuffer         = 1 << 3,
        UniformBuffer       = 1 << 4,
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

    enum class BufferMemoryProperties {
        DeviceLocal, //GPU optimised, can't be written to be CPU
        HostVisible, //Can be written to by CPU, not GPU optimised
    };

    struct BufferDescriptor2 {//TODO: Remove 2, this is because it is conflicting the previously defined type
        size_t size = 0;
        BufferUsageMode usageFlags;
        BufferSharingMode sharingMode;
        BufferMemoryProperties memoryProperties;
    };
}

namespace clv::gfx::vk {
    class VKBuffer {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory bufferMemory;

        BufferDescriptor2 descriptor;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKBuffer() = delete;
        VKBuffer(VkDevice device, VkPhysicalDevice physicalDevice, BufferDescriptor2 descriptor, const QueueFamilyIndices& familyIndices);

        ~VKBuffer();

        void map(const void* data, const size_t size);

        VkBuffer getBuffer() const;
    };
}