#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"

#include "Clove/Rendering/RenderGraph/CacheUtils.hpp"

#include <Clove/Graphics/GhaFactory.hpp>
#include <Clove/Log/Log.hpp>
#include <fstream>

namespace garlic::clove {
    RgGlobalCache::RgGlobalCache(std::shared_ptr<GhaFactory> ghaFactory)
        : factory{ std::move(ghaFactory) } {
    }

    RgGlobalCache::RgGlobalCache(RgGlobalCache &&other) noexcept = default;

    RgGlobalCache &RgGlobalCache::operator=(RgGlobalCache &&other) noexcept = default;

    RgGlobalCache::~RgGlobalCache() = default;

    std::shared_ptr<GhaShader> RgGlobalCache::createShader(std::filesystem::path const &file, GhaShader::Stage shaderStage) {
        //TODO: Copied from the shader compiler. Ideally we'd cache the shader source somewhere. Perhaps in a shader library
        std::ifstream fileStream(file.c_str(), std::ios::ate | std::ios::binary);

        if(!fileStream.is_open()) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "{0}: Failed to open file", CLOVE_FUNCTION_NAME);
            return {};
        }

        size_t const fileSize{ static_cast<size_t>(fileStream.tellg()) };
        std::vector<char> buffer(fileSize);

        fileStream.seekg(0);
        fileStream.read(buffer.data(), fileSize);

        fileStream.close();

        std::string const shaderSource{ buffer.begin(), buffer.end() };

        if(!shaders.contains(shaderSource)) {
            shaders[shaderSource] = *factory->createShaderFromFile(file, shaderStage);
        }
        return shaders.at(shaderSource);
    }

    std::shared_ptr<GhaShader> RgGlobalCache::createShader(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) {
        std::string const shaderSource{ source };

        if(!shaders.contains(shaderSource)) {
            shaders[shaderSource] = *factory->createShaderFromSource(source, std::move(includeSources), shaderName, shaderStage);
        }
        return shaders.at(shaderSource);
    }

    std::shared_ptr<GhaRenderPass> RgGlobalCache::createRenderPass(GhaRenderPass::Descriptor descriptor) {
        //TODO
        return nullptr;
    }

    std::shared_ptr<GhaDescriptorSetLayout> RgGlobalCache::createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor descriptor) {
        PoolId layoutId{};
        for(auto &binding : descriptor.bindings) {
            CacheUtils::hashCombine(layoutId, binding.binding);
            CacheUtils::hashCombine(layoutId, binding.type);
            CacheUtils::hashCombine(layoutId, binding.arraySize);
            CacheUtils::hashCombine(layoutId, binding.stage);
        }

        if(!descriptorSetLayouts.contains(layoutId)) {
            descriptorSetLayouts[layoutId] = factory->createDescriptorSetLayout(std::move(descriptor)).getValue();
        }

        return descriptorSetLayouts.at(layoutId);
    }

    std::shared_ptr<GhaGraphicsPipelineObject> RgGlobalCache::createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor descriptor) {
        //TODO
        return nullptr;
    }
}
