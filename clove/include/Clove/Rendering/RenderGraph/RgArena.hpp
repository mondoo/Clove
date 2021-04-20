#pragma once

#include <Clove/Graphics/GhaBuffer.hpp>
#include <memory>
#include <vector>

namespace garlic::clove {
    /**
     * @brief Tracks created objects for any given render graph.
     * Allowing for resources to be created once and reused.
     */
    class RgArena {
        //VARIABLES
    private:
        std::vector<std::shared_ptr<GhaBuffer>> buffers{};
        
        //FUNCTIONS
    public:
        //TODO: ctors

        std::shared_ptr<GhaBuffer> allocateBuffer(GhaBuffer::Descriptor descriptor);
    };
}