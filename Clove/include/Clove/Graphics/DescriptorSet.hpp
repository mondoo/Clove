#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    class GraphicsBuffer;
    class Sampler;
    class GraphicsImageView;
}

namespace clv::gfx {
    /**
     * @brief Used to associate data from certain containers into a binding inside a Shader.
     * @details Once a container has been written to a DescriptorSet then when ever the
     * DescriptorSet is bound then that container's data will be used for the associated
     * binding slot.
     */
    class DescriptorSet {
        //FUNCTIONS
    public:
        virtual ~DescriptorSet() = default;

        virtual void write(const GraphicsBuffer& buffer, const size_t offset, const size_t range, const uint32_t bindingSlot)                = 0;
        virtual void write(const GraphicsImageView& imageView, const Sampler& sampler, const ImageLayout layout, const uint32_t bindingSlot) = 0;
    };
}