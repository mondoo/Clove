#pragma once

#include <Clove/Graphics/GhaBuffer.hpp>
#include <Clove/Graphics/GhaImage.hpp>
#include <Clove/Graphics/GhaFramebuffer.hpp>
#include <memory>
#include <vector>

namespace garlic::clove {
    /**
     * @brief Stores objects associated with a single frame (images, buffers, etc.) for a RenderGraph
     */
    class RgFrameCache {
        //VARIABLES
    private:
        std::vector<std::shared_ptr<GhaBuffer>> buffers{};
        
        //FUNCTIONS
    public:
        //TODO: ctors

        std::shared_ptr<GhaBuffer> allocateBuffer(GhaBuffer::Descriptor descriptor);
        std::shared_ptr<GhaImage> allocateImage(GhaImage::Descriptor descriptor);

        std::shared_ptr<GhaFramebuffer> allocateFrameBuffer(GhaFramebuffer::Descriptor descriptor);
    };
}