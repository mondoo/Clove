#pragma once

#include <Clove/Graphics/GhaBuffer.hpp>
#include <memory>

namespace garlic::clove {
    class GhaFactory;
}

namespace garlic::clove {
    /**
     * @brief Tracks the usage and dependencies of a buffer in the RenderGraph.
     * @details Can be used to later create a GhaBuffer when executing the graph.
     */
    class RgBuffer {
        friend class RenderGraph;

        //VARIABLES
    private:
        GhaBuffer::Descriptor ghaDescriptor{}; /**< Gets updated while the buffer is used within the graph. */

        //TODO: Keep some sort of internal ID (and maybe pointer to GhaBuffer) so when copying around it's easy to tell which RgBufferss are unique etc.

        //FUNCTIONS
    public:
        RgBuffer();
        RgBuffer(size_t bufferSize);

        RgBuffer(RgBuffer const &other);
        RgBuffer(RgBuffer &&other) noexcept;

        RgBuffer &operator=(RgBuffer const &other);
        RgBuffer &operator=(RgBuffer &&other) noexcept;

        ~RgBuffer();

    private:
        /**
         * @brief Create a GhaBuffer with it's usage set based on how it's used in the graph.
         * @param factory 
         * @return 
         */
        std::unique_ptr<GhaBuffer> createGhaBuffer(GhaFactory& factory) const;
    };
}