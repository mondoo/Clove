#pragma once

#include "Clove/Graphics/Framebuffer.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKFramebuffer : public Framebuffer {
        //VARIABLES
    private:
        DevicePointer device;

        VkFramebuffer framebuffer{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKFramebuffer() = delete;
        VKFramebuffer(DevicePointer device, Descriptor descriptor);

        VKFramebuffer(const VKFramebuffer& other) = delete;
        VKFramebuffer(VKFramebuffer&& other) noexcept;

        VKFramebuffer& operator=(const VKFramebuffer& other) = delete;
        VKFramebuffer& operator=(VKFramebuffer&& other) noexcept;

        ~VKFramebuffer();

        VkFramebuffer getFrameBuffer() const;
    };
}