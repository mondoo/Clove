#pragma once

#include "Bulb/Rendering/RenderingConstants.hpp"

#include <Clove/Graphics/Descriptor.hpp>

namespace clv::gfx {
    class DescriptorSetLayout;
    class GraphicsFactory;
}

namespace blb::rnd {
    /**
     * @brief Creates all descriptor sets according to DescriptorSetSlots.
     */
    std::unordered_map<DescriptorSetSlots, std::shared_ptr<clv::gfx::DescriptorSetLayout>> createDescriptorSetLayouts(clv::gfx::GraphicsFactory& factory);

    /**
     * @brief Counts the different binding types in a DescriptorSetLayout.
     * @returns A map where each key is the binding type and the value is the count.
     */
    std::unordered_map<clv::gfx::DescriptorType, uint32_t> countDescriptorBindingTypes(const clv::gfx::DescriptorSetLayout& descriptorSetLayout);
}