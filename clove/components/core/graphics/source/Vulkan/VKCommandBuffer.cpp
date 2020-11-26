#include "Clove/Graphics/Vulkan/VKCommandBuffer.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    VkCommandBufferUsageFlags getCommandBufferUsageFlags(CommandBufferUsage garlicUsage) {
        switch(garlicUsage) {
            case garlic::clove::CommandBufferUsage::Default:
                return 0;
            case garlic::clove::CommandBufferUsage::OneTimeSubmit:
                return VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            default:
                CLOVE_ASSERT(false, "{0}: Unkown usage type", CLOVE_FUNCTION_NAME);
                return 0;
        }
    }
}