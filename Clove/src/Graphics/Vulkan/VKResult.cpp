#include "Clove/Graphics/Vulkan/VKResult.hpp"

#include <Root/Definitions.hpp>
#include <Root/Log/Log.hpp>

namespace clv::gfx::vk {
    Result convertResult(VkResult result) {
        switch(result) {
            case VK_SUCCESS:
                return Result::Success;
            case VK_SUBOPTIMAL_KHR:
                return Result::Success_SwapchainSuboptimal;
            case VK_ERROR_OUT_OF_DATE_KHR:
                return Result::Error_SwapchainOutOfDate;
            default:
                GARLIC_ASSERT(false, "Unkown result type");
                return Result::Unkown;
        }
    }
}