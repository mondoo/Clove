#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"

namespace garlic::clove {
    /**
     * @brief Tracks the usage and dependencies of a buffer in the RenderGraph.
     * @details Can be used to later create a GhaBuffer when executing the graph
     * or track an exisiting GhaBuffer.
     */
    using RgBuffer = ResourceIdType;
}