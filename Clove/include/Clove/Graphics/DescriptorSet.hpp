#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    class Buffer;
    class Sampler;
    class GraphicsImageView;
}

namespace clv::gfx {
    /**
     * @brief Represents an actual binding in a shader. Used to write data from certain objects into it.
     */
    class DescriptorSet {
        //FUNCTIONS
    public:
        virtual ~DescriptorSet() = default;

        virtual void writeFrom(const Buffer& buffer, const size_t offset, const size_t range, const uint32_t bindingSlot)                        = 0;
        virtual void writeFrom(const Sampler& sampler, const GraphicsImageView& imageView, const ImageLayout layout, const uint32_t bindingSlot) = 0;
    };
}