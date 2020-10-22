#include "Bulb/Rendering/RenderingHelpers.hpp"

#include "Bulb/Rendering/RenderingConstants.hpp"

#include <Clove/Graphics/DescriptorSetLayout.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/Shader.hpp>

namespace blb::rnd {
    std::unordered_map<DescriptorSetSlots, std::shared_ptr<clv::gfx::DescriptorSetLayout>> createDescriptorSetLayouts(clv::gfx::GraphicsFactory& factory) {
        std::unordered_map<DescriptorSetSlots, std::shared_ptr<clv::gfx::DescriptorSetLayout>> setLayouts;

        //MATERIAL SET
        {
            clv::gfx::DescriptorSetBindingInfo textureBinding{
                .binding   = 0,
                .type      = clv::gfx::DescriptorType::CombinedImageSampler,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Pixel,
            };

            setLayouts[DescriptorSetSlots::Material] = factory.createDescriptorSetLayout(clv::gfx::DescriptorSetLayout::Descriptor{
                .bindings = {
                    std::move(textureBinding),
                },
            });
        }

        //VIEW SET
        {
            clv::gfx::DescriptorSetBindingInfo viewDataBinding{
                .binding   = 0,
                .type      = clv::gfx::DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Vertex,
            };

            clv::gfx::DescriptorSetBindingInfo viewPosBinding{
                .binding   = 1,
                .type      = clv::gfx::DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Pixel,
            };

            setLayouts[DescriptorSetSlots::View] = factory.createDescriptorSetLayout(clv::gfx::DescriptorSetLayout::Descriptor{
                .bindings = {
                    std::move(viewDataBinding),
                    std::move(viewPosBinding) },
            });
        }

        //LIGHTING SET
        {
            clv::gfx::DescriptorSetBindingInfo lightDataBinding{
                .binding   = 0,
                .type      = clv::gfx::DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Pixel,
            };

            clv::gfx::DescriptorSetBindingInfo numLightBinding{
                .binding   = 1,
                .type      = clv::gfx::DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Vertex | clv::gfx::Shader::Stage::Pixel,
            };

            clv::gfx::DescriptorSetBindingInfo directionalShadowTransformBinding{
                .binding   = 2,
                .type      = clv::gfx::DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = clv::gfx::Shader::Stage::Vertex,
            };

            clv::gfx::DescriptorSetBindingInfo directionalShadowMapBinding{
                .binding   = 3,
                .type      = clv::gfx::DescriptorType::CombinedImageSampler,
                .arraySize = MAX_LIGHTS,
                .stage     = clv::gfx::Shader::Stage::Pixel,
            };

            clv::gfx::DescriptorSetBindingInfo pointShadowMapBinding{
                .binding   = 4,
                .type      = clv::gfx::DescriptorType::CombinedImageSampler,
                .arraySize = MAX_LIGHTS,
                .stage     = clv::gfx::Shader::Stage::Pixel,
            };

            setLayouts[DescriptorSetSlots::Lighting] = factory.createDescriptorSetLayout(clv::gfx::DescriptorSetLayout::Descriptor{
                .bindings = {
                    std::move(lightDataBinding),
                    std::move(numLightBinding),
                    std::move(directionalShadowTransformBinding),
                    std::move(directionalShadowMapBinding),
                    std::move(pointShadowMapBinding),
                },
            });
        }

        return setLayouts;
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