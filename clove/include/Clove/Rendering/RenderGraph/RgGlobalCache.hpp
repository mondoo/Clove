#pragma once

#include <Clove/Graphics/GhaDescriptorSetLayout.hpp>
#include <Clove/Graphics/GhaGraphicsPipelineObject.hpp>
#include <Clove/Graphics/GhaComputePipelineObject.hpp>
#include <Clove/Graphics/GhaRenderPass.hpp>
#include <Clove/Graphics/GhaSampler.hpp>
#include <Clove/Graphics/GhaShader.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace clove {
    class GhaFactory;
}

namespace clove {
    /**
     * @brief Stores objects that will be used regardless of what frame is being rendered (shaders, pipelines, etc.)
     */
    class RgGlobalCache {
        //TYPES
    private:
        using PoolId = uint64_t;

        //VARIABLES
    private:
        GhaFactory *factory{ nullptr };

        std::unordered_map<std::string, std::unique_ptr<GhaShader>> shaders{};
        std::unordered_map<PoolId, std::unique_ptr<GhaSampler>> samplers{};
        std::unordered_map<PoolId, std::unique_ptr<GhaRenderPass>> renderPasses{};
        std::unordered_map<PoolId, std::unique_ptr<GhaDescriptorSetLayout>> descriptorSetLayouts{};
        std::unordered_map<PoolId, std::unique_ptr<GhaGraphicsPipelineObject>> graphicsPipelines{};
        std::unordered_map<PoolId, std::unique_ptr<GhaComputePipelineObject>> computePipelines{};

        //FUNCTIONS
    public:
        RgGlobalCache() = delete;
        RgGlobalCache(GhaFactory *ghaFactory);

        RgGlobalCache(RgGlobalCache const &other) = delete;
        RgGlobalCache(RgGlobalCache &&other) noexcept;

        RgGlobalCache &operator=(RgGlobalCache const &other) = delete;
        RgGlobalCache &operator=(RgGlobalCache &&other) noexcept;

        ~RgGlobalCache();

        GhaShader *createShader(std::filesystem::path const &file, GhaShader::Stage shaderStage);
        GhaShader *createShader(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage);

        GhaSampler *createSampler(GhaSampler::Descriptor descriptor);

        GhaRenderPass *createRenderPass(GhaRenderPass::Descriptor descriptor);
        GhaDescriptorSetLayout *createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor descriptor);
        GhaGraphicsPipelineObject *createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor descriptor);

        GhaComputePipelineObject *createComputePipelineObject(GhaComputePipelineObject::Descriptor descriptor);
    };
}