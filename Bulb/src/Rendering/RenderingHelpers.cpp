#include "Bulb/Rendering/RenderingHelpers.hpp"

#include <Clove/Graphics/DescriptorSetLayout.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsTypes.hpp>

namespace blb::rnd {
    std::vector<std::shared_ptr<clv::gfx::DescriptorSetLayout>> createDescriptorSetLayouts(clv::gfx::GraphicsFactory& factory) {
        clv::gfx::DescriptorSetBindingInfo modelLayoutBinding{};
        modelLayoutBinding.binding   = 0;
        modelLayoutBinding.type      = clv::gfx::DescriptorType::UniformBuffer;
        modelLayoutBinding.arraySize = 1;
        modelLayoutBinding.stage     = clv::gfx::DescriptorStage::Vertex;

        clv::gfx::DescriptorSetBindingInfo viewLayoutBinding{};
        viewLayoutBinding.binding   = 1;
        viewLayoutBinding.type      = clv::gfx::DescriptorType::UniformBuffer;
        viewLayoutBinding.arraySize = 1;
        viewLayoutBinding.stage     = clv::gfx::DescriptorStage::Vertex;

        clv::gfx::DescriptorSetBindingInfo samplerLayoutBinding{};
        samplerLayoutBinding.binding   = 2;
        samplerLayoutBinding.type      = clv::gfx::DescriptorType::CombinedImageSampler;
        samplerLayoutBinding.arraySize = 1;
        samplerLayoutBinding.stage     = clv::gfx::DescriptorStage::Pixel;

        clv::gfx::DescriptorSetBindingInfo lightDataBindingInfo{};
        lightDataBindingInfo.binding   = 3;
        lightDataBindingInfo.type      = clv::gfx::DescriptorType::UniformBuffer;
        lightDataBindingInfo.arraySize = 1;
        lightDataBindingInfo.stage     = clv::gfx::DescriptorStage::Pixel;

        clv::gfx::DescriptorSetBindingInfo lightCountBindingInfo{};
        lightCountBindingInfo.binding   = 4;
        lightCountBindingInfo.type      = clv::gfx::DescriptorType::UniformBuffer;
        lightCountBindingInfo.arraySize = 1;
        lightCountBindingInfo.stage     = clv::gfx::DescriptorStage::Pixel;

        clv::gfx::DescriptorSetLayout::Descriptor descriptorSetLayoutDescriptor{};
        descriptorSetLayoutDescriptor.bindings = { modelLayoutBinding, viewLayoutBinding, samplerLayoutBinding, lightDataBindingInfo, lightCountBindingInfo };

        return { factory.createDescriptorSetLayout(descriptorSetLayoutDescriptor) };
    }

    std::unordered_map<clv::gfx::DescriptorType, uint32_t> countDescriptorBindingTypes(const clv::gfx::DescriptorSetLayout& descriptorSetLayout) {
        std::unordered_map<clv::gfx::DescriptorType, uint32_t> counts;
        const auto& descriptor = descriptorSetLayout.getDescriptor();
        for(auto& binding : descriptor.bindings) {
            counts[binding.type]++;
        }

        return counts;
    }
}