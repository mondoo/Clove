#pragma once

#include "Stem/Rendering/RenderingConstants.hpp"

#include <Clove/Graphics/Descriptor.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>

namespace clv::gfx {
    class DescriptorSetLayout;
    class GraphicsFactory;
}

namespace garlic::inline stem {
    /**
     * @brief Creates all descriptor sets according to DescriptorSetSlots.
     */
    std::unordered_map<DescriptorSetSlots, std::shared_ptr<clv::gfx::DescriptorSetLayout>> createDescriptorSetLayouts(clv::gfx::GraphicsFactory &factory);

    /**
     * @brief Counts the different binding types in a DescriptorSetLayout.
     * @returns A map where each key is the binding type and the value is the count.
     */
    std::unordered_map<clv::gfx::DescriptorType, uint32_t> countDescriptorBindingTypes(clv::gfx::DescriptorSetLayout const &descriptorSetLayout);

    /**
     * @brief Creates an image and then transfers the data into it.
     */
    std::unique_ptr<clv::gfx::GraphicsImage> createImageWithData(clv::gfx::GraphicsFactory &factory, clv::gfx::GraphicsImage::Descriptor imageDescriptor, void const *data, size_t const dataSize);
}