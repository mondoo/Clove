#pragma once

#include <Clove/Graphics/GhaDescriptorSetLayout.hpp>
#include <Clove/Graphics/GhaGraphicsPipelineObject.hpp>
#include <Clove/Graphics/GhaRenderPass.hpp>
#include <Clove/Graphics/GhaShader.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace garlic::clove {
    /**
     * @brief Stores objects that will be used regardless of what frame is being rendered (shaders, pipelines, etc.)
     */
    class RgGlobalCache {
        //VARIABLES
    private:
        //FUNCTIONS
    public:
        std::shared_ptr<GhaShader> createShader(std::filesystem::path const &file, GhaShader::Stage shaderStage);
        std::shared_ptr<GhaShader> createShader(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage);

        std::shared_ptr<GhaRenderPass> createRenderPass(GhaRenderPass::Descriptor descriptor);
        std::shared_ptr<GhaDescriptorSetLayout> createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor descriptor);
        std::shared_ptr<GhaGraphicsPipelineObject> createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor descriptor);
    };
}