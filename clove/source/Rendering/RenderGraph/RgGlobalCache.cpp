#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"

#include "Clove/Rendering/RenderGraph/CacheUtils.hpp"
#include "Clove/Rendering/RenderGraph/RenderGraphLog.hpp"

#include <Clove/Graphics/GhaFactory.hpp>
#include <fstream>

namespace clove {
    RgGlobalCache::RgGlobalCache(GhaFactory *ghaFactory)
        : factory{ ghaFactory } {
    }

    RgGlobalCache::RgGlobalCache(RgGlobalCache &&other) noexcept = default;

    RgGlobalCache &RgGlobalCache::operator=(RgGlobalCache &&other) noexcept = default;

    RgGlobalCache::~RgGlobalCache() = default;

    GhaShader *RgGlobalCache::createShader(std::filesystem::path const &file, GhaShader::Stage shaderStage) {
        //TODO: Copied from the shader compiler. Ideally we'd cache the shader source somewhere. Perhaps in a shader library
        std::ifstream fileStream(file.c_str(), std::ios::ate | std::ios::binary);

        if(!fileStream.is_open()) {
            CLOVE_LOG(CloveRenderGraph, LogLevel::Error, "{0}: Failed to open file", CLOVE_FUNCTION_NAME);
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
        return shaders.at(shaderSource).get();
    }

    GhaShader *RgGlobalCache::createShader(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) {
        std::string const shaderSource{ source };

        if(!shaders.contains(shaderSource)) {
            shaders[shaderSource] = *factory->createShaderFromSource(source, std::move(includeSources), shaderName, shaderStage);
        }
        return shaders.at(shaderSource).get();
    }

    GhaSampler *RgGlobalCache::createSampler(GhaSampler::Descriptor descriptor) {
        PoolId samplerId{ 0 };
        CacheUtils::hashCombine(samplerId, descriptor.minFilter);
        CacheUtils::hashCombine(samplerId, descriptor.magFilter);
        CacheUtils::hashCombine(samplerId, descriptor.addressModeU);
        CacheUtils::hashCombine(samplerId, descriptor.addressModeV);
        CacheUtils::hashCombine(samplerId, descriptor.addressModeV);
        CacheUtils::hashCombine(samplerId, descriptor.enableAnisotropy);
        CacheUtils::hashCombine(samplerId, descriptor.maxAnisotropy);

        if(!samplers.contains(samplerId)) {
            samplers[samplerId] = factory->createSampler(std::move(descriptor)).getValue();
        }
        return samplers.at(samplerId).get();
    }

    GhaRenderPass *RgGlobalCache::createRenderPass(GhaRenderPass::Descriptor descriptor) {
        auto const hashAttachment = [](PoolId &passId, AttachmentDescriptor const &attachment) {
            CacheUtils::hashCombine(passId, attachment.format);
            CacheUtils::hashCombine(passId, attachment.loadOperation);
            CacheUtils::hashCombine(passId, attachment.storeOperation);
            CacheUtils::hashCombine(passId, attachment.initialLayout);
            CacheUtils::hashCombine(passId, attachment.usedLayout);
            CacheUtils::hashCombine(passId, attachment.finalLayout);

            return passId;
        };

        PoolId passId{};
        for(auto const &colourAttachment : descriptor.colourAttachments) {
            hashAttachment(passId, colourAttachment);
        }
        hashAttachment(passId, descriptor.depthAttachment);

        if(!renderPasses.contains(passId)) {
            renderPasses[passId] = *factory->createRenderPass(std::move(descriptor));
        }
        return renderPasses.at(passId).get();
    }

    GhaDescriptorSetLayout *RgGlobalCache::createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor descriptor) {
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
        return descriptorSetLayouts.at(layoutId).get();
    }

