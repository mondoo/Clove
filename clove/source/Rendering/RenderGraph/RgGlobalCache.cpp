#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"

#include "Clove/Rendering/RenderGraph/CacheUtils.hpp"

#include <Clove/Graphics/GhaFactory.hpp>

namespace garlic::clove {
     RgGlobalCache::RgGlobalCache(std::shared_ptr<GhaFactory> ghaFactory)
        : factory{ std::move(ghaFactory) } {
    }

    RgGlobalCache::RgGlobalCache(RgGlobalCache &&other) noexcept = default;

    RgGlobalCache &RgGlobalCache::operator=(RgGlobalCache &&other) noexcept = default;

    RgGlobalCache::~RgGlobalCache() = default;

    std::shared_ptr<GhaShader> RgGlobalCache::createShader(std::filesystem::path const &file, GhaShader::Stage shaderStage) {
        //TODO
        return nullptr;
    }

    std::shared_ptr<GhaShader> RgGlobalCache::createShader(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) {
        //TODO
        return nullptr;
    }

    std::shared_ptr<GhaRenderPass> RgGlobalCache::createRenderPass(GhaRenderPass::Descriptor descriptor) {
        //TODO
        return nullptr;
    }

    std::shared_ptr<GhaDescriptorSetLayout> RgGlobalCache::createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor descriptor) {
        PoolId layoutId{};
        for(auto &binding : descriptor.bindings){
            CacheUtils::hashCombine(layoutId, binding.binding);
            CacheUtils::hashCombine(layoutId, binding.type);
            CacheUtils::hashCombine(layoutId, binding.arraySize);
            CacheUtils::hashCombine(layoutId, binding.stage);
        }

        if(!descriptorSetLayouts.contains(layoutId)){
            descriptorSetLayouts[layoutId] = factory->createDescriptorSetLayout(std::move(descriptor)).getValue();
        }

        return descriptorSetLayouts.at(layoutId);
    }

    std::shared_ptr<GhaGraphicsPipelineObject> RgGlobalCache::createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor descriptor) {
        //TODO
        return nullptr;
    }
}
