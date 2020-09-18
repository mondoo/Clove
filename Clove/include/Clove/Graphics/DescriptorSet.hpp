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
     * this set is bound it will represent what ever number is passed to 
     * GraphicsCommandBuffer::bindDescriptorSet which has to match the index of the 
     * DescriptorSetLayout in the array inside PiplineObject::Descriptor this set was allocated
     * from. Each binding is written to through this DescriptorSet
     * @note Once a container has been written to a DescriptorSet then when ever the
     * DescriptorSet is bound then that container's data will be used for the associated
     * binding slot. This allows multiple sets to reuse the same data.
     * @see DescriptorSetLayout
     * @see DescriptorPool
     */
    class DescriptorSet {
        //FUNCTIONS
    public:
        virtual ~DescriptorSet() = default;

        virtual void write(const GraphicsBuffer& buffer, const size_t offset, const size_t range, const uint32_t bindingSlot)                = 0;
        virtual void write(const GraphicsImageView& imageView, const Sampler& sampler, const ImageLayout layout, const uint32_t bindingSlot) = 0;
    };
}