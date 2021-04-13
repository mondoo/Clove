#pragma once

#include <Clove/Graphics/GhaBuffer.hpp>
#include <memory>

namespace garlic::clove {
    class GhaFactory;
}

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
        GhaBuffer::Descriptor ghaDescriptor{}; /**< Gets updated while the buffer is used within the graph. */

        //TODO: Keep some sort of internal ID (and maybe pointer to GhaBuffer) so when copying around it's easy to tell which RgBufferss are unique etc.

        //FUNCTIONS
    public:
        RgBuffer();
        

        RgBuffer(RgBuffer const &other);
        RgBuffer(RgBuffer &&other) noexcept;

        RgBuffer &operator=(RgBuffer const &other);
        RgBuffer &operator=(RgBuffer &&other) noexcept;

        ~RgBuffer();

        /**
         * @brief Write data into this buffer.
         * @param data Pointer to data to write.
         * @param offset Offset into the buffer to write to.
         * @param size Size of the region in the buffer to write to.
         */
        void write(void const *data, size_t const offset, size_t const size);

    private:
        RgBuffer(size_t bufferSize);
        RgBuffer(std::shared_ptr<GhaBuffer> buffer, size_t offset, size_t size);

        /**
         * @brief Create a GhaBuffer with it's usage set based on how it's used in the graph.
         * @param factory 
         * @return 
         */
        std::unique_ptr<GhaBuffer> createGhaBuffer(GhaFactory& factory) const;
    };
}