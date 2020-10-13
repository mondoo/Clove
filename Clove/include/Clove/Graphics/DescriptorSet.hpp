#pragma once

#include "Clove/Graphics/Descriptor.hpp"
#include "Clove/Graphics/GraphicsImage.hpp"

#include <span>

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

        /**
         * @brief Maps a region of a buffer into a binding inside a shader.
         * @param buffer The buffer to map from.
         * @param offset An offset into the buffer to start mapping from.
         * @param range The size of the region into the buffer to map.
         * @param bindingSlot The binding slot inside the shader to map to.
         */
        virtual void map(const GraphicsBuffer& buffer, const size_t offset, const size_t range, const uint32_t bindingSlot) = 0;

        /**
         * @brief Maps an image view into a binding inside a shader.
         * @param imageView The image view to map.
         * @param sampler A sampler describing how the image should be sampled.
         * @param layout The layout of the image being mapped.
         * @param bindingSlot The binding slot inside the shader to map to.
         */
        virtual void map(const GraphicsImageView& imageView, const Sampler& sampler, const GraphicsImage::Layout layout, const uint32_t bindingSlot)                       = 0;
        virtual void map(std::span<std::shared_ptr<GraphicsImageView>> imageViews, const Sampler& sampler, const GraphicsImage::Layout layout, const uint32_t bindingSlot) = 0;
    };
}