#include "Clove/Graphics/Vulkan/VKQueue.hpp"

#include "Clove/Log.hpp"

#include <Root/Definitions.hpp>

namespace clv::gfx::vk {
    VkCommandPoolCreateFlags convertCommandPoolCreateFlags(QueueFlags garlicFlags) {
        switch(garlicFlags) {
            case QueueFlags::None:
                return 0;
            case QueueFlags::Transient:
                return VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
            case QueueFlags::ReuseBuffers:
                return VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown queue flag", GARLIC_FUNCTION_NAME);
                return 0;
        }
    }
}