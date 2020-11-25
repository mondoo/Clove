#pragma once

#include "Clove/Rendering/RenderingConstants.hpp"

#include <Clove/Graphics/Descriptor.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <unordered_map>

namespace garlic::clove {
    class DescriptorSetLayout;
    class GraphicsFactory;
}

namespace garlic::clove {
    /**
     * @brief Creates all descriptor sets according to DescriptorSetSlots.
     */
    std::unordered_map<DescriptorSetSlots, std::shared_ptr<DescriptorSetLayout>> createDescriptorSetLayouts(GraphicsFactory &factory);

    /**
     * @brief Counts the different binding types in a DescriptorSetLayout.
     * @returns A map where each key is the binding type and the value is the count.
     */
    std::unordered_map<DescriptorType, uint32_t> countDescriptorBindingTypes(DescriptorSetLayout const &descriptorSetLayout);

    /**
     * @brief Creates an image and then transfers the data into it.
     */
    std::unique_ptr<GraphicsImage> createImageWithData(GraphicsFactory &factory, GraphicsImage::Descriptor imageDescriptor, void const *data, size_t const dataSize);
}