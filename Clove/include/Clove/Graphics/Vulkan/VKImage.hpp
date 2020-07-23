#pragma once

#include <Clove/Graphics/Vulkan/VulkanTypes.hpp>

//TODO: Remove
#include <vulkan/vulkan.h>
#include <Root/Definitions.hpp>

namespace clv::gfx {
    using UsageType = uint8_t;
    enum class ImageUsageMode : UsageType {
        TransferDestination    = 1 << 0,//To be used as a destination from a transfer buffer
        Sampled                = 1 << 1,//To be used in an image view that's sampled in a shader
        ColourAttachment       = 1 << 2,//To be used in an image view for a frame buffer
        DepthStencilAttachment = 1 << 3,//To be used in an image view for a depth / stencil attachment
    };
    GARLIC_ENUM_BIT_FLAG_OPERATORS(ImageUsageMode, UsageType)

    enum class ImageType {
        _2D,
        _3D
    };

    struct ImageDescriptor {
        ImageType type;
        ImageUsageMode usageFlags;
        clv::mth::vec2ui dimensions;
        ImageFormat format;
        SharingMode sharingMode;
        MemoryProperties memoryProperties;
    };
}

namespace clv::gfx::vk {
    class VKImage {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;
        VkImage image   = VK_NULL_HANDLE;
        VkDeviceMemory imageMemory;

        ImageDescriptor descriptor;

        //FUNCTIONS
    public:
        //TODO: other ctors
        VKImage(VkDevice device, VkPhysicalDevice physicalDevice, ImageDescriptor descriptor, const QueueFamilyIndices& familyIndices);
        ~VKImage();

        VkImage getImage() const;
    };
}