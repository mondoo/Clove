#pragma once

#include "Clove/Graphics/PipelineObject.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    VkPipelineStageFlags convertStage(PipelineStage stage);
}