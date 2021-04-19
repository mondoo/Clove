#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"

namespace garlic::clove {
    /**
     * @brief Tracks the usage and dependencies of an image in the RenderGraph.
     * @details Can be used to later create a GhaImage when executing the graph.
     */
    using RgImage = ResourceIdType;
}