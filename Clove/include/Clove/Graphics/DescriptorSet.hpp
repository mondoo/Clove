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
     * @details A 'set' is defined in the shader by: layout(set = 0, binding = 1). Whenever
     * this set is bound it will represent what ever index it's layout is in the array passed
     * to the Pipeline::Descriptor. A DescriptorSet can then write to any number of binding
     * positions inside the set.
     * @note Once a container has been written to a DescriptorSet then when ever the
     * DescriptorSet is bound then that container's data will be used for the associated
     * binding slot. This allows multiple sets to reuse the same data.
     */
    class DescriptorSet {
        //FUNCTIONS
    public:
        virtual ~DescriptorSet() = default;

        virtual void write(const GraphicsBuffer& buffer, const size_t offset, const size_t range, const uint32_t bindingSlot)                = 0;
        virtual void write(const GraphicsImageView& imageView, const Sampler& sampler, const ImageLayout layout, const uint32_t bindingSlot) = 0;
    };
}