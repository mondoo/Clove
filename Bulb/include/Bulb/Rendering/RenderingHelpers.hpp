#pragma once

namespace clv::gfx {
    class DescriptorSetLayout;
    class GraphicsFactory;
}

namespace blb::rnd {
    /**
     * @brief Creates all descriptor sets according to DescriptorSetSlots. Index 0 is left empty for shader specific data.
     */
    std::vector<std::shared_ptr<clv::gfx::DescriptorSetLayout>> createDescriptorSets(clv::gfx::GraphicsFactory& factory);
}