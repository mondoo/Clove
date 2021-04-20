#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"

namespace garlic::clove {
    /**
     * @brief Tracks the usage and dependencies of a buffer in the RenderGraph.
     * @details Can be used to later create a GhaBuffer when executing the graph
     * or track an exisiting GhaBuffer.
     */
    class RgBuffer {
        friend class RenderGraph;

        //VARIABLES
    private:
        ResourceIdType id{ INVALID_ID };

        size_t offset{ 0 }; /**< Offset into the GhaBuffer this buffer represents. */
        size_t size{ 0 };   /**< Size of the region in the GhaBuffer this buffer represents. */

        //FUNCTIONS
    public:
        RgBuffer();

        RgBuffer(RgBuffer const &other);
        RgBuffer(RgBuffer &&other) noexcept;

        RgBuffer &operator=(RgBuffer const &other);
        RgBuffer &operator=(RgBuffer &&other) noexcept;

        ~RgBuffer();

    private:
        RgBuffer(ResourceIdType id);
        RgBuffer(ResourceIdType id, size_t offset, size_t size);
    };
}