    GhaGraphicsPipelineObject *RgGlobalCache::createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor descriptor) {
        PoolId pipelineId{};

        bool vertFound{ false };
        bool pixelFound{ false };
        for(auto &&[source, shaderObject] : shaders) {
            if(!vertFound && shaderObject.get() == descriptor.vertexShader) {
                CacheUtils::hashCombine(pipelineId, source);
                vertFound = true;
            }

            if(pixelFound && shaderObject.get() == descriptor.pixelShader) {
                CacheUtils::hashCombine(pipelineId, source);
                pixelFound = true;
            }

            if(vertFound && pixelFound) {
                break;
            }
        }

        CacheUtils::hashCombine(pipelineId, descriptor.vertexInput.stride);
        for(auto const &attribute : descriptor.vertexAttributes) {
            CacheUtils::hashCombine(pipelineId, attribute.format);
            CacheUtils::hashCombine(pipelineId, attribute.offset);
        }

        CacheUtils::hashCombine(pipelineId, descriptor.viewportDescriptor.state);
        CacheUtils::hashCombine(pipelineId, descriptor.viewportDescriptor.position.x);
        CacheUtils::hashCombine(pipelineId, descriptor.viewportDescriptor.position.y);
        CacheUtils::hashCombine(pipelineId, descriptor.viewportDescriptor.size.x);
        CacheUtils::hashCombine(pipelineId, descriptor.viewportDescriptor.size.y);

        CacheUtils::hashCombine(pipelineId, descriptor.scissorDescriptor.state);
        CacheUtils::hashCombine(pipelineId, descriptor.scissorDescriptor.position.x);
        CacheUtils::hashCombine(pipelineId, descriptor.scissorDescriptor.position.y);
        CacheUtils::hashCombine(pipelineId, descriptor.scissorDescriptor.size.x);
        CacheUtils::hashCombine(pipelineId, descriptor.scissorDescriptor.size.y);

        //NOTE: No rasteriser descriptor implemented yet

        CacheUtils::hashCombine(pipelineId, descriptor.depthState.depthTest);
        CacheUtils::hashCombine(pipelineId, descriptor.depthState.depthWrite);

        CacheUtils::hashCombine(pipelineId, descriptor.enableBlending);

        CacheUtils::hashCombine(pipelineId, descriptor.renderPass);

        for(auto const &descriptorSetLayout : descriptor.descriptorSetLayouts) {
            CacheUtils::hashCombine(pipelineId, descriptorSetLayout);
        }

        for(auto const &pushConstantDescriptor : descriptor.pushConstants) {
            CacheUtils::hashCombine(pipelineId, pushConstantDescriptor.stage);
            CacheUtils::hashCombine(pipelineId, pushConstantDescriptor.offset);
            CacheUtils::hashCombine(pipelineId, pushConstantDescriptor.size);
        }

        if(!graphicsPipelines.contains(pipelineId)) {
            graphicsPipelines[pipelineId] = *factory->createGraphicsPipelineObject(std::move(descriptor));
        }
        return graphicsPipelines.at(pipelineId).get();
    }

    GhaComputePipelineObject *RgGlobalCache::createComputePipelineObject(GhaComputePipelineObject::Descriptor descriptor) {
        PoolId pipelineId{};

        for(auto &&[source, shaderObject] : shaders) {
            if(shaderObject.get() == descriptor.shader) {
                CacheUtils::hashCombine(pipelineId, source);
            }
        }

        for(auto const &descriptorSetLayout : descriptor.descriptorSetLayouts) {
            CacheUtils::hashCombine(pipelineId, descriptorSetLayout);
        }

        for(auto const &pushConstantDescriptor : descriptor.pushConstants) {
            CacheUtils::hashCombine(pipelineId, pushConstantDescriptor.stage);
            CacheUtils::hashCombine(pipelineId, pushConstantDescriptor.offset);
            CacheUtils::hashCombine(pipelineId, pushConstantDescriptor.size);
        }

        if(!computePipelines.contains(pipelineId)) {
            computePipelines[pipelineId] = *factory->createComputePipelineObject(std::move(descriptor));
        }
        return computePipelines.at(pipelineId).get();
    }
}
