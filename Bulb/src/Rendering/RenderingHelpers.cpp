#include "Bulb/Rendering/RenderingHelpers.hpp"

#include <Clove/Graphics/DescriptorSetLayout.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsTypes.hpp>

namespace blb::rnd {
    std::vector<std::shared_ptr<clv::gfx::DescriptorSetLayout>> createDescriptorSetLayouts(clv::gfx::GraphicsFactory& factory) {
        clv::gfx::DescriptorSetBindingInfo mvpLayoutBinding{};
        mvpLayoutBinding.binding   = 0;
        mvpLayoutBinding.type      = clv::gfx::DescriptorType::UniformBuffer;
        mvpLayoutBinding.arraySize = 1;
        mvpLayoutBinding.stage     = clv::gfx::DescriptorStage::Vertex;

        clv::gfx::DescriptorSetBindingInfo samplerLayoutBinding{};
        samplerLayoutBinding.binding   = 1;
        samplerLayoutBinding.type      = clv::gfx::DescriptorType::CombinedImageSampler;
        samplerLayoutBinding.arraySize = 1;
        samplerLayoutBinding.stage     = clv::gfx::DescriptorStage::Pixel;

        clv::gfx::DescriptorSetBindingInfo lightDataBindingInfo{};
        lightDataBindingInfo.binding   = 2;
        lightDataBindingInfo.type      = clv::gfx::DescriptorType::UniformBuffer;
        lightDataBindingInfo.arraySize = 1;
        lightDataBindingInfo.stage     = clv::gfx::DescriptorStage::Pixel;

        clv::gfx::DescriptorSetBindingInfo lightCountBindingInfo{};
        lightCountBindingInfo.binding   = 3;
        lightCountBindingInfo.type      = clv::gfx::DescriptorType::UniformBuffer;
        lightCountBindingInfo.arraySize = 1;
        lightCountBindingInfo.stage     = clv::gfx::DescriptorStage::Pixel;

        clv::gfx::DescriptorSetLayout::Descriptor descriptorSetLayoutDescriptor{};
        descriptorSetLayoutDescriptor.bindings = { mvpLayoutBinding, samplerLayoutBinding, lightDataBindingInfo, lightCountBindingInfo };

        return { factory.createDescriptorSetLayout(descriptorSetLayoutDescriptor) };
    }

    std::unordered_map<clv::gfx::DescriptorType, uint32_t> countElements(const clv::gfx::DescriptorSetLayout& descriptorSet) {
        std::unordered_map<clv::gfx::DescriptorType, uint32_t> counts;
        const auto& descriptor = descriptorSet.getDescriptor();
        for(auto& binding : descriptor.bindings) {
            counts[binding.type]++;
        }

        return counts;
    }
}