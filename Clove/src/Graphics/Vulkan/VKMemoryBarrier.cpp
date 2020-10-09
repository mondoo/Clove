#include "Clove/Graphics/Vulkan/VKMemoryBarrier.hpp"

#include "Clove/Log.hpp"

#include <Root/Definitions.hpp>
#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    uint32_t getQueueFamilyIndex(QueueType type, const QueueFamilyIndices& indices) {
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
