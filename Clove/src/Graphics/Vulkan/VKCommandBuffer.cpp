#include "Clove/Graphics/Vulkan/VKCommandBuffer.hpp"

#include "Clove/Log.hpp"

#include <Root/Definitions.hpp>

namespace clv::gfx::vk {
    VkCommandBufferUsageFlags getCommandBufferUsageFlags(CommandBufferUsage garlicUsage) {
        switch(garlicUsage) {
            case clv::gfx::CommandBufferUsage::Default:
                return 0;
            case clv::gfx::CommandBufferUsage::OneTimeSubmit:
                return VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown usage type", GARLIC_FUNCTION_NAME);
                return 0;
        }
    }
}