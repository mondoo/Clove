#include "Clove/Graphics/Vulkan/VKMemoryBarrier.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <vulkan/vulkan.h>

namespace garlic::clove {
    uint32_t getQueueFamilyIndex(QueueType type, QueueFamilyIndices const &indices) {
        switch(type) {
            case QueueType::None:
                return VK_QUEUE_FAMILY_IGNORED;
            case QueueType::Graphics:
                return *indices.graphicsFamily;
            case QueueType::Transfer:
                return *indices.transferFamily;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown queue type", GARLIC_FUNCTION_NAME);
                return VK_QUEUE_FAMILY_IGNORED;
        }
    }
}
